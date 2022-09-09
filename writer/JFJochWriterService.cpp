// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochWriterService.h"

#include <utility>
#include <filesystem>

JFJochWriterService::JFJochWriterService(ZMQContext &in_context, std::string in_zmq_addr, Logger &in_logger) :
logger(in_logger), zmq_context(in_context), zmq_addr(std::move(in_zmq_addr)) {}

grpc::Status
JFJochWriterService::WriteMasterFile(grpc::ServerContext *context, const JFJochProtoBuf::JFJochWriterMetadataInput *request,
                              JFJochProtoBuf::Empty *response) {
    try {
        logger.Info("Writing master file");
        if (request->has_calibration())
            logger.Info("   ... request has calibration info");
        WriteHDF5MasterFile(*request);
        logger.Info("   ... done.");
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochWriterService::Start(grpc::ServerContext *context, const JFJochProtoBuf::JFJochWriterInput *request,
                                        JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::shared_mutex> ul(m);
    try {
        if (writer)
            writer.reset();
        logger.Info("Starting writer");
        writer = std::make_unique<JFJochWriter>(DiffractionExperiment(request->jungfraujoch_settings()),
                                                zmq_context, zmq_addr, logger);
        logger.Info("   ... done.");
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochWriterService::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                       JFJochProtoBuf::Empty *response) {
    try {
        {
            std::shared_lock<std::shared_mutex> ul(m);
            logger.Info("Stopping writer");
            writer->Stop();
        }
        {
            std::unique_lock<std::shared_mutex> ul(m);
            if (writer)
                writer.reset();
        }
        logger.Info("Done");
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochWriterService::Abort(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                        JFJochProtoBuf::Empty *response) {
    std::shared_lock<std::shared_mutex> ul(m);
    writer->Abort();
    return grpc::Status::OK;
}


JFJochWriterService &JFJochWriterService::BaseDirectory(const std::string &input) {
    logger.Info("Setting base directory to " + input);
    try {
        std::filesystem::current_path(input);
    } catch (const std::filesystem::filesystem_error& err) {
        logger.Error("Cannot set base directory: " + std::string(err.what()));
        throw JFJochException(JFJochExceptionCategory::FileWriteError,
                              "Cannot set base directory " + std::string(err.what()));
    }
    return *this;
}