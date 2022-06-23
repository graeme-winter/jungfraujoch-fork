// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include <random>
#include "../common/SpotFinder.h"
#include "FPGAUnitTest.h"

using namespace std::literals::chrono_literals;

void naive_spot_finder(StrongPixelSet& set, const int16_t *buffer, const spot_parameters &parameters) {
    for (int col = 0; col < parameters.cols; col++ ) {
        for (int line = 0; line < parameters.lines; line++ ) {
            double sum = 0.0;
            double sum2 = 0.0;
            double count = 0.0;

            for (int x = col - parameters.nbx; x <= col + parameters.nbx; x++) {
                for (int y = line - parameters.nby; y <= line + parameters.nby; y++) {
                    // Only calculate pixels within edges of the detector
                    if (((x >= 0) && (x < parameters.cols)) && ((y >= 0) && (y < parameters.lines))) {
                        // Don't count the actual point into mean and variance calculation
                        if ((x != col) || (y != line)) {
                            // values below some minimum are considered bad pixel and should not be included
                            if (buffer[x + y * parameters.cols] >= parameters.min_viable_number) {
                                sum += buffer[x + y * parameters.cols];
                                sum2 += buffer[x + y * parameters.cols] * buffer[x + y * parameters.cols];
                                count += 1;
                            }
                        }
                    }
                }
            }

            double mean = sum / count;
            double mean2 = sum2 / count;

            double val_minus_mean = buffer[col+line*parameters.cols] - mean;
            double variance = (mean2 - mean * mean) * count / (count - 1);

            if ((val_minus_mean > 0)
            && (val_minus_mean >  sqrt(parameters.strong_pixel_threshold2 * variance))
            && (buffer[col+line*parameters.cols] > parameters.count_threshold))
                set.AddStrongPixel(col, line,  0, buffer[col+line*parameters.cols]);
        }
    }
}

#ifdef CUDA_SPOT_FINDING
TEST_CASE("SpotFinder_GPU", "[SpotFinder]") {

    const uint16_t nbx = 5;
    const uint16_t nby = 5;

    const double mean = 30;

    std::vector<int16_t> image(512*1024);

    // Predictable random number generator, so test always gives the same result
    std::mt19937 g1(2023);
    // Poissonian distribution, with mean == variance
    std::normal_distribution<double> distribution(mean, sqrt(mean));

    for (auto &i: image) {
        i = static_cast<int16_t>(distribution(g1));
    }

    image[8000] = static_cast<int16_t>(mean + 3.5 * sqrt(mean));
    image[14000] = static_cast<int16_t>(mean + 5 * sqrt(mean));
    image[20000] = static_cast<int16_t>(mean + 6 * sqrt(mean));
    image[25000] = static_cast<int16_t>(mean + 8 * sqrt(mean));
    image[60000] = static_cast<int16_t>(mean + 15 * sqrt(mean));
    std::vector<double> threshold_vals = {3,4,6,10};

    SpotFinder spot_finder(1024, 512);

    for (const auto& threshold: threshold_vals) {
        std::cout << "I/sigma threshold value: " << threshold << std::endl;

        // Run COLSPOT (CPU version)
        spot_parameters spot_params;
        spot_params.strong_pixel_threshold2 = threshold * threshold;
        spot_params.count_threshold = 1;
        spot_params.nbx = nbx;
        spot_params.nby = nby;
        spot_params.lines = 512;
        spot_params.cols = 1024;
        spot_params.min_viable_number = 1;

        JFJochProtoBuf::DataProcessingSettings settings;
        settings.set_local_bkg_size(nbx);
        settings.set_photon_count_threshold(1);
        settings.set_signal_to_noise_threshold(threshold);

        StrongPixelSet colspot_gpu;
        memcpy(spot_finder.GetInputBuffer(), image.data(), 512*1024*sizeof(uint16_t));
        spot_finder.RunSpotFinder(settings);
        spot_finder.GetResults(colspot_gpu, 0);

        StrongPixelSet naive_cpu;
        naive_spot_finder(naive_cpu, image.data(), spot_params);

        REQUIRE(naive_cpu.Common(colspot_gpu) >= 0.90 * naive_cpu.Count());
        REQUIRE(naive_cpu.Common(colspot_gpu) >= 0.90 * colspot_gpu.Count());

        std::cout << "Common: " << naive_cpu.Common(colspot_gpu) << " fast impl: " << colspot_gpu.Count() << " ref. impl: " << naive_cpu.Count() << std::endl;
    }
}
#endif