// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHWRITERSERVICE_H
#define JUNGFRAUJOCH_JFJOCHWRITERSERVICE_H

#include <shared_mutex>
#include "jfjoch.grpc.pb.h"
#include "JFJochWriter.h"

class JFJochWriterService final : public JFJochProtoBuf::gRPC_JFJochWriter::Service {
    std::shared_mutex m;
    std::unique_ptr<JFJochWriter> writer;
    Logger &logger;
    ZMQContext &zmq_context;
    std::string zmq_addr;
public:
    JFJochWriterService(ZMQContext &in_context, std::string in_zmq_addr, Logger &in_logger);
    grpc::Status Start(grpc::ServerContext *context, const JFJochProtoBuf::JFJochWriterInput *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                      JFJochProtoBuf::Empty *response) override;
    grpc::Status Abort(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status WriteMasterFile(grpc::ServerContext *context, const JFJochProtoBuf::JFJochReceiverOutput *request,
                                 JFJochProtoBuf::Empty *response) override;
};


#endif //JUNGFRAUJOCH_JFJOCHWRITERSERVICE_H
