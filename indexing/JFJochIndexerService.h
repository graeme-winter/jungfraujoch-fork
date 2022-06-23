// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHINDEXERSERVICE_H
#define JUNGFRAUJOCH_JFJOCHINDEXERSERVICE_H

#include <shared_mutex>
#include "JFJochIndexer.h"
#include "jfjoch.grpc.pb.h"
#include "../common/ZMQWrappers.h"
#include "../common/Logger.h"

class JFJochIndexerService : public JFJochProtoBuf::gRPC_JFJochIndexer::Service {
    Logger &logger;
    ZMQContext &zmq_context;
    int64_t nthreads = 16;
    std::unique_ptr<JFJochIndexer> indexer;
    std::shared_mutex indexer_mutex;
public:
    JFJochIndexerService(ZMQContext &zmq_context, Logger &logger);
    JFJochIndexerService &NumThreads(int64_t input);
    grpc::Status Start(grpc::ServerContext *context, const JFJochProtoBuf::JFJochIndexerInput *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                      JFJochProtoBuf::JFJochIndexerOutput *response) override;
    grpc::Status GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                           JFJochProtoBuf::IndexerStatus *response) override;
};


#endif //JUNGFRAUJOCH_JFJOCHINDEXERSERVICE_H
