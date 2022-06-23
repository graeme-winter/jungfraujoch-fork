// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_PEDESTALCALCGPU_H
#define JUNGFRAUJOCH_PEDESTALCALCGPU_H

#include "PedestalCalc.h"

struct CudaStreamWrapper;

class PedestalCalcGPU : public PedestalCalc {
    uint16_t *gpu_image = nullptr;
    CudaStreamWrapper *cudastream;
    static std::atomic<uint16_t> threadid;
    int numberOfSMs;
public:
    PedestalCalcGPU(const DiffractionExperiment & experiment, int64_t lines = -1, int32_t gpu_device = -1);
    ~PedestalCalcGPU() override;
    void AnalyzeImage(const uint16_t *raw_image) override;
    static bool GPUPresent();
};

#endif //JUNGFRAUJOCH_PEDESTALCALCGPU_H
