// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHDETECTORCLIENT_H
#define JUNGFRAUJOCH_JFJOCHDETECTORCLIENT_H

#include <string>
#include <jfjoch.grpc.pb.h>

#include "../common/DiffractionExperiment.h"

class JFJochDetectorClient {
    std::unique_ptr<JFJochProtoBuf::gRPC_JFJochDetector::Stub> _stub;
public:
    void Connect(const std::string &addr);
    void Start(const DiffractionExperiment &request);
    void Stop();
    JFJochProtoBuf::DetectorStatus GetStatus();
    void On(const JFJochProtoBuf::DetectorConfig &request);
    void Off();
    void Trigger();
};


#endif //JUNGFRAUJOCH_JFJOCHDETECTORCLIENT_H
