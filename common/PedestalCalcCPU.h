// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_PEDESTALCALCCPU_H
#define JUNGFRAUJOCH_PEDESTALCALCCPU_H

#include "PedestalCalc.h"

class PedestalCalcCPU : public PedestalCalc {
    template <unsigned int GAIN_BIT> void AnalyzeImage(const uint16_t *raw_image);
    std::vector<double> v_currPedestal;
    std::vector<double> v_currPedestalSquare;
    std::vector<uint32_t> v_wrongCount;
public:
    PedestalCalcCPU(const DiffractionExperiment & experiment, int64_t lines = -1);
    ~PedestalCalcCPU() = default;
    void AnalyzeImage(const uint16_t *raw_image);
};

#endif //JUNGFRAUJOCH_PEDESTALCALCCPU_H
