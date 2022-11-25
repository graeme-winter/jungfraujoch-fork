// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochIndexerService.h"

JFJochIndexerService::JFJochIndexerService(ZMQContext &in_zmq_context, Logger &in_logger)
: zmq_context(in_zmq_context), logger(in_logger) {}

JFJochIndexerService &JFJochIndexerService::NumThreads(int64_t input) {
    if (input <= 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Thread number must be above zero");
    nthreads = input;
    return *this;
}


grpc::Status
JFJochIndexerService::Start(grpc::ServerContext *context, const JFJochProtoBuf::IndexerInput *request,
                            JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::shared_mutex> ul(indexer_mutex);
    try {
        indexer.reset();
        indexer = std::make_unique<JFJochIndexer>(zmq_context, *request, logger, nthreads);
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochIndexerService::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                        JFJochProtoBuf::IndexerOutput *response) {
    std::shared_lock<std::shared_mutex> sl(indexer_mutex);
    if (indexer)
        *response = indexer->End();
    return grpc::Status::OK;
}

grpc::Status JFJochIndexerService::GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                             JFJochProtoBuf::IndexerStatus *response) {
    std::shared_lock<std::shared_mutex> sl(indexer_mutex);
    if (indexer)
        *response = indexer->GetStatus();
    return grpc::Status::OK;
}