// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFPEDESTAL_H
#define JUNGFRAUJOCH_JFPEDESTAL_H

#include <vector>
#include <cstdint>
#include <ctime>
#include <cmath>
#include <memory>
#include <jfjoch.pb.h>

#include "Definitions.h"

inline uint16_t to_fixed(double val, uint16_t fractional_bits) {
    // If val is result of division by zero, only reasonable value of output is zero (otherwise number could be interpreted improperly)
    uint32_t int_val = std::isfinite(val) ? (std::lround(val * (1<<fractional_bits))) : 0;
    // It is unlikely (but not impossible), that gain value will be lower than the smallest possible
    // Then reciprocal of gain could be more than allowed by data format. Protection is added for this condition
    if (int_val > UINT16_MAX) int_val = UINT16_MAX;
    return int_val;
}

struct JFPedestal {
    std::vector<uint16_t> pedestal;
    std::vector<int16_t> pedestal_rms;
    std::vector<uint8_t> pedestal_mask;

public:
    JFPedestal();

    JFPedestal(const JFJochProtoBuf::JFPedestal& pedestal); // deserialize
    [[nodiscard]] operator JFJochProtoBuf::JFPedestal() const; // serialize

    time_t collection_time = 0;
    int64_t frames = -1;

    [[nodiscard]] double Mean() const;
    [[nodiscard]] double MeanRMS() const;

    template<class T> void LoadPedestal(const std::vector<T> &vector) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            pedestal[i] = to_fixed(vector[i], 2);
            pedestal_rms[i] = -1;
        }
    }

    [[nodiscard]] uint16_t& operator[](size_t val);
    [[nodiscard]] const uint16_t& operator[](size_t val) const;
    [[nodiscard]] int16_t &RMS(size_t val);
    [[nodiscard]] const int16_t &RMS(size_t val) const;
    [[nodiscard]] uint8_t &Mask(size_t val);
    [[nodiscard]] const uint8_t &Mask(size_t val) const;
    [[nodiscard]] size_t CountMaskedPixels() const;

    std::vector<float> GetPedestalFP() const;

};


#endif //JUNGFRAUJOCH_JFPEDESTAL_H
