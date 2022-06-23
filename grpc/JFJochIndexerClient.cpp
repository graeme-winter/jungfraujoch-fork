// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <grpcpp/grpcpp.h>
#include "JFJochIndexerClient.h"
#include "../common/JFJochException.h"

void JFJochIndexerClient::Connect(const std::string &addr) {
    if (addr.empty()) _stub.reset();
    else {
        grpc::ChannelArguments ch_args;
        ch_args.SetMaxReceiveMessageSize(GRPC_MAX_MESSAGE_SIZE);
        auto ch = grpc::CreateCustomChannel(addr, grpc::InsecureChannelCredentials(), ch_args);
        _stub = std::make_unique<JFJochProtoBuf::gRPC_JFJochIndexer::Stub>(ch);
    }
}

JFJochIndexerClient &JFJochIndexerClient::ZMQReceiverAddr(const std::string &receiver_addr) {
    zmq_addr = receiver_addr;
    return *this;
}

void JFJochIndexerClient::Start(const DiffractionExperiment &x) {
    JFJochProtoBuf::JFJochIndexerInput input;
    *input.mutable_jungfraujoch_settings() = x;
    input.set_zmq_recv_pub_addr(zmq_addr);
    input.set_bin_size(x.GetSpotFindingBin());

    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Start(&context, input, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochIndexer: " + status.error_message());
    }
}

JFJochProtoBuf::JFJochIndexerOutput JFJochIndexerClient::Stop() {
    JFJochProtoBuf::JFJochIndexerOutput ret;
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;
        auto status = _stub->Stop(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochIndexer: " + status.error_message());
    }
    return ret;
}

JFJochProtoBuf::IndexerStatus JFJochIndexerClient::GetStatus() {
    JFJochProtoBuf::IndexerStatus ret;
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;
        auto status = _stub->GetStatus(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochIndexer: " + status.error_message());
    }
    return ret;
}