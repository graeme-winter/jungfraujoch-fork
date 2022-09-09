// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "RadialIntegration.h"
#include "JFJochException.h"

RadialIntegration::RadialIntegration(const std::vector<uint16_t>& in_mapping, uint16_t in_nbins) :
        pixel_to_bin(in_mapping), nbins(in_nbins), sum(in_nbins, 0), count(in_nbins, 0)
{}

RadialIntegration::RadialIntegration(const RadialIntegrationMapping &mapping) :
        RadialIntegration(mapping.GetPixelToBinMapping(), mapping.GetBinNumber())
{}

void RadialIntegration::Process(const int16_t *data, size_t npixel) {
    for (auto &i : sum)
        i = 0;

    for (auto &i : count)
        i = 0;

    if (npixel != pixel_to_bin.size())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Mismatch in size of pixel-to-bin mapping and image");

    for (int i = 0; i < npixel; i++) {
        auto bin = pixel_to_bin[i];
        auto value  = data[i];
        if ((value > INT16_MIN + 4) && (value < INT16_MAX - 4) && (bin < nbins)) {
            sum[bin]   += value;
            count[bin] += 1;
        }
    }
}

void RadialIntegration::GetResult(std::vector<float> &result) const {
    result.resize(nbins);

    for (int i = 0; i < nbins; i++) {
        if (count[i] > 0)
            result[i] = static_cast<float>(sum[i]) / static_cast<float>(count[i]);
        else
            result[i] = 0;
    }
}

const std::vector<int64_t> &RadialIntegration::GetSum() const {
    return sum;
}

const std::vector<int64_t> &RadialIntegration::GetCount() const {
    return count;
}

float RadialIntegration::GetRangeValue(uint16_t min_bin, uint16_t max_bin) {
    int64_t ret_sum = 0;
    int64_t ret_count = 0;

    for (int i = std::min(nbins,min_bin); i <= std::min((uint16_t)(nbins-1),max_bin); i++) {
        ret_sum += sum[i];
        ret_count += count[i];
    }
    if (ret_count == 0)
        return 0;
    else
        return static_cast<float>(ret_sum) / static_cast<float>(ret_count);
}