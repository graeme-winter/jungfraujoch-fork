// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PedestalCalc.h"

PedestalCalc::PedestalCalc(const DiffractionExperiment & experiment, int64_t in_lines) {
    if (in_lines < 0)
        lines = experiment.GetModulesNum() * RAW_MODULE_LINES;
    else
        lines = in_lines;

    switch (experiment.GetDetectorMode()) {
        case DetectorMode::PedestalG1:
            gain_level = 1;
            break;
        case DetectorMode::PedestalG2:
            gain_level = 2;
            break;
        default:
            gain_level = 0;
            break;
    }
}

void PedestalCalc::Export(std::vector<uint32_t> &mean, std::vector<uint32_t> &rmsd,
                          std::vector<uint32_t> &wrong_count, size_t offset) {
    for (int i = 0; i < lines * RAW_MODULE_COLS; i++) {
        double tmp = double(currPedestal[i]) / window_size;
        mean[offset + i] = tmp;
        double variance = double(currPedestalSquare[i]) / window_size - tmp * tmp;
        rmsd[offset + i] = sqrt(variance);
        wrong_count[offset + i] = wrongCount[i];
    }
}

inline uint16_t to_fixed(double val, uint16_t fractional_bits) {
    // If val is result of division by zero, only reasonable value of output is zero (otherwise number could be interpreted improperly)
    uint32_t int_val = std::isfinite(val) ? (std::lround(val * (1<<fractional_bits))) : 0;
    // It is unlikely (but not impossible), that gain value will be lower than the smallest possible
    // Then reciprocal of gain could be more than allowed by data format. Protection is added for this condition
    if (int_val > UINT16_MAX) int_val = UINT16_MAX;
    return int_val;
}

void PedestalCalc::Export(JungfrauCalibration &calibration, size_t offset, size_t allowed_wrong_gains, double rms_mask) {
    for (int i = 0; i < lines * RAW_MODULE_COLS; i++) {
        double tmp = double(currPedestal[i]) / window_size;
        double variance = double(currPedestalSquare[i]) / window_size - tmp * tmp;

        if (sqrt(variance) > rms_mask)
            calibration.Mask()[offset+i] |= 1<<(gain_level+4);
        else
            calibration.Mask()[offset+i] &= ~(1<<(gain_level+4));

        // Minimally, full window size needs to be recorded to give viable pedestal
        if ((image_number < window_size) || (wrongCount[i] > allowed_wrong_gains)) {
            calibration.Mask()[offset + i] |= 1U << (gain_level + 1);
            calibration.Pedestal(gain_level)[offset + i] = to_fixed(16383.5, 2);
            calibration.PedestalRMS(gain_level)[offset + i] = -1;
        } else {
            calibration.Mask()[offset + i] &= ~(1U << (gain_level + 1));
            calibration.Pedestal(gain_level)[offset + i] = to_fixed(tmp, 2);
            double rms = sqrt(variance) * PEDESTAL_RMS_MULTIPLIER;
            calibration.PedestalRMS(gain_level)[offset + i] = (rms > INT16_MAX) ? INT16_MAX : rms;
        }
    }
}