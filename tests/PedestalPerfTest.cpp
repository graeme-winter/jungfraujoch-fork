// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <random>
#include <iostream>
#include <iomanip>
#include "../common/PedestalCalcCPU.h"
#include "../common/PedestalCalcGPU.h"

int main () {
    size_t nframes = 2000;
    DiffractionExperiment x(1,{1});

    std::vector<uint16_t> data(nframes * RAW_MODULE_SIZE);

    x.Mode(DetectorMode::Conversion);
    double mean = 1000.0;
    double stddev = 50.0;

    // Predictable random number generator
    std::mt19937 g1(5423);
    std::normal_distribution<double> distribution(mean, stddev);

    for (int i = 0; i < nframes * RAW_MODULE_SIZE; i++) {
        double number = distribution(g1);
        if (number < 20) number = 20;
        if (number > 16300) number = 16300;
        data[i] = number;
    }

    PedestalCalcCPU calc_cpu(x);
    auto start_time = std::chrono::system_clock::now();
    for (int i = 0; i < nframes; i++)
        calc_cpu.AnalyzeImage(data.data() + i * RAW_MODULE_SIZE);

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "CPU Pedestal performance: " << std::setw(5) <<  std::lround(elapsed.count() / ((double) nframes)) <<  " us/image "
              << std::setw(5) << nframes * RAW_MODULE_SIZE * sizeof(uint16_t) * 1000 * 1000/ ((double) elapsed.count() * 1024 * 1024 * 1024) << " GB/s" << std::endl;

#ifdef CUDA_SPOT_FINDING
    PedestalCalcGPU calc_gpu(x);
    start_time = std::chrono::system_clock::now();
    for (int i = 0; i < nframes; i++)
        calc_gpu.AnalyzeImage(data.data() + i * RAW_MODULE_SIZE);

    end_time = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "GPU Pedestal performance: " << std::setw(5) <<  std::lround(elapsed.count() / ((double) nframes)) <<  " us/image "
              << std::setw(5) << nframes * RAW_MODULE_SIZE * sizeof(uint16_t) * 1000 * 1000/ ((double) elapsed.count() * 1024 * 1024 * 1024) << " GB/s" << std::endl;
#endif
}