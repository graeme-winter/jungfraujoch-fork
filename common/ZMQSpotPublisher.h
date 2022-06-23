// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQSPOTPUBLISHER_H
#define JUNGFRAUJOCH_ZMQSPOTPUBLISHER_H

#include "ZMQWrappers.h"
#include "DiffractionExperiment.h"
#include "JungfrauCalibration.h"
#include "DiffractionSpot.h"

class ZMQSpotPublisher {
    ZMQSocket socket;
public:
    ZMQSpotPublisher(ZMQContext& context, const std::string& addr);
    void Publish(const DiffractionExperiment& experiment, const std::vector<DiffractionSpot> &vec,
                 int64_t image_number);
    void PublishReciprocal(const DiffractionExperiment& experiment, const std::vector<Coord> &vec,
                           int64_t image_number);
};


#endif //JUNGFRAUJOCH_ZMQSPOTPUBLISHER_H
