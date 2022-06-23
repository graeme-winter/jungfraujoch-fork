// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_GRPCSERVER_TEMPLATE_H
#define JUNGFRAUJOCH_GRPCSERVER_TEMPLATE_H

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>
#include "../common/Definitions.h"

inline std::unique_ptr<grpc::Server> gRPCServer(const std::string &addr, grpc::Service &service, int32_t max_threads = -1) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.SetMaxReceiveMessageSize(GRPC_MAX_MESSAGE_SIZE); // 1 GB
    builder.RegisterService(&service);
    if (max_threads > 0) {
        grpc::ResourceQuota rq;
        rq.SetMaxThreads(max_threads);
        builder.SetResourceQuota(rq);
    }
    return builder.BuildAndStart();
}

#endif //JUNGFRAUJOCH_GRPCSERVER_TEMPLATE_H
