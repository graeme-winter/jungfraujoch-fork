// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include "JFJochWriter.h"

#include <utility>
#include <sys/stat.h>
#include <sys/types.h>

JFJochWriter::JFJochWriter(DiffractionExperiment  in_experiment, ZMQContext& context, const std::string& zmq_addr,
                           Logger &in_logger) :
        experiment(std::move(in_experiment)), logger(in_logger), image_puller(context, zmq_addr) {

    logger.Info("Write request for dataset: " + experiment.GetFilePrefix() + " ...");

    logger.Info("   ... creating subdirectory");
    MakeDirectory(experiment.GetFilePrefix());

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

void JFJochWriter::MakeDirectory(const std::string &path) {
    // If first character is /, then it makes no sense to attempt to create root directory anyway
    auto pos = path.find('/' , 1);

    while (pos != std::string::npos) {
        std::string dir_name = path.substr(0, pos);

        struct stat info{};

        if( stat( dir_name.c_str(), &info ) == 0) {
            // Something exists in the filesystem, need to check if this is directory or file
            if ( (info.st_mode & S_IFDIR) == 0)
                throw JFJochException(JFJochExceptionCategory::FileWriteError,
                                      "Cannot create directory - file with this name exists");
        } else {
            int ret = mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_ISGID);
            if (ret == -1)
                throw JFJochException(JFJochExceptionCategory::FileWriteError, "Cannot create directory");
            ret = chmod(dir_name.c_str(), S_IRWXU | S_IRWXG | S_ISGID | S_IROTH | S_IXOTH);
            if (ret == -1)
                throw JFJochException(JFJochExceptionCategory::FileWriteError, "Cannot set permissions");

        }
        pos = path.find('/', pos + 1);
    }
}

JFJochWriter::~JFJochWriter() {
    try {
        Stop();
    } catch (...) {}
}