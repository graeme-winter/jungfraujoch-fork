// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <grpcpp/grpcpp.h>

#include "JFJochTriggerClient.h"
#include "../common/JFJochException.h"

void JFJochTriggerClient::Connect(const std::string &addr) {
    if (addr.empty()) _stub.reset();
    else {

        grpc::ChannelArguments ch_args;
        ch_args.SetInt(GRPC_ARG_KEEPALIVE_TIME_MS, 2000);
        ch_args.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, 5000);
        ch_args.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);
        ch_args.SetInt(GRPC_ARG_HTTP2_MAX_PINGS_WITHOUT_DATA, 0);

        _stub = std::make_unique<JFJochProtoBuf::gRPC_JFJochTrigger::Stub>
                (grpc::CreateCustomChannel(addr, grpc::InsecureChannelCredentials(), ch_args));
    }
}

void JFJochTriggerClient::Trigger() {
    if (_stub) {
        grpc::ClientContext context;

        JFJochProtoBuf::Empty empty;
        JFJochProtoBuf::Empty empty2;

        auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
        context.set_deadline(deadline);

        auto status = _stub->Trigger(&context, empty, &empty2);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochTrigger: " + status.error_message());
    }
}