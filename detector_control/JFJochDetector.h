// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DETECTORWRAPPER_H
#define DETECTORWRAPPER_H

#include <sls/Detector.h>
#include "jfjoch.grpc.pb.h"
#include "DetectorWrapper.h"

#include "../common/Logger.h"

#define BASE_DETECTOR_MAC 0xAABBCCDDEE10 // little-endian!
#define HIGH_VOLTAGE      120

class JFJochDetector final : public JFJochProtoBuf::gRPC_JFJochDetector::Service {
    std::mutex m;
    DetectorWrapper detector;
public:
    grpc::Status Start(grpc::ServerContext *context, const JFJochProtoBuf::DetectorInput *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                      JFJochProtoBuf::Empty *response) override;
    grpc::Status Status(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                      JFJochProtoBuf::DetectorStatus *response) override;
    grpc::Status On(grpc::ServerContext *context, const JFJochProtoBuf::DetectorConfig *request,
                    JFJochProtoBuf::Empty *response) override;
    grpc::Status Off(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                     JFJochProtoBuf::Empty *response) override;
    grpc::Status Trigger(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                         JFJochProtoBuf::Empty *response) override;

};

#endif //JUNGFRAUJOCH_DETECTORWRAPPER_H
