// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_RADIALINTEGRATION_H
#define JUNGFRAUJOCH_RADIALINTEGRATION_H

#include <vector>
#include <cstdint>
#include <cmath>

class RadialIntegration {
    std::vector<uint32_t> bin_map;
    std::vector<int64_t> sum;
    std::vector<int64_t> count;

    size_t xpixel, ypixel;
public:
    RadialIntegration(size_t xpixel, size_t ypixel);
    void Setup(double beam_x, double beam_y, const std::vector<uint32_t> &in_mask);
    void Clear();
    std::vector<double> GetAverage();

    template <class T>
    void Process(const T *data, T min, T max, size_t pixel0 = 0, size_t npixels = 0) {
        if (npixels == 0)
            npixels = xpixel * ypixel;
        for (int i = 0; i < npixels; i++) {
            auto bin = bin_map[i + pixel0];
            auto value          = data[i];
            if ((value > min) && (value < max) && (bin < sum.size())) {
                sum[bin]   += value;
                count[bin] += 1;
            }
        }
    }
};


#endif //JUNGFRAUJOCH_RADIALINTEGRATION_H
