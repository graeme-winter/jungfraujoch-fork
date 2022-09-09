// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFCALIBRATION_H
#define JUNGFRAUJOCH_JFCALIBRATION_H

#include "JFPedestal.h"
#include "DiffractionExperiment.h"
#include "RawToConvertedGeometry.h"
#include "JFJochException.h"

class JFCalibration {
    const size_t nmodules;
    const size_t nstorage_cells;

    std::vector<JFPedestal> pedestal;
    std::vector<uint32_t> mask; // pedestal independent mask
    void GetModuleStatistics(JFJochProtoBuf::JFCalibrationStatistics &statistics, size_t storage_cell) const;
public:
    explicit JFCalibration(size_t nmodules, size_t nstorage_cells = 1);
    explicit JFCalibration(const DiffractionExperiment& experiment);
    JFCalibration(const JFJochProtoBuf::JFCalibration &input);

    [[nodiscard]] operator JFJochProtoBuf::JFCalibration() const;
    [[nodiscard]] operator JFJochProtoBuf::JFCalibrationStatistics() const;

    [[nodiscard]] size_t GetModulesNum() const;
    [[nodiscard]] size_t GetStorageCellNum() const;

    [[nodiscard]] uint32_t &Mask(size_t id);
    [[nodiscard]] const uint32_t &Mask(size_t id) const;

    [[nodiscard]] JFPedestal& Pedestal(size_t module, size_t gain_level, size_t storage_cell = 0);
    [[nodiscard]] const JFPedestal& Pedestal(size_t module, size_t gain_level, size_t storage_cell = 0) const;

    [[nodiscard]] std::vector<uint32_t> CalculateMask(const DiffractionExperiment& experiment, size_t storage_cell = 0) const;
    [[nodiscard]] std::vector<uint8_t> CalculateOneByteMask(const DiffractionExperiment& experiment, size_t storage_cell = 0) const;
    [[nodiscard]] std::vector<uint32_t> CalculateNexusMask(const DiffractionExperiment& experiment, size_t storage_cell = 0) const;

    [[nodiscard]] int64_t CountMaskedPixels(size_t module, size_t storage_cell = 0) const;

    [[nodiscard]] JFJochProtoBuf::JFCalibrationStatistics GetModuleStatistics(size_t storage_cell) const;
    [[nodiscard]] JFJochProtoBuf::JFCalibrationStatistics GetModuleStatistics() const;

    [[nodiscard]] std::vector<float> GetPedestal(size_t gain_level, size_t storage_cell = 0) const;
    [[nodiscard]] std::vector<int16_t> GetPedestalRMS(size_t gain_level, size_t storage_cell = 0) const;

    template <class T> void LoadMask(const DiffractionExperiment &experiment, const std::vector<T> &conv_mask,
                                     int32_t bit_val = -1) {
        if (experiment.GetModulesNum() != nmodules)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                                  "Mismatch in module size");

        if (experiment.GetDetectorMode() != DetectorMode::Conversion)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                                  "The operation makes sense only in conversion mode");

        std::vector<T> raw_mask(nmodules * RAW_MODULE_SIZE);
        ConvertedToRawGeometry(experiment, raw_mask.data(), conv_mask.data());

        if (bit_val >= 0) {
            for (int i = 0; i < nmodules * RAW_MODULE_SIZE; i++) {
                if (raw_mask[i] != 0)
                    mask[i] |= 1 << bit_val;
                else
                    mask[i] &= ~(1 << bit_val);
            }
        } else {
            for (int i = 0; i < nmodules * RAW_MODULE_SIZE; i++)
                mask[i] = raw_mask[i];
        }
    }
};


#endif //JUNGFRAUJOCH_JFCALIBRATION_H
