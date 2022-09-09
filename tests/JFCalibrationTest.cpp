// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <random>

#include "../common/JFCalibration.h"

TEST_CASE("JFCalibration_Constructor","[JFCalibration]") {
    DiffractionExperiment experiment(2, {4,4});
    experiment.StorageCells(16);

    JFCalibration c(experiment);
    REQUIRE(c.GetStorageCellNum() == 16);
    REQUIRE(c.GetModulesNum() == 8);
}

TEST_CASE("JFCalibration_Statistics","[JFCalibration]") {
    JFCalibration calibration(4, 2);

    for (int module = 0; module < 4; module++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            calibration.Pedestal(module, 0, 1)[i] = 1000 * (module+1) * 4;
            calibration.Pedestal(module, 1, 1)[i] = 2000 * (module+1) * 4;
            calibration.Pedestal(module, 2, 1)[i] = 3000 * (module+1) * 4;
        }
    }
    calibration.Mask(1024*511) = 4;

    calibration.Mask(RAW_MODULE_SIZE + 1024*511+5) = 8;
    calibration.Mask(RAW_MODULE_SIZE + 1024*511+6) = 8;
    calibration.Mask(RAW_MODULE_SIZE + 1024*511+7) = 8;

    calibration.Mask(3*RAW_MODULE_SIZE + 1024*511+5) = 8;
    calibration.Mask(3*RAW_MODULE_SIZE + 1024*511+6) = 8;

    calibration.Pedestal(3, 0, 1).Mask(0) = 5;
    calibration.Pedestal(3, 1, 1).Mask(1) = 5;
    calibration.Pedestal(3, 2, 1).Mask(1) = 5;
    calibration.Pedestal(3, 2, 1).Mask(2) = 5;

    auto s1 = calibration.GetModuleStatistics(1);

    REQUIRE(s1.module_statistics(0).pedestal_g0_mean() == Approx(1000));
    REQUIRE(s1.module_statistics(0).pedestal_g1_mean() == Approx(2000));
    REQUIRE(s1.module_statistics(0).pedestal_g2_mean() == Approx(3000));

    REQUIRE(s1.module_statistics(1).pedestal_g0_mean() == Approx(2000));
    REQUIRE(s1.module_statistics(1).pedestal_g1_mean() == Approx(4000));
    REQUIRE(s1.module_statistics(1).pedestal_g2_mean() == Approx(6000));

    REQUIRE(s1.module_statistics(2).pedestal_g0_mean() == Approx(3000));
    REQUIRE(s1.module_statistics(2).pedestal_g1_mean() == Approx(6000));
    REQUIRE(s1.module_statistics(2).pedestal_g2_mean() == Approx(9000));

    REQUIRE(s1.module_statistics(0).masked_pixels() == 1);
    REQUIRE(s1.module_statistics(1).masked_pixels() == 3);
    REQUIRE(s1.module_statistics(2).masked_pixels() == 0);
    REQUIRE(s1.module_statistics(3).masked_pixels() == 5);

    auto s0 = calibration.GetModuleStatistics(0);

    REQUIRE(s0.module_statistics(0).pedestal_g0_mean() == 0);
    REQUIRE(s0.module_statistics(0).pedestal_g1_mean() == 0);
    REQUIRE(s0.module_statistics(0).pedestal_g2_mean() == 0);

    REQUIRE(s0.module_statistics(0).masked_pixels() == 1);
    REQUIRE(s0.module_statistics(1).masked_pixels() == 3);
    REQUIRE(s0.module_statistics(2).masked_pixels() == 0);
    REQUIRE(s0.module_statistics(3).masked_pixels() == 2);
}

TEST_CASE("JFCalibration_Statistics_All","[JFCalibration]") {
    JFCalibration calibration(4, 2);

    for (int module = 0; module < 4; module++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            calibration.Pedestal(module, 0, 1)[i] = 1000 * (module+1) * 4;
            calibration.Pedestal(module, 1, 1)[i] = 2000 * (module+1) * 4;
            calibration.Pedestal(module, 2, 1)[i] = 3000 * (module+1) * 4;
        }
    }
    calibration.Mask(1024*511) = 4;

    calibration.Mask(RAW_MODULE_SIZE + 1024*511+5) = 8;
    calibration.Mask(RAW_MODULE_SIZE + 1024*511+6) = 8;
    calibration.Mask(RAW_MODULE_SIZE + 1024*511+7) = 8;

    calibration.Mask(3*RAW_MODULE_SIZE + 1024*511+5) = 8;
    calibration.Mask(3*RAW_MODULE_SIZE + 1024*511+6) = 8;

    calibration.Pedestal(3, 0, 1).Mask(0) = 5;
    calibration.Pedestal(3, 1, 1).Mask(1) = 5;
    calibration.Pedestal(3, 2, 1).Mask(1) = 5;
    calibration.Pedestal(3, 2, 1).Mask(2) = 5;

    auto s0 = calibration.GetModuleStatistics();

    REQUIRE(s0.module_statistics(0).module_number() == 0);
    REQUIRE(s0.module_statistics(0).storage_cell_number() == 0);
    REQUIRE(s0.module_statistics(5).module_number() == 1);
    REQUIRE(s0.module_statistics(5).storage_cell_number() == 1);
    REQUIRE(s0.module_statistics(7).module_number() == 3);
    REQUIRE(s0.module_statistics(7).storage_cell_number() == 1);

    REQUIRE(s0.module_statistics(0).pedestal_g0_mean() == 0);
    REQUIRE(s0.module_statistics(0).pedestal_g1_mean() == 0);
    REQUIRE(s0.module_statistics(0).pedestal_g2_mean() == 0);

    REQUIRE(s0.module_statistics(0).masked_pixels() == 1);
    REQUIRE(s0.module_statistics(1).masked_pixels() == 3);
    REQUIRE(s0.module_statistics(2).masked_pixels() == 0);
    REQUIRE(s0.module_statistics(3).masked_pixels() == 2);

    REQUIRE(s0.module_statistics(4).pedestal_g0_mean() == Approx(1000));
    REQUIRE(s0.module_statistics(4).pedestal_g1_mean() == Approx(2000));
    REQUIRE(s0.module_statistics(4).pedestal_g2_mean() == Approx(3000));

    REQUIRE(s0.module_statistics(5).pedestal_g0_mean() == Approx(2000));
    REQUIRE(s0.module_statistics(5).pedestal_g1_mean() == Approx(4000));
    REQUIRE(s0.module_statistics(5).pedestal_g2_mean() == Approx(6000));

    REQUIRE(s0.module_statistics(6).pedestal_g0_mean() == Approx(3000));
    REQUIRE(s0.module_statistics(6).pedestal_g1_mean() == Approx(6000));
    REQUIRE(s0.module_statistics(6).pedestal_g2_mean() == Approx(9000));

    REQUIRE(s0.module_statistics(4).masked_pixels() == 1);
    REQUIRE(s0.module_statistics(5).masked_pixels() == 3);
    REQUIRE(s0.module_statistics(6).masked_pixels() == 0);
    REQUIRE(s0.module_statistics(7).masked_pixels() == 5);
}

TEST_CASE("JFCalibration_MaskModuleEdges","[JFCalibration]") {
    DiffractionExperiment experiment(1,{4});
    experiment.MaskModuleEdges(true).Mode(DetectorMode::Raw);

    JFCalibration calibration(experiment);

    auto mask_out = calibration.CalculateNexusMask(experiment);

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

TEST_CASE("JFCalibration_MaskChipEdges","[JFCalibration]") {
    DiffractionExperiment experiment(1,{4});
    experiment.MaskChipEdges(true).Mode(DetectorMode::Raw);

    JFCalibration calibration(experiment);

    auto mask_out = calibration.CalculateNexusMask(experiment);

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

TEST_CASE("JFCalibration_CalculateNexusMask","[JFCalibration]") {
    DiffractionExperiment experiment(1, {4}, 8, 36);
    experiment.UpsideDown(false).MaskModuleEdges(false).MaskChipEdges(false);
    JFCalibration calibration(4, 2);

    calibration.Mask(0) = 50;
    calibration.Mask(1) = 30;
    calibration.Pedestal(0, 0, 0).Mask(2) = 4;
    calibration.Pedestal(0, 0, 1).Mask(3) = 4;
    calibration.Pedestal(0, 1, 0).Mask(4) = 8;
    calibration.Pedestal(0, 2, 0).Mask(5) = 16;

    auto mask_v = calibration.CalculateNexusMask(experiment);

    REQUIRE(mask_v.size() == experiment.GetPixelsNum() );
    REQUIRE(mask_v[0] == 50);
    REQUIRE(mask_v[1] == 30);
    REQUIRE(mask_v[2] == 4);
    REQUIRE(mask_v[3] == 0);
    REQUIRE(mask_v[4] == 8);
    REQUIRE(mask_v[5] == 16);
    REQUIRE(mask_v[6] == 0);
    REQUIRE(mask_v[1030 * 700 + 300] == 0);

    mask_v = calibration.CalculateNexusMask(experiment, 1);
    REQUIRE(mask_v[0] == 50);
    REQUIRE(mask_v[1] == 30);
    REQUIRE(mask_v[2] == 0);
    REQUIRE(mask_v[3] == 4);
    REQUIRE(mask_v[4] == 0);
    REQUIRE(mask_v[5] == 0);
    REQUIRE(mask_v[6] == 0);
    REQUIRE(mask_v[1030 * 700 + 300] == 0);
}

TEST_CASE("JFCalibration_CalculateOneByteMask","[JFCalibration]") {
    DiffractionExperiment experiment(1, {4}, 8, 36);
    experiment.UpsideDown(false).MaskModuleEdges(false).MaskChipEdges(false);
    JFCalibration calibration(experiment);
    calibration.Mask(0) = 50;
    calibration.Mask(1) = 30;
    calibration.Mask(RAW_MODULE_SIZE * 4 - 1) = 10;

    auto mask_v = calibration.CalculateOneByteMask(experiment);

    REQUIRE(mask_v.size() == experiment.GetPixelsNum() );
    REQUIRE(mask_v[0] == 0); // Explicitly masked
    REQUIRE(mask_v[1] == 0); // Explicitly masked
    REQUIRE(mask_v[5] == 1);
    REQUIRE(mask_v[(514*2+36*2+1)*(experiment.GetXPixelsNum()) + 876] == 1);
    REQUIRE(mask_v[experiment.GetPixelsNum() - 256] == 1);
    REQUIRE(mask_v[experiment.GetPixelsNum() - 2] == 1);
    REQUIRE(mask_v[experiment.GetPixelsNum() - 1] == 0); // Explicitly masked
    REQUIRE(mask_v[518*(experiment.GetXPixelsNum())] == 0); // Y Gap
}

TEST_CASE("JFCalibration_LoadMask_Bitwise","[JFCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(2, {4, 4}, 8, 36).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    std::vector<uint32_t> new_mask(experiment.GetPixelsNum());
    new_mask[experiment.GetXPixelsNum() * 323 + 567] = 1;
    new_mask[experiment.GetXPixelsNum() * 2000 + 1500] = 2;

    JFCalibration calibration(experiment);

    calibration.LoadMask(experiment, new_mask, 4);

    auto output = calibration.CalculateNexusMask(experiment);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 1<<4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 1<<4);

    std::vector<uint32_t> second_mask(experiment.GetPixelsNum());
    second_mask[experiment.GetXPixelsNum() * 323 + 568] = 1;
    calibration.LoadMask(experiment, second_mask, 4);

    output = calibration.CalculateNexusMask(experiment);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 0);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 568] == 1 << 4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 0);

    calibration.LoadMask(experiment, new_mask, 3);

    output = calibration.CalculateNexusMask(experiment);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 1 << 3);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 568] == 1 << 4);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 1 << 3);
}

TEST_CASE("JFCalibration_LoadMask","[JFCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(2, {4, 4}, 8, 36).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    std::vector<uint32_t> new_mask(experiment.GetPixelsNum());
    new_mask[experiment.GetXPixelsNum() * 323 + 567] = 1;
    new_mask[experiment.GetXPixelsNum() * 2000 + 1500] = 2;

    JFCalibration calibration(experiment);

    calibration.LoadMask(experiment, new_mask);

    auto output = calibration.CalculateNexusMask(experiment);
    REQUIRE(output[experiment.GetXPixelsNum() * 323 + 567] == 1);
    REQUIRE(output[experiment.GetXPixelsNum() * 2000 + 1500] == 2);
}

TEST_CASE("JFCalibration_MaskDetectorGaps","[JFCalibration]") {
    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(2, {4, 4}, 8, 36).UpsideDown(true)
            .PhotonEnergy_keV(WVL_1A_IN_KEV);
    JFCalibration calibration(experiment);

    auto mask_export = calibration.CalculateNexusMask(experiment);

    REQUIRE(mask_export[0] == 0);
    REQUIRE(mask_export[1029] == 0);
    REQUIRE(mask_export[1030] == 1);
    REQUIRE(mask_export[1031] == 1);
    REQUIRE(mask_export[1037] == 1);
    REQUIRE(mask_export[1038] == 0);

    REQUIRE(mask_export[(1030*2+8)*514] == 1);
    REQUIRE(mask_export[(1030*2+8)*514 + 566] == 1);
    REQUIRE(mask_export[(1030*2+8)*(514*3+36*2+12) + 566] == 1);
}

TEST_CASE("JFCalibration_Serialize","[ProtoBuf][JFCalibration]") {
    uint16_t nmodules = 2;
    uint16_t nstorage_cells = 2;

    DiffractionExperiment experiment;

    experiment.DataStreamModuleSize(1, {nmodules}).UpsideDown(true).PhotonEnergy_keV(WVL_1A_IN_KEV);

    JFCalibration calibration(nmodules, nstorage_cells);

    // Predictable random number generator
    std::mt19937 g1(1576223);
    std::normal_distribution<double> distribution(12000, 100);
    std::uniform_int_distribution<uint8_t> mask_distribution;

    for (int m = 0; m < nmodules; m++) {
        for (int s = 0; s < nstorage_cells; s++) {
            for (int g = 0; g < 2; g++) {
                for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                    calibration.Pedestal(m, g, s)[i] = static_cast<uint16_t>(distribution(g1));
                    calibration.Pedestal(m, g, s).Mask(i) = static_cast<uint8_t>(mask_distribution(g1));
                }
                calibration.Pedestal(m, g, s).collection_time = m * s + 2 * g;
                calibration.Pedestal(m, g, s).frames = m + s * g;
            }
        }
    }

    for (int i = 0; i < RAW_MODULE_SIZE * nmodules; i++)
        calibration.Mask(i) = i % 65534;

    JFJochProtoBuf::JFCalibration serialized = calibration;

    JFCalibration deserialized(serialized);
    REQUIRE(deserialized.GetStorageCellNum() == nstorage_cells);
    REQUIRE(deserialized.GetModulesNum() == nmodules);

    uint64_t diff_pedestal = 0, diff_pedestal_mask = 0, diff_pedestal_coll_time = 0,
        diff_pedestal_frames = 0, diff_mask = 0;

    for (int m = 0; m < nmodules; m++) {
        for (int s = 0; s < nstorage_cells; s++) {
            for (int g = 0; g < 2; g++) {
                for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                    if (calibration.Pedestal(m, g, s)[i] != deserialized.Pedestal(m, g, s)[i])
                        diff_pedestal++;
                    if (calibration.Pedestal(m, g, s).Mask(i) != deserialized.Pedestal(m, g, s).Mask(i))
                        diff_pedestal_mask++;
                }
                if (calibration.Pedestal(m, g, s).collection_time != deserialized.Pedestal(m, g, s).collection_time)
                    diff_pedestal_coll_time++;
                if (calibration.Pedestal(m, g, s).frames != deserialized.Pedestal(m, g, s).frames)
                    diff_pedestal_frames++;
            }
        }
    }

    for (int i = 0; i < RAW_MODULE_SIZE * nmodules; i++)
        if (calibration.Mask(i) != deserialized.Mask(i)) diff_mask++;

    REQUIRE(diff_pedestal == 0);
    REQUIRE(diff_pedestal_mask == 0);
    REQUIRE(diff_pedestal_coll_time == 0);
    REQUIRE(diff_pedestal_frames == 0);
    REQUIRE(diff_mask == 0);
}

TEST_CASE("JFCalibration_GetPedestal","[JFCalibration]") {
    JFCalibration calibration(4, 2);

    for (int module = 0; module < 4; module++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            calibration.Pedestal(module, 1, 1)[i] = 2000 * (module + 1) * 4 + (i % 30000);
        }
    }

    auto x = calibration.GetPedestal(1, 1);
    REQUIRE(x[0] == Approx(2000));
    REQUIRE(x[2*RAW_MODULE_SIZE] == Approx(3 * 2000));
    REQUIRE(x[3*RAW_MODULE_SIZE+500] == Approx(4 * 2000 + 500/4.0f));
}