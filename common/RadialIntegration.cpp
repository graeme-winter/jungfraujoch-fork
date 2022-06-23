// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "RadialIntegration.h"

void RadialIntegration::Clear() {
    for (auto &i : sum)
        i = 0;
    for (auto &i: count)
        i = 0;
}

RadialIntegration::RadialIntegration(size_t in_xpixel, size_t in_ypixel) :
bin_map(in_xpixel * in_ypixel, UINT32_MAX) {
    xpixel = in_xpixel;
    ypixel = in_ypixel;
}

void RadialIntegration::Setup(double beam_x, double beam_y, const std::vector<uint32_t> &in_mask) {
    size_t rmax = 0;
    for (int y = 0; y < ypixel; y++) {
        for (int x = 0; x < xpixel; x++) {
            uint16_t val =  std::lround(sqrt((x-beam_x)*(x-beam_x) + (y-beam_y)*(y-beam_y)));

            if (in_mask[x + y * xpixel] != 0)
                bin_map[x + y * xpixel] = UINT32_MAX;
            else
                bin_map[x + y * xpixel] = val;

            if (val > rmax) rmax = val;
        }
    }
    sum = std::vector<int64_t>(rmax + 2, 0);
    count = std::vector<int64_t>(rmax + 2, 0);
}

std::vector<double> RadialIntegration::GetAverage() {
    std::vector<double> ret(sum.size(), 0);
    for (int i = 0; i < sum.size(); i++) {
        ret[i] = static_cast<double>(sum[i]) / static_cast<double>(count[i]);
    }
    return ret;
}