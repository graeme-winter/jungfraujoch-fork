// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PedestalCalc.h"
#include "JFJochException.h"

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

void PedestalCalc::Export(JFPedestal &calibration, size_t allowed_wrong_gains, double rms_mask) {
    if (lines != RAW_MODULE_LINES)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Exporting to JFCalibration works only if pedestal is calculated for a full module");

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        double tmp = double(currPedestal[i]) / window_size;
        double variance = double(currPedestalSquare[i]) / window_size - tmp * tmp;

        if (sqrt(variance) > rms_mask)
            calibration.Mask(i) |= 1<<(gain_level+4);
        else
            calibration.Mask(i) &= ~(1<<(gain_level+4));

        // Minimally, full window size needs to be recorded to give viable pedestal
        if ((image_number < window_size) || (wrongCount[i] > allowed_wrong_gains)) {
            calibration.Mask(i) |= 1U << (gain_level + 1);
            calibration[i] = to_fixed(16383.5, 2);
            calibration.RMS(i) = -1;
        } else {
            calibration.Mask(i) &= ~(1U << (gain_level + 1));
            calibration[i] = to_fixed(tmp, 2);
            double rms = sqrt(variance) * PEDESTAL_RMS_MULTIPLIER;
            calibration.RMS(i) = (rms < INT16_MAX) ? static_cast<int16_t>(rms) : (int16_t) INT16_MAX;
        }
    }
}