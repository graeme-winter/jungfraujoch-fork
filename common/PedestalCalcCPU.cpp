// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PedestalCalcCPU.h"

PedestalCalcCPU::PedestalCalcCPU(const DiffractionExperiment &experiment, int64_t in_lines) :
        PedestalCalc(experiment, in_lines) {

    v_currPedestal.resize(lines * RAW_MODULE_COLS, 0);
    v_currPedestalSquare.resize(lines * RAW_MODULE_COLS, 0);
    v_wrongCount.resize(lines * RAW_MODULE_COLS, 0);

    currPedestal = v_currPedestal.data();
    currPedestalSquare = v_currPedestalSquare.data();
    wrongCount = v_wrongCount.data();
}

template <unsigned int GAIN_BIT> void PedestalCalcCPU::AnalyzeImage(const uint16_t *raw_image) {
    if (image_number < window_size) {
        for (int i = 0; i < lines * RAW_MODULE_COLS; i++) {
            double adc = raw_image[i] & 0x3FFF;
            uint16_t gain = raw_image[i] & 0xc000;

            if (gain != GAIN_BIT)
                wrongCount[i] ++;
            currPedestal[i] += adc;
            currPedestalSquare[i] += adc * adc;
        }
    } else {
        for (int i = 0; i < lines * RAW_MODULE_COLS; i++) {
            double adc =  raw_image[i] & 0x3FFF;
            uint16_t gain = raw_image[i] & 0xc000;

            if (gain != GAIN_BIT)
                wrongCount[i]++;
            else {
                // Don't include wrong gains into moving average
                currPedestal[i] += adc - currPedestal[i] / window_size;
                currPedestalSquare[i] += adc * adc - currPedestalSquare[i] / window_size;
            }
        }
    }
}

void PedestalCalcCPU::AnalyzeImage(const uint16_t *raw_image) {
    switch (gain_level) {
        case 0:
            AnalyzeImage<0>(raw_image);
            break;
        case 1:
            AnalyzeImage<0x4000>(raw_image);
            break;
        case 2:
            AnalyzeImage<0xc000>(raw_image);
            break;
        default:
            break;
    }
    image_number++;
}

#ifndef CUDA_SPOT_FINDING
std::unique_ptr<PedestalCalc> MakePedestalCalc(const DiffractionExperiment& experiment, int64_t lines, int32_t thread_id) {
    return std::make_unique<PedestalCalcCPU>(experiment, lines);
}
#endif
