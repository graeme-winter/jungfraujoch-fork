// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../common/FrameTransformationPreview.h"
#include "../common/RawToConvertedGeometry.h"

TEST_CASE("FrameTransformationPreview_NoSummation" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::None);

    JungfrauCalibration calibration(experiment);

    std::mt19937 g1(1687);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<int16_t> output(experiment.GetPixelsNum());

    std::vector<uint8_t> mask(experiment.GetPixelsNum(), 1);
    FrameTransformationPreview transformation(experiment, mask, output.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    REQUIRE_NOTHROW(transformation.Pack());

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(input_0[311*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 38]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(input_1[(311+2*512)*1024]     == output[200 * 1030 * 2 + 0]);
    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformationPreview_NoSummation_Mask" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::None);

    JungfrauCalibration calibration(experiment);

    std::mt19937 g1(1687);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<int16_t> output(experiment.GetPixelsNum());

    std::vector<uint8_t> mask(experiment.GetPixelsNum(), 1);
    mask[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0] = 0;

    FrameTransformationPreview transformation(experiment, mask, output.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    REQUIRE_NOTHROW(transformation.Pack());

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(INT16_MIN                     == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 38]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(input_1[(311+2*512)*1024]     == output[200 * 1030 * 2 + 0]);
    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformationPreview_Summation" ,"") {

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(1,{1}, 0, 0)
            .UpsideDown(false).Compression(CompressionAlgorithm::None).ImageTime(std::chrono::milliseconds(4))
            .FrameSummationEnable(false);

    REQUIRE(experiment.GetSummation()           == 1);
    REQUIRE(experiment.GetSummationForPreview() == 4);

    JungfrauCalibration calibration(experiment);

    std::vector<int16_t> input_0(RAW_MODULE_SIZE);
    input_0[0] = INT16_MAX/4 + 5;
    input_0[1] = INT16_MAX - 3;
    input_0[2] = INT16_MIN/4 - 10;
    input_0[3] = INT16_MIN;
    input_0[4] = 55;
    input_0[511*1024 + 3*256 + 45] = -11;

    std::vector<int16_t> output(experiment.GetPixelsNum());
    std::vector<uint8_t> mask(experiment.GetPixelsNum(), 1);

    FrameTransformationPreview transformation(experiment, mask, output.data());

    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 0,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 0,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 0,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 0,0, 0));

    REQUIRE_NOTHROW(transformation.Pack());

    REQUIRE(output[0] == INT16_MAX);
    REQUIRE(output[1] == INT16_MAX);

    REQUIRE(output[2] == INT16_MIN);
    REQUIRE(output[3] == INT16_MIN);
    REQUIRE(output[4] == 55 * 4);
    REQUIRE(output[513*1030 + 3 * 258 + 45] == -11 * 4);
}

