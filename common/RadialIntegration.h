// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_RADIALINTEGRATION_H
#define JUNGFRAUJOCH_RADIALINTEGRATION_H

#include <vector>
#include <cstdint>
#include <cmath>

#include "DiffractionExperiment.h"
#include "RadialIntegrationMapping.h"

class RadialIntegration {
    const std::vector<uint16_t>& pixel_to_bin;
    const uint16_t nbins;
    std::vector<int64_t> sum;
    std::vector<int64_t> count;
public:
    RadialIntegration(const RadialIntegrationMapping& mapping);
    RadialIntegration(const std::vector<uint16_t>& mapping, uint16_t nbins);
    void Process(const int16_t *data, size_t npixel);
    void GetResult(std::vector<float> &result) const;
    [[nodiscard]] float GetRangeValue(uint16_t min_bin, uint16_t max_bin);
    [[nodiscard]] const std::vector<int64_t>& GetSum() const;
    [[nodiscard]] const std::vector<int64_t>& GetCount() const;
};


#endif //JUNGFRAUJOCH_RADIALINTEGRATION_H
