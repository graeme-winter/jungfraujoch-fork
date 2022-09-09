// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/RadialIntegration.h"

TEST_CASE("RadialIntegrationMapping_Constructor","[RadialIntegration]") {
    DiffractionExperiment x;

    std::unique_ptr<RadialIntegrationMapping> radial;

    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.1).HighQForRadialInt_recipA(5);
    REQUIRE_NOTHROW(radial = std::make_unique<RadialIntegrationMapping>(x));
}

TEST_CASE("RadialIntegrationMapping_Constructor_Mask","[RadialIntegration]") {
    DiffractionExperiment x{2,{8}, 8, 36};

    x.DetectorDistance_mm(50).BeamX_pxl(1000).BeamY_pxl(1000);
    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.001).HighQForRadialInt_recipA(9.9);

    std::vector<uint8_t> one_byte_mask(x.GetPixelsNum(), 1);
    one_byte_mask[456*1030+234] = 0;
    one_byte_mask[800*1030+74] = 0;

    std::unique_ptr<RadialIntegrationMapping> radial;

    REQUIRE_NOTHROW(radial = std::make_unique<RadialIntegrationMapping>(x, one_byte_mask.data()));
    REQUIRE(radial->GetPixelToBinMapping()[1030*512+344] != UINT16_MAX);
    REQUIRE(radial->GetPixelToBinMapping()[456*1030+234] == UINT16_MAX);
    REQUIRE(radial->GetPixelToBinMapping()[800*1030+74] == UINT16_MAX);
}

TEST_CASE("RadialIntegrationMapping_GetBinNumber","[RadialIntegration]") {
    DiffractionExperiment x{2,{8}, 8, 36};
    x.DetectorDistance_mm(50).BeamX_pxl(1000).BeamY_pxl(1000);
    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.1).HighQForRadialInt_recipA(4);
    RadialIntegrationMapping mapping(x);
    REQUIRE(mapping.GetBinNumber() == 39);
}

TEST_CASE("RadialIntegrationMapping_GetBinNumber_DetectorLimit","[RadialIntegration]") {
    DiffractionExperiment x{2,{8}, 8, 36};
    x.DetectorDistance_mm(50).BeamX_pxl(1000).BeamY_pxl(1000);

    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.1).HighQForRadialInt_recipA(9.9);
    RadialIntegrationMapping mapping(x);
    REQUIRE(mapping.GetBinNumber() < 80);
}

TEST_CASE("RadialIntegrationMapping_GetBinToQ","[RadialIntegration]") {
    DiffractionExperiment x{2,{8}, 8, 36};
    x.DetectorDistance_mm(50).BeamX_pxl(1000).BeamY_pxl(1000);
    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.1).HighQForRadialInt_recipA(4);

    RadialIntegrationMapping mapping(x);
    auto bin_to_q = mapping.GetBinToQ();

    REQUIRE(bin_to_q[0] == Approx(0.15));
    REQUIRE(bin_to_q[1] == Approx(0.25));
    REQUIRE(bin_to_q[15] == Approx(1.65));
    REQUIRE(bin_to_q[38] == Approx(3.95));
}

TEST_CASE("RadialIntegrationMapping_QToBin","[RadialIntegration]") {
    DiffractionExperiment x{2,{8}, 8, 36};
    x.DetectorDistance_mm(50).BeamX_pxl(1000).BeamY_pxl(1000);
    x.QSpacingForRadialInt_recipA(0.1).LowQForRadialInt_recipA(0.1).HighQForRadialInt_recipA(4);

    RadialIntegrationMapping mapping(x);

    REQUIRE(mapping.QToBin(0.0) == 0);
    REQUIRE(std::floor(mapping.QToBin(0.200001)) == 1);
    REQUIRE(mapping.QToBin(0.6) == Approx(5));
    REQUIRE(mapping.QToBin(50.0) == Approx(38));
}

TEST_CASE("RadialIntegration_Process","[RadialIntegration]") {
    std::vector<uint16_t> pixel_to_bin = {0,1,2,4,3,1,2,3};
    std::vector<int16_t> test_image = {7,6,5,4,3,2,1,0};
    std::vector<float> result;

    RadialIntegration radial(pixel_to_bin, 4);
    radial.Process(test_image.data(), 8);

    REQUIRE(radial.GetCount().size() == 4);
    REQUIRE(radial.GetSum().size() == 4);

    size_t count = 0;
    for (const auto &i: radial.GetCount())
        count += i;
    REQUIRE(count == 7); // bin number 4 is excluded

    REQUIRE(radial.GetCount()[0] == 1);
    REQUIRE(radial.GetCount()[1] == 2);

    REQUIRE(radial.GetSum()[0] == 7);
    REQUIRE(radial.GetSum()[1] == 6+2);
}

TEST_CASE("RadialIntegration_GetResult","[RadialIntegration]") {
    std::vector<uint16_t> pixel_to_bin = {0,1,2,4,3,1,2,3};
    std::vector<int16_t> test_image = {7,6,5,4,3,2,1,0};
    std::vector<float> result;

    RadialIntegration radial(pixel_to_bin, 4);
    radial.Process(test_image.data(), 8);
    radial.GetResult(result);

    REQUIRE(result.size() == 4);

    REQUIRE(result[0] == 7);
    REQUIRE(result[1] == (6 + 2) / 2.0);
    REQUIRE(result[2] == (5 + 1) / 2.0);
    REQUIRE(result[3] == (3 + 0) / 2.0);
}

TEST_CASE("RadialIntegration_GetRangeValue","[RadialIntegration]") {
    std::vector<uint16_t> pixel_to_bin = {0,1,2,4,3,1,2,3};
    std::vector<int16_t> test_image = {7,6,5,4,3,2,1,0};
    std::vector<float> result;

    RadialIntegration radial(pixel_to_bin, 4);
    radial.Process(test_image.data(), 8);
    REQUIRE(radial.GetRangeValue(0, 7) == Approx((0+1+2+3+5+6+7) / 7.0));
    REQUIRE(radial.GetRangeValue(2, 2) == Approx((5+1) / 2.0));
    REQUIRE(radial.GetRangeValue(2, 3) == Approx((5+3+1+0) / 4.0));
    REQUIRE(radial.GetRangeValue(15, 15) == 0); // Empty set
}
