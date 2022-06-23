// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BackgroundEstimation.h"
#include <cmath>

BackgroundEstimation::BackgroundEstimation(const DiffractionExperiment &experiment) :
        mask(experiment.GetPixelsNum()), npixel(experiment.GetPixelsNum())
{
    double min_pixel = experiment.GetLowResolutionLimitForBkg_Pixel();
    double max_pixel = experiment.GetHighResolutionLimitForBkg_Pixel();

    if (min_pixel >= max_pixel)
        enable = false;
    else
        enable = true;

    double min_pixel2 = min_pixel * min_pixel;
    double max_pixel2 = max_pixel * max_pixel;

    for (int i = 0; i < experiment.GetPixelsNum(); i++) {
        double x = (i % experiment.GetXPixelsNum()) - experiment.GetBeamX_pxl();
        double y = (i / experiment.GetXPixelsNum()) - experiment.GetBeamY_pxl();
        double dist2 = (x * x) + (y * y);
        if ((dist2 > min_pixel2) && (dist2 < max_pixel2))
            mask[i] = 1;
    }
}

std::pair<float, float> BackgroundEstimation::Process(const int16_t *data) {
    if (!enable)
        return {0, 0};

    int64_t sum = 0;
    int64_t sum2 = 0;

    int64_t count = 0;
    for (int i = 0; i < npixel; i++) {
        if ((data[i] > INT16_MIN + 4) && (data[i] < INT16_MAX - 4)) {
            sum += data[i];
            sum2 += data[i] * data[i];
            count++;
        }
    }
    int64_t tmp = sum2 * count - sum * sum;
    if (count == 0)
        return {0, 0};
    else
        return {static_cast<double>(sum) / static_cast<double>(count),
                sqrt(static_cast<double>(tmp) / static_cast<double>(count * (count -1)))};
}