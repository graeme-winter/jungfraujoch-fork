// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVERCLIENT_H
#define JUNGFRAUJOCH_JFJOCHRECEIVERCLIENT_H

#include <string>
#include <jfjoch.grpc.pb.h>

#include "../common/DiffractionExperiment.h"
#include "../common/JFCalibration.h"

class JFJochReceiverClient {
    std::unique_ptr<JFJochProtoBuf::gRPC_JFJochReceiver::Stub> _stub;
public:
    void Connect(const std::string& addr);
    void Start(const DiffractionExperiment &experiment, const JFCalibration *calibration,
               const std::vector<std::string> &writer_zmq_addr);
    void Abort();
    void Cancel();
    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings& settings);
    JFJochProtoBuf::ReceiverOutput Stop();
    JFJochProtoBuf::ReceiverStatus GetStatus();
    JFJochProtoBuf::PreviewFrame GetPreviewFrame();
    JFJochProtoBuf::ReceiverNetworkConfig GetNetworkConfig();
};


#endif //JUNGFRAUJOCH_JFJOCHRECEIVERCLIENT_H
