// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <grpcpp/grpcpp.h>

#include "JFJochDetectorClient.h"
#include "../common/JFJochException.h"

void JFJochDetectorClient::Connect(const std::string &addr) {
    if (addr.empty()) _stub.reset();
    else {
        _stub = std::make_unique<JFJochProtoBuf::gRPC_JFJochDetector::Stub>(
                grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()));
    }
}

void JFJochDetectorClient::Start(const DiffractionExperiment &in_experiment) {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::DetectorInput request = in_experiment;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Start(&context, request, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochDetector: " + status.error_message());
    }
}

void JFJochDetectorClient::Stop() {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Stop(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochDetector: " + status.error_message());
    }
}

void JFJochDetectorClient::On(const JFJochProtoBuf::DetectorConfig &request) {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->On(&context, request, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochDetector: " + status.error_message());
    }
}

void JFJochDetectorClient::Off() {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Off(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochDetector: " + status.error_message());
    }
}

JFJochProtoBuf::DetectorStatus JFJochDetectorClient::GetStatus() {
    JFJochProtoBuf::DetectorStatus ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Status(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochDetector: " + status.error_message());
    } else {
        ret.set_status(JFJochProtoBuf::NO_DETECTOR);
    }
    return ret;
}