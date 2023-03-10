// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <grpcpp/grpcpp.h>
#include "JFJochReceiverClient.h"
#include "../common/JFJochException.h"

void JFJochReceiverClient::Connect(const std::string& addr)
{
    if (addr.empty()) _stub.reset();
    else {
        grpc::ChannelArguments ch_args;
        ch_args.SetMaxReceiveMessageSize(GRPC_MAX_MESSAGE_SIZE);
        auto ch = grpc::CreateCustomChannel(addr, grpc::InsecureChannelCredentials(), ch_args);
        _stub = std::make_unique<JFJochProtoBuf::gRPC_JFJochReceiver::Stub>(ch);
    }
};

void JFJochReceiverClient::Start(const DiffractionExperiment &experiment, const JFCalibration *calibration,
                                 const std::vector<std::string> &writer_zmq_addr) {
    JFJochProtoBuf::ReceiverInput receiver_input;

    *receiver_input.mutable_jungfraujoch_settings() = experiment;
    if (calibration != nullptr)
        *receiver_input.mutable_calibration() = *calibration;
    for (const auto& i: writer_zmq_addr)
        receiver_input.add_writer_zmq_address(i);

    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Start(&context, receiver_input, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
};

void JFJochReceiverClient::Cancel() {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Cancel(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
};

void JFJochReceiverClient::Abort() {
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Abort(&context, empty, &empty);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
};

JFJochProtoBuf::ReceiverOutput JFJochReceiverClient::Stop() {
    JFJochProtoBuf::ReceiverOutput ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->Stop(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
    return ret;
};

JFJochProtoBuf::ReceiverStatus JFJochReceiverClient::GetStatus() {
    JFJochProtoBuf::ReceiverStatus ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->GetStatus(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
    return ret;
}

void JFJochReceiverClient::SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings) {
    JFJochProtoBuf::Empty ret;
    if (_stub) {
        grpc::ClientContext context;
        auto status = _stub->SetDataProcessingSettings(&context, settings, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
}

JFJochProtoBuf::PreviewFrame JFJochReceiverClient::GetPreviewFrame() {
    JFJochProtoBuf::PreviewFrame ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->GetPreviewFrame(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    }
    return ret;
}

JFJochProtoBuf::ReceiverNetworkConfig JFJochReceiverClient::GetNetworkConfig() {
    JFJochProtoBuf::ReceiverNetworkConfig ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->GetNetworkConfig(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    } else {
        // For tests to work, dummy receiver needs to replay with nonsense MAC addresses
        ret.add_fpga_mac_addr("00:00:00:00:00:00");
        ret.add_fpga_mac_addr("00:00:00:00:00:01");
    }
    return ret;
}

JFJochProtoBuf::ReceiverDataProcessingPlots JFJochReceiverClient::GetPlots() {
    JFJochProtoBuf::ReceiverDataProcessingPlots ret;
    if (_stub) {
        grpc::ClientContext context;
        JFJochProtoBuf::Empty empty;
        auto status = _stub->GetDataProcessingPlots(&context, empty, &ret);
        if (!status.ok()) throw JFJochException(JFJochExceptionCategory::gRPCError,
                                                "JFJochReceiver: " + status.error_message());
    } else {
        // TODO: Write some dummy plots
    }
    return ret;

}