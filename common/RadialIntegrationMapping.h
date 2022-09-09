// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_RADIALINTEGRATIONMAPPING_H
#define JUNGFRAUJOCH_RADIALINTEGRATIONMAPPING_H

#include <optional>
#include "DiffractionExperiment.h"

class RadialIntegrationMapping {
    const double low_q, high_q, q_spacing;
    std::vector<float> bin_to_q;
    std::vector<uint16_t> pixel_to_bin;
    uint16_t max_bin_number;
public:
    RadialIntegrationMapping(const DiffractionExperiment& experiment, const uint8_t *one_byte_mask = nullptr);
    [[nodiscard]] uint16_t GetBinNumber() const;
    [[nodiscard]] const std::vector<uint16_t> &GetPixelToBinMapping() const;
    [[nodiscard]] const std::vector<float> &GetBinToQ() const;
    [[nodiscard]] double QToBin(double q) const;
};


#endif //JUNGFRAUJOCH_RADIALINTEGRATIONMAPPING_H
