// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPGAUNITTEST_H
#define FPGAUNITTEST_H

#include <iostream>
#include <fstream>
#include "../common/DiffractionExperiment.h"
#include "../common/RawToConvertedGeometry.h"
#include "../common/JFJochException.h"
#include "../common/JFCalibration.h"
#include "../common/JFModuleGainCalibration.h"

template <class T> void LoadBinaryFile(std::string filename, T* output, size_t size) {
    std::fstream file(filename.c_str(), std::fstream::in | std::fstream::binary);
    if (file.is_open())
        file.read((char *) output, size * sizeof(T));
    else
        throw JFJochException(JFJochExceptionCategory::GainFileOpenError, "Gain file cannot be opened");
}

inline void CalcConvertedRef(const DiffractionExperiment &experiment, const JFCalibration &calib,
                             const JFModuleGainCalibration &gain_calibration,
                             const uint16_t *raw, double *converted, uint8_t *gain_mask,
                             size_t pixels = 0, size_t storage_cell = 0) {

    double one_over_gain;
    double energy = experiment.GetPhotonEnergy_keV();

    auto &gain_arr = gain_calibration.GetGainCalibration();
    auto pedestal_G0 = calib.GetPedestalFP(0, storage_cell);
    auto pedestal_G1 = calib.GetPedestalFP(1, storage_cell);
    auto pedestal_G2 = calib.GetPedestalFP(2, storage_cell);

    if (pixels == 0) pixels = experiment.GetModulesNum() * RAW_MODULE_SIZE;

    for (size_t i = 0; i < pixels; i++) {
        uint16_t gainbits = raw[i] & 0xc000;
        uint16_t adc = raw[i] & 0x3fff;
        float adcfloat = adc;

        double expected;
        double gain;

        bool special_val = false;
        switch (gainbits) {
            case 0:
                gain = gain_arr[i % RAW_MODULE_SIZE];
                one_over_gain = ((gain * energy != 0.0) ? 1.0 / (gain * energy) : 0.0);
                expected = (adcfloat - pedestal_G0[i]) * one_over_gain;
                gain_mask[i] = 0;
                break;
            case 0x4000:
                gain = gain_arr[i % RAW_MODULE_SIZE + RAW_MODULE_SIZE];
                one_over_gain = ((gain * energy != 0.0) ? 1.0 / (gain * energy) : 0.0);
                expected = (adcfloat - pedestal_G1[i]) * one_over_gain;
                gain_mask[i] = 1;
                if (adc == 0) expected = PIXEL_OUT_G1_SATURATION, special_val = true;
                break;
            case 0xc000:
                gain = gain_arr[i % RAW_MODULE_SIZE + 2 * RAW_MODULE_SIZE];
                one_over_gain = ((gain * energy != 0.0) ? 1.0 / (gain * energy) : 0.0);
                expected = (adcfloat - pedestal_G2[i]) * one_over_gain;
                gain_mask[i] = 3;
                if (adc == 0)           expected = PIXEL_OUT_SATURATION, special_val = true;
                else if (adc == 0x3fff) expected = PIXEL_OUT_0xFFFF, special_val = true;
                break;
            default:
                gain_mask[i] = 2;
                expected = PIXEL_OUT_GAINBIT_2, special_val = true;;
                break;
        }

        if ((expected > PIXEL_OUT_SATURATION) && !special_val)
            expected = PIXEL_OUT_SATURATION;
        converted[i] = expected;
    }
}

template <class T> double CheckConversion(const DiffractionExperiment &experiment,
                                          const JFCalibration &calib,
                                          const JFModuleGainCalibration &gain,
                                          const uint16_t *raw, T *converted, size_t pixels,
                                          size_t storage_cell = 0) {

    double result = 0;
    int32_t count = 0;

    std::vector<double> conversion_ref(pixels);
    std::vector<uint8_t> gain_mask_ref(pixels);

    CalcConvertedRef(experiment, calib, gain, raw, conversion_ref.data(), gain_mask_ref.data(),
                     pixels, storage_cell);

    for (size_t i = 0; i < pixels; i++) {
        double diff = conversion_ref[i] - converted[i];
        result += diff * diff;
        count++;
    }

    return sqrt(result / count);
}

template <class T> double CheckConversionWithGeomTransform(const DiffractionExperiment &experiment,
                                                           const JFCalibration &calib,
                                                           const JFModuleGainCalibration &gain,
                                                           const uint16_t *raw, T *converted,
                                                           size_t storage_cell = 0) {

    double result = 0;
    int32_t count = 0;

    std::vector<double> conversion_ref(experiment.GetModulesNum() * RAW_MODULE_SIZE);
    std::vector<uint8_t> gain_mask_ref(experiment.GetModulesNum() * RAW_MODULE_SIZE);

    CalcConvertedRef(experiment, calib, gain, raw, conversion_ref.data(), gain_mask_ref.data(),
                     experiment.GetModulesNum() * RAW_MODULE_SIZE, storage_cell);

    std::vector<double> conversion_ref_transformed(experiment.GetPixelsNum());
    std::vector<uint8_t> gain_mask_ref_transformed(experiment.GetPixelsNum());

    RawToConvertedGeometryAdjustMultipixels(experiment, conversion_ref_transformed.data(), conversion_ref.data() );
    RawToConvertedGeometry(experiment, gain_mask_ref_transformed.data(), gain_mask_ref.data());

    for (size_t i = 0; i < experiment.GetPixelsNum(); i++) {
        double diff = conversion_ref_transformed[i] - converted[i];
        result += diff * diff;
        count++;
    }

    return sqrt(result / count);
}

#endif //FPGAUNITTEST_H
