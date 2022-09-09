// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>

#include "RadialIntegrationMapping.h"
#include "JFJochException.h"

RadialIntegrationMapping::RadialIntegrationMapping(const DiffractionExperiment& experiment, const uint8_t *one_byte_mask) :
        low_q(experiment.GetLowQForRadialInt_recipA()),
        high_q(experiment.GetHighQForRadialInt_recipA()),
        q_spacing(experiment.GetQSpacingForRadialInt_recipA()),
        pixel_to_bin(experiment.GetPixelsNum()),
        max_bin_number(0)
{

    if (q_spacing < 0.0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Q-spacing has to be >= 0.0");

    if (low_q >= high_q)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Low-Q must be smaller than high-Q");

    if (low_q <= 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Low-Q must be > 0");

    if (std::ceil((high_q-low_q) / q_spacing ) >= UINT16_MAX)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Cannot accommodate more than 65536 rings");

    for (int y = 0; y < experiment.GetYPixelsNum(); y++) {
        for (int x = 0; x < experiment.GetXPixelsNum(); x++) {
            int64_t pixel_number = y * experiment.GetXPixelsNum() + x;

            double pixel_q = 2 * M_PI / experiment.PxlToRes(x, y);
            if (((one_byte_mask != nullptr) && (one_byte_mask[pixel_number] == 0))
                || (pixel_q < low_q)
                || (pixel_q >= high_q))
                pixel_to_bin[pixel_number] = UINT16_MAX;
            else
                pixel_to_bin[pixel_number] = std::floor((pixel_q - low_q) / q_spacing);
        }
    }

    // In principle max bin number is equal to std::floor((high_q - low_q) / q_spacing), but it might be lower than this
    // depending on detector distance and beam center position
    for (const auto &i: pixel_to_bin) {
        if ((i != UINT16_MAX) && (i > max_bin_number))
            max_bin_number = i;
    }

    bin_to_q.resize(max_bin_number + 1);
    for (int i = 0; i < max_bin_number + 1; i++)
        bin_to_q[i] = static_cast<float>(q_spacing * (i + 0.5) + low_q);
}

uint16_t RadialIntegrationMapping::GetBinNumber() const {
    return max_bin_number + 1;
}

const std::vector<uint16_t> &RadialIntegrationMapping::GetPixelToBinMapping() const {
    return pixel_to_bin;
}

const std::vector<float> &RadialIntegrationMapping::GetBinToQ() const {
    return bin_to_q;
}

double RadialIntegrationMapping::QToBin(double q) const {
    return std::min(static_cast<double>(max_bin_number), std::max(0.0, (q - low_q) / q_spacing));
}