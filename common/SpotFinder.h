// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_SPOTFINDER_H
#define JUNGFRAUJOCH_SPOTFINDER_H

#include <vector>

#include "StrongPixelSet.h"

struct spot_parameters {
    float strong_pixel_threshold2;
    uint32_t count_threshold;
    uint32_t nbx;
    uint32_t nby;
    uint32_t lines;
    uint32_t cols;
    int16_t min_viable_number;
};

struct CudaStreamWrapper;

class SpotFinder {
    std::mutex m;
    CudaStreamWrapper *cudastream;

    static std::atomic<uint16_t> threadid;

    const int32_t xpixels;
    const int32_t ypixels;

    int16_t *host_in = nullptr;
    uint8_t *gpu_mask = nullptr;
    int16_t *gpu_in;
    uint32_t *gpu_out, *host_out;
    int numberOfSMs;
    const int numberOfCudaThreads = 128;    // #threads per block that works well for Nvidia T4
    const int numberOfWaves = 40;           // #waves that works well for Nvidia T4
    const int windowSizeLimit = 21;         // limit on the window size (2nby+1, 2nbx+1) to prevent shared memory problems
public:

    SpotFinder(int32_t xpixels, int32_t ypixels, int32_t gpu_device = -1);
    explicit SpotFinder(const DiffractionExperiment& experiment, int32_t gpu_device = -1);
    ~SpotFinder();

    void SetInputBuffer(void *host_in);

    void RunSpotFinder(const JFJochProtoBuf::DataProcessingSettings &settings);
    void GetResults(StrongPixelSet &pixel_set, int64_t image_number);
    void GetResults(const DiffractionExperiment &experiment, const JFJochProtoBuf::DataProcessingSettings &settings,
                    std::vector<DiffractionSpot> &vec, int64_t image_number);
    static bool GPUPresent();
    void LoadMask(const std::vector<uint8_t> &mask);
};

#endif //JUNGFRAUJOCH_SPOTFINDER_H
