// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/PedestalCalcCPU.h"
#include "../common/PedestalCalcGPU.h"

TEST_CASE("PedestalCalc", "[PedestalCalc]") {
    for (int gain_level = 0; gain_level < 3; gain_level++) {
        uint16_t base_value;
        uint16_t wrong_value;
        uint16_t mask_value;
        DiffractionExperiment x;
        x.DataStreamModuleSize(1, {1});
        switch (gain_level) {
            case 1:
                base_value = 0x4000;
                wrong_value = 0xc000;
                mask_value = 4;
                x.Mode(DetectorMode::PedestalG1);
                break;
            case 2:
                base_value = 0xc000;
                wrong_value = 0;
                mask_value = 8;
                x.Mode(DetectorMode::PedestalG2);
                break;
            default:
                base_value = 0;
                wrong_value = 0x4000;
                mask_value = 2;
                x.Mode(DetectorMode::Conversion);
                break;
        }

        std::vector<uint16_t> image(RAW_MODULE_SIZE, base_value);

        for (int i = 0; i < RAW_MODULE_SIZE; i++)
            image[i] += i;

        image[2] = wrong_value;

        PedestalCalcCPU calc(x);

        // Predictable random number generator
        std::mt19937 g1(0);
        std::normal_distribution<double> distribution(12000.0, 12.5);

        for (int i = 0; i < 299; i++) {
            image[3] = base_value + distribution(g1);
            calc.AnalyzeImage(image.data());
        }
        image[3] = base_value + distribution(g1);

        image[0] = wrong_value;
        image[1] = wrong_value;
        image[2] = base_value;

        calc.AnalyzeImage(image.data());

        JFPedestal calib;
        // Previously these bits were part of the mask - checking if they are cleared properly
        calib.Mask(3) = mask_value;
        calib.Mask(1) = 8*mask_value;

        calc.Export(calib, 1, INFINITY);

        for (int i = 4; i < RAW_MODULE_COLS; i++)
            REQUIRE(calib[i] == i * 4);

        REQUIRE(calib[3] > 11900.0 * 4);
        REQUIRE(calib[3] < 12100.0 * 4);

        // Should be 12.5, but obviously with large error bar
        REQUIRE(calib.RMS(3) >= 12 * PEDESTAL_RMS_MULTIPLIER);
        REQUIRE(calib.RMS(3) <= 13 * PEDESTAL_RMS_MULTIPLIER);

        REQUIRE(calib[0] == 0);

        REQUIRE(calib.Mask(0) == 0);
        REQUIRE(calib.Mask(1) == 0);
        REQUIRE(calib.Mask(2) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(3) == 0);

        calc.Export(calib, 0, 10.0);
        for (int i = 4; i < RAW_MODULE_COLS; i++) {
            REQUIRE(calib[i] == i * 4);
        }

        REQUIRE(calib.Mask(3) == mask_value * 8); // RMS condition
        REQUIRE(calib.Mask(2) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(1) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(0) == mask_value); // Wrong gain
    }
}

TEST_CASE("PedestalCalc_ImagesLessThanWindow", "[PedestalCalc]") {
    DiffractionExperiment x;
    x.DataStreamModuleSize(1, {1}).Mode(DetectorMode::PedestalG2);
    JFPedestal calib;
    PedestalCalcCPU calc(x, RAW_MODULE_LINES);

    // No images at all
    calc.Export(calib);
    REQUIRE(calib[0] == 16383.5 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 1 << 3);

    std::vector<uint16_t> image(RAW_MODULE_SIZE, 0xc000 + 12000);
    for (int i = 0; i < PEDESTAL_WINDOW_SIZE - 1; i++)
        calc.AnalyzeImage(image.data());

    // 127 images
    calc.Export(calib);
    REQUIRE(calib[0] == 16383.5 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 1 << 3);

    size_t cnt = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        if (calib.Mask(i) != 0) cnt++;
    REQUIRE(cnt == RAW_MODULE_SIZE);

    // 128 images
    calc.AnalyzeImage(image.data());
    calc.Export(calib);
    REQUIRE(calib[0] == 12000 * 4);
    REQUIRE(calib.Mask(0) == 0);
    REQUIRE(calib[511*1024+33] == 12000 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 0);
}

#ifdef CUDA_SPOT_FINDING
TEST_CASE("PedestalCalcGPU", "[PedestalCalcGPU]") {
    for (int gain_level = 0; gain_level < 3; gain_level++) {
        uint16_t base_value;
        uint16_t wrong_value;
        uint16_t mask_value;
        DiffractionExperiment x;
        x.DataStreamModuleSize(1, {1});
        switch (gain_level) {
            case 1:
                base_value = 0x4000;
                wrong_value = 0xc000;
                mask_value = 4;
                x.Mode(DetectorMode::PedestalG1);
                break;
            case 2:
                base_value = 0xc000;
                wrong_value = 0;
                mask_value = 8;
                x.Mode(DetectorMode::PedestalG2);
                break;
            default:
                base_value = 0;
                wrong_value = 0x4000;
                mask_value = 2;
                x.Mode(DetectorMode::Conversion);
                break;
        }

        std::vector<uint16_t> image(RAW_MODULE_SIZE, base_value);

        for (int i = 0; i < RAW_MODULE_SIZE; i++)
            image[i] += i;

        image[2] = wrong_value;

        PedestalCalcGPU calc(x);

        // Predictable random number generator
        std::mt19937 g1(0);
        std::normal_distribution<double> distribution(12000.0, 12.5);

        for (int i = 0; i < 299; i++) {
            image[3] = base_value + distribution(g1);
            calc.AnalyzeImage(image.data());
        }
        image[3] = base_value + distribution(g1);

        image[0] = wrong_value;
        image[1] = wrong_value;
        image[2] = base_value;

        calc.AnalyzeImage(image.data());

        JFPedestal calib;
        // Previously these bits were part of the mask - checking if they are cleared properly
        calib.Mask(3) = mask_value;
        calib.Mask(1) = 8*mask_value;

        calc.Export(calib, 1, INFINITY);

        for (int i = 4; i < RAW_MODULE_COLS; i++)
            REQUIRE(calib[i] == i * 4);

        REQUIRE(calib[3] > 11900.0 * 4);
        REQUIRE(calib[3] < 12100.0 * 4);

        // Should be 12.5, but obviously with large error bar
        REQUIRE(calib.RMS(3) >= 12 * PEDESTAL_RMS_MULTIPLIER);
        REQUIRE(calib.RMS(3) <= 13 * PEDESTAL_RMS_MULTIPLIER);

        REQUIRE(calib[0] == 0);

        REQUIRE(calib.Mask(0) == 0);
        REQUIRE(calib.Mask(1) == 0);
        REQUIRE(calib.Mask(2) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(3) == 0);

        calc.Export(calib, 0, 10.0);
        for (int i = 4; i < RAW_MODULE_COLS; i++) {
            REQUIRE(calib[i] == i * 4);
        }

        REQUIRE(calib.Mask(3) == mask_value * 8); // RMS condition
        REQUIRE(calib.Mask(2) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(1) == mask_value); // Wrong gain
        REQUIRE(calib.Mask(0) == mask_value); // Wrong gain
    }
}

TEST_CASE("PedestalCalcGPU_ImagesLessThanWindow", "[PedestalCalcGPU]") {
    DiffractionExperiment x;
    x.DataStreamModuleSize(1, {1}).Mode(DetectorMode::PedestalG2);
    JFPedestal calib;
    PedestalCalcGPU calc(x, RAW_MODULE_LINES);

    // No images at all
    calc.Export(calib);
    REQUIRE(calib[0] == 16383.5 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 1 << 3);

    std::vector<uint16_t> image(RAW_MODULE_SIZE, 0xc000 + 12000);
    for (int i = 0; i < PEDESTAL_WINDOW_SIZE - 1; i++)
        calc.AnalyzeImage(image.data());

    // 127 images
    calc.Export(calib);
    REQUIRE(calib[0] == 16383.5 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 1 << 3);

    size_t cnt = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        if (calib.Mask(i) != 0) cnt++;
    REQUIRE(cnt == RAW_MODULE_SIZE);

    // 128 images
    calc.AnalyzeImage(image.data());
    calc.Export(calib);
    REQUIRE(calib[0] == 12000 * 4);
    REQUIRE(calib.Mask(0) == 0);
    REQUIRE(calib[511*1024+33] == 12000 * 4);
    REQUIRE(calib.Mask(511*1024+33) == 0);
}
#endif