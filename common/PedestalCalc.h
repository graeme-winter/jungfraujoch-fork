// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_PEDESTALCALC_H
#define JUNGFRAUJOCH_PEDESTALCALC_H

#include <cmath>
#include "DiffractionExperiment.h"
#include "JFPedestal.h"

#define PEDESTAL_RMS_MULTIPLIER (2)

class PedestalCalc {
protected:
    double *currPedestal = nullptr;
    double *currPedestalSquare = nullptr;
    uint32_t *wrongCount = nullptr;
    size_t lines;
    uint8_t gain_level;
    uint32_t image_number = 0;
    constexpr static const uint32_t window_size = PEDESTAL_WINDOW_SIZE;
public:
    PedestalCalc(const DiffractionExperiment & experiment, int64_t lines = -1);
    virtual ~PedestalCalc() {};
    virtual void AnalyzeImage(const uint16_t *raw_image) = 0;

    void Export(std::vector<uint32_t> &mean, std::vector<uint32_t> &rms,
                std::vector<uint32_t> &wrong_count, size_t offset = 0);

    void Export(JFPedestal& calibration, size_t allowed_wrong_gains = 0, double rms_mask = INFINITY);
};

std::unique_ptr<PedestalCalc> MakePedestalCalc(const DiffractionExperiment& experiment, int64_t lines = -1, int32_t thread_id = -1);

#endif //JUNGFRAUJOCH_PEDESTALCALC_H
