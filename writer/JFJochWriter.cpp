// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochWriter.h"
#include "../common/JungfrauCalibration.h"
#include <unistd.h>

#include <utility>

JFJochWriter::JFJochWriter(DiffractionExperiment  in_experiment, ZMQContext& context, const std::string& zmq_addr,
                           Logger &in_logger) :
        experiment(std::move(in_experiment)), logger(in_logger), image_puller(context, zmq_addr) {

    SetGroupNumber(experiment);

    logger.Info("Write request for dataset: " + experiment.GetFilePrefix() + " ...");

    logger.Info("   ... creating HDF5 files");
    data_file_set = std::make_unique<HDF5Writer>(experiment);

    logger.Info("   ... starting image receiver");
    writing_future = std::async(std::launch::async, &JFJochWriter::WritingThread, this);
    pulling_future = std::async(std::launch::async, &JFJochWriter::PullingThread, this);
    measurement = std::async(std::launch::async, &JFJochWriter::MeasurementThread, this);

    logger.Info("   ... ready to write images!");
}

void JFJochWriter::WritingThread() {
    auto image = image_fifo.GetBlocking();
    while (image != nullptr) {
        try {
            data_file_set->WriteFromStream(image->data(), image->size());
        } catch (const JFJochException& e) {
            logger.ErrorException(e);
            image_puller.Abort();
        }
        image = image_fifo.GetBlocking();
    }
}

void JFJochWriter::PullingThread() {
    auto image = std::make_shared<std::vector<uint8_t>>();
    int64_t msg_size = image_puller.GetImage(*image);
    while (msg_size > 0) {
        image_fifo.PutBlocking(image);
        image = std::make_shared<std::vector<uint8_t>>();
        try {
            msg_size = image_puller.GetImage(*image);
        } catch (JFJochException &e) {
            logger.ErrorException(e);
            image_puller.Abort();
            msg_size = 0;
        }
    }
    image_fifo.PutBlocking(nullptr);
}

void JFJochWriter::MeasurementThread() {
    pulling_future.get();
    writing_future.get();
    data_file_set.reset();
    auto stats = image_puller.GetStatistics();
    logger.Info("Write task done. Images = " + std::to_string(stats.nimages)
                + " Throughput = " + std::to_string(stats.throughput_MBs) + " MB/s"
                + " Frame rate = " + std::to_string(stats.throughput_Hz) + " Hz");
}

void JFJochWriter::Abort() {
    image_puller.Abort();
}

void JFJochWriter::WaitTillDone() {
    measurement.get();
}

void JFJochWriter::Stop() {
    Abort();
    WaitTillDone();
}

JFJochWriter::~JFJochWriter() {
    try {
        Stop();
    } catch (...) {}
}

void JFJochWriter::SetGroupNumber(const DiffractionExperiment &experiment) {
    auto gid = getgid();
    if (experiment.GetGroupID() != DiffractionExperiment::GroupIDNotSet)
           gid = experiment.GetGroupID();
    logger.Info("Switching to group ID " + std::to_string(gid));

    if (setegid(gid) != 0) {
        if (errno == EINVAL)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Group ID invalid");
        else if (errno == EPERM)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Group ID permission issue");
        else
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Group ID setting error");
    }
}