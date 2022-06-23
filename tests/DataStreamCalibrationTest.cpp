// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/JungfrauCalibration.h"
#include "../fpga/hls/hls_jfjoch.h"

TEST_CASE("JungfrauCalibration_LoadMaskTransformed","[JungfrauCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(2, {4, 4}, 8, 36).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    std::vector<uint32_t> new_mask(experiment.GetPixelsNum());
    new_mask[experiment.GetXPixelsNum() * 323 + 567] = 1;
    new_mask[experiment.GetXPixelsNum() * 2000 + 1500] = 2;

    JungfrauCalibration calibration(experiment);

    calibration.LoadMaskTransformed(experiment, new_mask, 4);

    std::vector<uint32_t> output(experiment.GetPixelsNum());
    calibration.GetMaskTransformed(experiment, output);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 1<<4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 1<<4);
    REQUIRE(calibration.CountMaskedPixels() == 2);

    std::vector<uint32_t> second_mask(experiment.GetPixelsNum());
    second_mask[experiment.GetXPixelsNum() * 323 + 568] = 1;
    calibration.LoadMaskTransformed(experiment, second_mask, 4);

    calibration.GetMaskTransformed(experiment, output);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 0);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 568] == 1 << 4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 0);
    REQUIRE(calibration.CountMaskedPixels() == 1);

    calibration.LoadMaskTransformed(experiment, new_mask, 3);

    calibration.GetMaskTransformed(experiment, output);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 1 << 3);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 568] == 1 << 4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 1 << 3);
    REQUIRE(calibration.CountMaskedPixels() == 3);
}

TEST_CASE("JungfrauCalibration_MaskDetectorGaps","[JungfrauCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(2, {4, 4}, 8, 36).UpsideDown(true)
            .PhotonEnergy_keV(WVL_1A_IN_KEV);
    JungfrauCalibration calibration(experiment, TASK_NO_DATA_STREAM);

    REQUIRE (calibration.Mask().size() == 8 * RAW_MODULE_SIZE);
    std::vector<uint32_t> mask_export;
    calibration.GetMaskTransformed(experiment, mask_export);

    REQUIRE(mask_export[1030] == 1);
    REQUIRE(mask_export[1031] == 1);
    REQUIRE(mask_export[1037] == 1);
    REQUIRE(mask_export[(1030*2+8)*514] == 1);
    REQUIRE(mask_export[(1030*2+8)*514 + 566] == 1);
    REQUIRE(mask_export[(1030*2+8)*(514*3+36*2+12) + 566] == 1);
}

TEST_CASE("JungfrauCalibration_SetPedestalProperties","[JSON][CBOR][JungfrauCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(1, {2}).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV).
            PedestalG0Frames(2545).PedestalG1Frames(343).PedestalG2Frames(2212);

    JungfrauCalibration calibration(experiment, 0);

    experiment.Mode(DetectorMode::PedestalG0);
    calibration.SetPedestalProperties(experiment, 111);
    experiment.Mode(DetectorMode::PedestalG1);
    calibration.SetPedestalProperties(experiment, 112);
    experiment.Mode(DetectorMode::PedestalG2);
    calibration.SetPedestalProperties(experiment, 113);

    REQUIRE(calibration.GetPedestalFrames(0) == 2545);
    REQUIRE(calibration.GetPedestalFrames(1) == 343);
    REQUIRE(calibration.GetPedestalFrames(2) == 2212);

    REQUIRE(calibration.GetPedestalTimestamp(0) == 111);
    REQUIRE(calibration.GetPedestalTimestamp(1) == 112);
    REQUIRE(calibration.GetPedestalTimestamp(2) == 113);
}

TEST_CASE("JungfrauCalibration_ExportProtoBuf","[ProtoBuf][JungfrauCalibration]") {
    uint16_t nmodules = 2;

    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(1, {nmodules}).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    JungfrauCalibration calibration(experiment, 0);
    std::vector<double> gain_vector(3 * RAW_MODULE_SIZE);
    std::vector<float> pedestal_vector(3 * RAW_MODULE_SIZE);
    std::vector<uint32_t> mask_vector(nmodules * RAW_MODULE_SIZE);

    calibration.SetPedestalProperties(0, 5167, 9111);
    calibration.SetPedestalProperties(1, 5267, 9211);
    calibration.SetPedestalProperties(2, 5367, 9311);

    std::mt19937 g1(16781);
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    for (auto &i: pedestal_vector) i = dist(g1);
    for (auto &i: mask_vector) i = dist(g1);

     REQUIRE_NOTHROW(calibration.LoadModulePedestal(pedestal_vector,0));
    REQUIRE_NOTHROW(calibration.LoadMask(mask_vector));

    JFJochProtoBuf::JFCalibration pb_calibration = calibration;

    JungfrauCalibration calibration2(experiment.GetModulesNum(0), 0);
    REQUIRE_NOTHROW(calibration2 = pb_calibration);

    experiment.Mode(DetectorMode::Raw);

    size_t diff = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {

        if (calibration.Pedestal(0)[i] != calibration2.Pedestal(0)[i]) diff++;
        if (calibration.Pedestal(1)[i] != calibration2.Pedestal(1)[i]) diff++;
        if (calibration.Pedestal(2)[i] != calibration2.Pedestal(2)[i]) diff++;

        if (calibration.Mask()[i] != calibration2.Mask()[i]) diff++;
    }
    REQUIRE(diff == 0);

    REQUIRE(calibration2.GetPedestalFrames(0) == 5167);
    REQUIRE(calibration2.GetPedestalFrames(1) == 5267);
    REQUIRE(calibration2.GetPedestalFrames(2) == 5367);

    REQUIRE(calibration2.GetPedestalTimestamp(0) == 9111);
    REQUIRE(calibration2.GetPedestalTimestamp(1) == 9211);
    REQUIRE(calibration2.GetPedestalTimestamp(2) == 9311);
}

TEST_CASE("JungfrauCalibration_ExportProtoBuf_ImportPedestalAndMask","[ProtoBuf][JungfrauCalibration]") {
    uint16_t nmodules = 2;

    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(1, {nmodules}).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    JungfrauCalibration calibration(experiment);
    std::vector<double> gain_vector(3 * RAW_MODULE_SIZE);
    std::vector<float> pedestal_vector(3 * RAW_MODULE_SIZE);
    std::vector<uint32_t> mask_vector(nmodules * RAW_MODULE_SIZE);

    calibration.SetPedestalProperties(0, 5167, 9111);
    calibration.SetPedestalProperties(1, 5267, 9211);
    calibration.SetPedestalProperties(2, 5367, 9311);

    std::mt19937 g1(16781);
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    for (auto &i: pedestal_vector) i = dist(g1);
    for (auto &i: mask_vector) i = dist(g1);

    REQUIRE_NOTHROW(calibration.LoadModulePedestal(pedestal_vector,0));
    REQUIRE_NOTHROW(calibration.LoadMask(mask_vector));

    JFJochProtoBuf::JFCalibration pb_calibration = calibration;

    JungfrauCalibration calibration2(experiment.GetModulesNum());
    REQUIRE_NOTHROW(calibration2.ImportPedestalAndMask(pb_calibration));

    experiment.Mode(DetectorMode::Raw);

    size_t diff = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (calibration.Pedestal(0)[i] != calibration2.Pedestal(0)[i]) diff++;
        if (calibration.Pedestal(1)[i] != calibration2.Pedestal(1)[i]) diff++;
        if (calibration.Pedestal(2)[i] != calibration2.Pedestal(2)[i]) diff++;

        if (calibration.Mask()[i] != calibration2.Mask()[i]) diff++;
    }

    REQUIRE(calibration2.GetPedestalFrames(0) == 5167);
    REQUIRE(calibration2.GetPedestalFrames(1) == 5267);
    REQUIRE(calibration2.GetPedestalFrames(2) == 5367);

    REQUIRE(calibration2.GetPedestalTimestamp(0) == 9111);
    REQUIRE(calibration2.GetPedestalTimestamp(1) == 9211);
    REQUIRE(calibration2.GetPedestalTimestamp(2) == 9311);
}

TEST_CASE("JungfrauCalibration_MeanPedestalRMS","[JungfrauCalibration]") {
    JungfrauCalibration calibration(1);
    REQUIRE(std::isnan(calibration.MeanPedestalRMS(0, 0)));
    REQUIRE(std::isnan(calibration.MeanPedestalRMS(1, 0)));
    REQUIRE(std::isnan(calibration.MeanPedestalRMS(2, 0)));

    for (auto &i: calibration.PedestalRMS(0)) i = 3.0 * PEDESTAL_RMS_MULTIPLIER;
    for (auto &i: calibration.PedestalRMS(1)) i = 4.0 * PEDESTAL_RMS_MULTIPLIER;
    for (auto &i: calibration.PedestalRMS(2)) i = 5.0 * PEDESTAL_RMS_MULTIPLIER;

    REQUIRE(calibration.MeanPedestalRMS(0, 0) == 3.0);
    REQUIRE(calibration.MeanPedestalRMS(1, 0) == 4.0);
    REQUIRE(calibration.MeanPedestalRMS(2, 0) == 5.0);
}

TEST_CASE("JungfrauCalibration_Statistics","[JungfrauCalibration]") {
    JungfrauCalibration calibration(4, 0);
    for (int module = 0; module < 4; module++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            calibration.Pedestal(0)[RAW_MODULE_SIZE * module + i] = 1000 * (module+1) * 4;
            calibration.Pedestal(1)[RAW_MODULE_SIZE * module + i] = 2000 * (module+1) * 4;
            calibration.Pedestal(2)[RAW_MODULE_SIZE * module + i] = 3000 * (module+1) * 4;
        }
    }
    calibration.Mask()[1024*511] = 4;

    calibration.Mask()[RAW_MODULE_SIZE + 1024*511+5] = 8;
    calibration.Mask()[RAW_MODULE_SIZE + 1024*511+6] = 8;
    calibration.Mask()[RAW_MODULE_SIZE + 1024*511+7] = 8;

    calibration.Mask()[3*RAW_MODULE_SIZE + 1024*511+5] = 8;
    calibration.Mask()[3*RAW_MODULE_SIZE + 1024*511+6] = 8;

    REQUIRE(calibration.MeanPedestal(0,0) == Approx(1000));
    REQUIRE(calibration.MeanPedestal(0,1) == Approx(2000));
    REQUIRE(calibration.MeanPedestal(0,2) == Approx(3000));

    REQUIRE(calibration.MeanPedestal(1,0) == Approx(2000));
    REQUIRE(calibration.MeanPedestal(1,1) == Approx(4000));
    REQUIRE(calibration.MeanPedestal(1,2) == Approx(6000));

    REQUIRE(calibration.MeanPedestal(2,0) == Approx(3000));
    REQUIRE(calibration.MeanPedestal(2,1) == Approx(6000));
    REQUIRE(calibration.MeanPedestal(2,2) == Approx(9000));

    REQUIRE(calibration.CountMaskedPixels(0) == 1);
    REQUIRE(calibration.CountMaskedPixels(1) == 3);
    REQUIRE(calibration.CountMaskedPixels(2) == 0);
    REQUIRE(calibration.CountMaskedPixels(3) == 2);
}

TEST_CASE("JungfrauCalibration_ComparePedestals","[JungfrauCalibration]") {
    const uint16_t nmodules = 4;
    JungfrauCalibration calibration1(nmodules, 0);
    JungfrauCalibration calibration2(nmodules, 0);
    JungfrauCalibration calibration3(nmodules, 0);
    JungfrauCalibration calibration4(nmodules, 0);

    std::vector<float> pedestal1(3*RAW_MODULE_SIZE);
    std::vector<float> pedestal2(3*RAW_MODULE_SIZE);
    for (auto &i: pedestal1)
        i = 1.0;
    for (auto &i: pedestal2)
        i = 2.0;

    for (int i = 0; i < nmodules; i++) {
        calibration1.LoadModulePedestal(pedestal1, i);
        calibration2.LoadModulePedestal(pedestal1, i);
        calibration3.LoadModulePedestal(pedestal2, i);
    }
    calibration4.LoadModulePedestal(pedestal2, 0);
    for (int i = 1; i < nmodules; i++)
        calibration4.LoadModulePedestal(pedestal1, i);

    REQUIRE(calibration1.ComparePedestals(calibration2, 0) == 0.0);
    REQUIRE(calibration1.ComparePedestals(calibration3, 0) == 1.0);
    REQUIRE(calibration2.ComparePedestals(calibration3, 0) == 1.0);
    REQUIRE(calibration1.ComparePedestals(calibration4, 0) == sqrt(1.0/4.0));
    REQUIRE(calibration3.ComparePedestals(calibration4, 0) == sqrt(3.0/4.0));
}

TEST_CASE("JungfrauCalibration_MaskModuleEdges","[JungfrauCalibration]") {
    DiffractionExperiment experiment(1,{4});
    experiment.MaskModuleEdges(true).Mode(DetectorMode::Raw);

    JungfrauCalibration calibration(experiment);

    std::vector<uint32_t> mask_out;
    calibration.GetMaskTransformed(experiment, mask_out);

    CHECK(mask_out[0] == (1u<<30));
    CHECK(mask_out[1022] == (1u<<30));
    CHECK(mask_out[1024*5] == (1u<<30));
    CHECK(mask_out[1024*5+1023] == (1u<<30));
    CHECK(mask_out[1024*512-1] == (1u<<30));
    CHECK(mask_out[1024*512] == (1u<<30));
    CHECK(mask_out[1024*512*3+123] == (1u<<30));
    CHECK(mask_out[1024*512*3+1024] == (1u<<30));
    CHECK(mask_out[1024*800+256] == 0);
}

TEST_CASE("JungfrauCalibration_MaskChipEdges","[JungfrauCalibration]") {
    DiffractionExperiment experiment(1,{4});
    experiment.MaskChipEdges(true).Mode(DetectorMode::Raw);

    JungfrauCalibration calibration(experiment);

    std::vector<uint32_t> mask_out;
    calibration.GetMaskTransformed(experiment, mask_out);

    CHECK(mask_out[255] == (1u<<31));
    CHECK(mask_out[1024*800+256] == (1u<<31));

    CHECK(mask_out[1024*233+511] == (1u<<31));
    CHECK(mask_out[1024*512+512] == (1u<<31));
    CHECK(mask_out[1024*1000+767] == (1u<<31));
    CHECK(mask_out[1024*(512*3+12)+768] == (1u<<31));
    CHECK(mask_out[1024*(512+255)+345] == (1u<<31));
    CHECK(mask_out[1024*(1024+256)+876] == (1u<<31));

    CHECK(mask_out[1022] == 0);
    CHECK(mask_out[1024*5+1023] == 0);
    CHECK(mask_out[1024*18] == 0);
}

TEST_CASE("JungfrauCalibration_GetMaskOneByte","[JungfrauCalibration]") {
    DiffractionExperiment experiment(1, {4}, 8, 36);
    experiment.UpsideDown(false).MaskModuleEdges(false).MaskChipEdges(false);
    JungfrauCalibration calibration(experiment);
    calibration.Mask()[0] = 50;
    calibration.Mask()[1] = 30;
    calibration.Mask()[RAW_MODULE_SIZE * 4 - 1] = 10;
    auto mask_v = calibration.GetMaskOneByte(experiment);

    REQUIRE(mask_v.size() == experiment.GetPixelsNum() );
    REQUIRE(mask_v[0] == 0);
    REQUIRE(mask_v[1] == 0);
    REQUIRE(mask_v[5] == 1);
    REQUIRE(mask_v[experiment.GetPixelsNum() - 1] == 0);
    REQUIRE(mask_v[518*(experiment.GetXPixelsNum())] == 0);
}