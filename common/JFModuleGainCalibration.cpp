// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFModuleGainCalibration.h"

#include <fstream>

#include "Definitions.h"
#include "JFJochException.h"

JFModuleGainCalibration::JFModuleGainCalibration() : gain(RAW_MODULE_SIZE * 3) {
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        gain[i + 0 * RAW_MODULE_SIZE] = DEFAULT_G0_FACTOR;
        gain[i + 1 * RAW_MODULE_SIZE] = DEFAULT_G1_FACTOR;
        gain[i + 2 * RAW_MODULE_SIZE] = DEFAULT_G2_FACTOR;
    }
}

JFModuleGainCalibration::JFModuleGainCalibration(const std::string &filename) : gain(RAW_MODULE_SIZE * 3) {
    std::ifstream file(filename.c_str(), std::fstream::binary);
    if (!file.is_open())
        throw JFJochException(JFJochExceptionCategory::GainFileOpenError, "Gain file cannot be opened");
    try {
        file.read((char *) gain.data(), gain.size() * sizeof(double));
    } catch (...) {
        throw JFJochException(JFJochExceptionCategory::GainFileOpenError, "Gain file cannot be read");
    }
}

JFModuleGainCalibration::JFModuleGainCalibration(const std::vector<double> &vec) {
    if (vec.size() != 3 * RAW_MODULE_SIZE) {
        throw JFJochException(JFJochExceptionCategory::GainFileOpenError,
                              "Wrong size of input vector for gain calibration");
    }
    gain = vec;
}

const std::vector<double> &JFModuleGainCalibration::GetGainCalibration() const {
    return gain;
}