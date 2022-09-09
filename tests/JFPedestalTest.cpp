// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <random>

#include "../common/JFPedestal.h"

TEST_CASE("JFPedestal_Serialize","[ProtoBuf][JFPedestal]") {
    // Predictable random number generator
    std::mt19937 g1(1576);
    std::normal_distribution<double> distribution(12000, 100);

    JFPedestal pedestal;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal[i] = static_cast<uint16_t>(distribution(g1));
        pedestal.RMS(i) = static_cast<int16_t>(distribution(g1));
        pedestal.Mask(i) = static_cast<uint8_t>(i % UINT8_MAX);
    }

    pedestal.collection_time = 3456;
    pedestal.frames = 5000;

    JFJochProtoBuf::JFPedestal serialized = pedestal;
    JFPedestal deserialized = serialized;

    size_t diff = 0, diff_rms = 0, diff_mask = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (pedestal[i] != deserialized[i])
            diff++;
        if (pedestal.RMS(i) != deserialized.RMS(i))
            diff_rms++;
        if (pedestal.Mask(i) != deserialized.Mask(i))
            diff_mask++;

    }
    REQUIRE(diff == 0);
    REQUIRE(diff_rms == 0);
    REQUIRE(diff_mask == 0);

    REQUIRE(deserialized.collection_time == pedestal.collection_time);
    REQUIRE(deserialized.frames == pedestal.frames);
}

TEST_CASE("JFPedestal_Mean","[JFPedestal]") {
    JFPedestal pedestal;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        pedestal[i] = 2455;

    REQUIRE(pedestal.Mean() == Approx(2455/4.0));
}

TEST_CASE("JFPedestal_MeanRMS","[JFPedestal]") {
    JFPedestal pedestal;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        pedestal.RMS(i) = 30000;

    REQUIRE(pedestal.MeanRMS() == Approx(30000));
}

TEST_CASE("JFPedestal_FP","[JFPedestal]") {
    JFPedestal pedestal;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        pedestal[i] = i;

    auto fp = pedestal.GetPedestalFP();

    REQUIRE(fp[0] == 0.0);
    REQUIRE(fp[1] == Approx(0.25f));
    REQUIRE(fp[2] == Approx(0.5f));
    REQUIRE(fp[65535] == Approx(65535 * 0.25f));
}