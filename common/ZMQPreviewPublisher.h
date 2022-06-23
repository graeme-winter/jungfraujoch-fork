// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQPREVIEWPUBLISHER_H
#define JUNGFRAUJOCH_ZMQPREVIEWPUBLISHER_H

#include "ZMQWrappers.h"
#include "DiffractionExperiment.h"
#include "JungfrauCalibration.h"
#include "jfjoch.pb.h"

class ZMQPreviewPublisher  {
    ZMQSocket socket;

    mutable std::mutex frame_mutex;
    JFJochProtoBuf::PreviewFrame saved_frame;
    void SetPreviewImage(const JFJochProtoBuf::PreviewFrame &frame);
public:
    ZMQPreviewPublisher(ZMQContext& context, const std::string& addr);
    void Start(const DiffractionExperiment& experiment, const JungfrauCalibration &calibration);
    void Publish(const DiffractionExperiment& experiment, const int16_t* image_data, uint32_t image_number);
    void Stop(const DiffractionExperiment& experiment);
    JFJochProtoBuf::PreviewFrame GetPreviewImage() const;
};



#endif //JUNGFRAUJOCH_ZMQPREVIEWPUBLISHER_H
