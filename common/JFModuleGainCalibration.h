// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFMODULEGAINCALIBRATION_H
#define JUNGFRAUJOCH_JFMODULEGAINCALIBRATION_H

#include <vector>
#include <cstdint>
#include <string>

class JFModuleGainCalibration {
    std::vector<double> gain;
public:
    JFModuleGainCalibration();
    explicit JFModuleGainCalibration(const std::string &filename);
    explicit JFModuleGainCalibration(const std::vector<double>& vec);
    [[nodiscard]] const std::vector<double> &GetGainCalibration() const;
};

// Only to use in automated tests run from the default directory
inline JFModuleGainCalibration GainCalibrationFromTestFile() {
    return JFModuleGainCalibration("../../tests/test_data/gainMaps_M049.bin");
}

#endif //JUNGFRAUJOCH_JFMODULEGAINCALIBRATION_H
