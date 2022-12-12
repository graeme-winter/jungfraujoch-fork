// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include "JFJochWriter.h"
#include "MakeDirectory.h"

JFJochWriter::JFJochWriter(const JFJochProtoBuf::WriterInput &request, ZMQContext& context, Logger &in_logger) :
       logger(in_logger), image_puller(context) {

    logger.Info("Write request for dataset: " + request.file_prefix() + " ...");

    logger.Info("   ... creating subdirectory");
    MakeDirectory(request.file_prefix());

    logger.Info("   ... creating HDF5 files");
    data_file_set = std::make_unique<HDF5Writer>(request);

    logger.Info("   ... connecting to PUSH ZeroMQ socket " + request.zmq_receiver_address());
    image_puller.Connect(request.zmq_receiver_address());

    logger.Info("   ... starting image receiver");
    measurement = std::async(std::launch::async, &JFJochWriter::MeasurementThread, this);

    logger.Info("   ... ready to write images!");
}

JFJochProtoBuf::WriterOutput JFJochWriter::MeasurementThread() {
    auto image = std::make_shared<std::vector<uint8_t>>();
    image->reserve(8*1024*1024); // 8 MiB

    try {
        image_puller.WaitForImage();

        while (image_puller.GetFrameType() != JFJochFrameDeserializer::Type::START)
            image_puller.WaitForImage();

        while (image_puller.GetFrameType() != JFJochFrameDeserializer::Type::END) {
            if (image_puller.GetFrameType() == JFJochFrameDeserializer::Type::IMAGE)
                data_file_set->Write(image_puller.GetImage(),
                                 image_puller.GetImageSize(),
                                     image_puller.GetSpots(),
                                 image_puller.GetImageNumber());
            image_puller.WaitForImage();
        }
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        image_puller.Abort();
    }

    image_puller.Disconnect();
    data_file_set.reset();
    auto stats = image_puller.GetStatistics();
    logger.Info("Write task done. Images = {} Throughput = {:.0f} MB/s Frame rate = {:.0f} Hz",
                stats.nimages(), stats.performance_mbs(), stats.performance_hz());

    return stats;
}

void JFJochWriter::Abort() {
    image_puller.Abort();
}

JFJochProtoBuf::WriterOutput JFJochWriter::Stop() {
    return measurement.get();
}

JFJochWriter::~JFJochWriter() {
    try {
        Stop();
    } catch (...) {}
}