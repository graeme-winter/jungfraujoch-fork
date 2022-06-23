// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUCALIBRATION_H
#define JUNGFRAUCALIBRATION_H

#define PEDESTAL_RMS_MULTIPLIER (2)
#include <vector>
#include <cstdint>

#include "jfjoch.pb.h"
#include "DiffractionExperiment.h"

class JungfrauCalibration {
    uint16_t nmodules;
    uint16_t data_stream;

    std::vector<float> pedestalFP[3];
    std::vector<uint16_t> pedestal[3];
    std::vector<int16_t> pedestal_rms[3];
    std::vector<uint32_t> mask;

    std::vector<time_t> pedestal_time;
    std::vector<uint32_t> pedestal_frames;
public:
    explicit JungfrauCalibration(const DiffractionExperiment& experiment, uint16_t data_stream = TASK_NO_DATA_STREAM);
    explicit JungfrauCalibration(uint16_t nmodules, uint16_t data_stream = TASK_NO_DATA_STREAM);
    JungfrauCalibration(const JFJochProtoBuf::JFCalibration &calibration);
    void ImportFPGAInput(const DiffractionExperiment &experiment, const uint16_t *arr, uint16_t data_stream,
                         uint16_t module, bool import_mask);
    template<class T> void LoadModulePedestal(const std::vector<T> &vector, uint16_t module);
    template<class T> void LoadMask(const std::vector<T> &vector);

    const std::vector<uint16_t>& Pedestal(uint8_t gain_level) const;
    const std::vector<float>& PedestalFP(uint8_t gain_level) const;
    const std::vector<int16_t>& PedestalRMS(uint8_t gain_level) const;
    const std::vector<uint32_t>& Mask() const;

    std::vector<uint16_t>& Pedestal(uint8_t gain_level);
    std::vector<int16_t>& PedestalRMS(uint8_t gain_level);
    std::vector<uint32_t>& Mask();

    size_t GetDataStream() const;
    uint16_t GetModulesNum() const;

    void SetPedestalProperties(const DiffractionExperiment &experiment, time_t meas_time = 0);
    void SetPedestalProperties(uint8_t gain_level, uint32_t frames, time_t meas_time = 0);
    uint32_t GetPedestalTimestamp(uint8_t gain_level) const;
    size_t GetPedestalFrames(uint8_t gain_level) const;

    double MeanPedestal(uint8_t gain_level, uint16_t module) const;
    double MeanPedestalRMS(uint8_t gain_level, uint16_t module) const;

    size_t CountMaskedPixels(uint16_t module) const;
    size_t CountMaskedPixels() const;

    void GetMaskTransformed(const DiffractionExperiment& experiment, std::vector<uint32_t> &vector) const;
    std::vector<uint8_t> GetMaskOneByte(const DiffractionExperiment& experiment) const;
    void LoadMaskTransformed(const DiffractionExperiment& experiment, const std::vector<uint32_t> &vector, int32_t bit_val);

    operator JFJochProtoBuf::JFCalibration() const;
    void ImportPedestalAndMask(const JFJochProtoBuf::JFCalibration & calibration);

    double ComparePedestals(const JungfrauCalibration &c, uint8_t gain_level) const;
    double ComparePedestals(const JungfrauCalibration &c, uint8_t gain_level, uint16_t module) const;
};
#endif //JUNGFRAUCALIBRATION_H
