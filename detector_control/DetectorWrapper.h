// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_DETECTORWRAPPER_H
#define JUNGFRAUJOCH_DETECTORWRAPPER_H

#include <sls/Detector.h>
#include <jfjoch.pb.h>

#define BASE_DETECTOR_MAC 0xAABBCCDDEE10 // little-endian!
#define HIGH_VOLTAGE      120

class DetectorWrapper {
    sls::Detector det;
    void InternalStop();
public:
    enum class DetectorState {IDLE, ERROR, BUSY};
    [[nodiscard]] DetectorState GetState() const;
    void Configure(const JFJochProtoBuf::DetectorConfig &config);
    void Start(const JFJochProtoBuf::DetectorInput &request);
    void Stop();
    void Trigger();
    void Deactivate();
    int64_t GetFirmwareVersion();
    int64_t GetDetectorServerVersion();
};


#endif //JUNGFRAUJOCH_DETECTORWRAPPER_H
