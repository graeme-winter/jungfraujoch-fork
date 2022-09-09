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

void JFJochWriterClient::Start(const DiffractionExperiment &experiment) {
    JFJochProtoBuf::JFJochWriterInput writer_input;
    *writer_input.mutable_jungfraujoch_settings() = experiment;

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

void JFJochWriterClient::Stop() {
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;
        auto status = _stub->Stop(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
}

void JFJochWriterClient::WriteMasterFile(const JFJochProtoBuf::JFJochReceiverOutput &input,
                                         const JFJochProtoBuf::JFCalibration &calibration) {
    if (_stub) {
        JFJochProtoBuf::Empty empty;
        grpc::ClientContext context;

        JFJochProtoBuf::JFJochWriterMetadataInput request;
        *request.mutable_receiver_output() = input;
        *request.mutable_calibration() = calibration;

        auto status = _stub->WriteMasterFile(&context, request, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochWriter: " + status.error_message());
    }
}