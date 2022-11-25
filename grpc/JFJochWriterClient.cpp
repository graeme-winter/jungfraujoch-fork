// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <grpcpp/grpcpp.h>

#include "JFJochWriterClient.h"
#include "../common/JFJochException.h"

void JFJochWriterClient::Connect(const std::string &addr) {
    if (addr.empty()) _stub.reset();
    else {
        _stub = std::make_unique<JFJochProtoBuf::gRPC_JFJochWriter::Stub>(
                grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()));
    }
}

void JFJochWriterClient::Start(const DiffractionExperiment &experiment, const std::string zmq_push_addr) {
    JFJochProtoBuf::WriterInput writer_input = experiment;
    writer_input.set_zmq_receiver_address(zmq_push_addr);

    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Start(&context, writer_input, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
}

void JFJochWriterClient::Abort() {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Abort(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
}

JFJochProtoBuf::WriterOutput JFJochWriterClient::Stop() {
    JFJochProtoBuf::WriterOutput ret;
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;
        auto status = _stub->Stop(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
    return ret;
}

void JFJochWriterClient::WriteMasterFile(JFJochProtoBuf::WriterMetadataInput &request) {
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;

        auto status = _stub->WriteMasterFile(&context, request, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
}