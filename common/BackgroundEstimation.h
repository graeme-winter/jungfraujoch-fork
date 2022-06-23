// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_BACKGROUNDESTIMATION_H
#define JUNGFRAUJOCH_BACKGROUNDESTIMATION_H

#include <vector>
#include <cstdint>

#include "DiffractionExperiment.h"

class BackgroundEstimation {
    std::vector<uint8_t> mask;
    const int64_t npixel;
    bool enable;
public:
    BackgroundEstimation(const DiffractionExperiment &experiment);
    std::pair<float, float> Process(const int16_t *data);
};


#endif //JUNGFRAUJOCH_BACKGROUNDESTIMATION_H
