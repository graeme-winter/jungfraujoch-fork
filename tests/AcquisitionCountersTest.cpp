// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../fpga/host/AcquisitionOnlineCounters.h"

TEST_CASE("AcquisitionDeviceCountersTest","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x(1,{2});
    x.NumTriggers(1).ImagesPerTrigger(50);

    AcquisitionOnlineCounters counters;
    counters.Reset(x, 0);
    REQUIRE(counters.GetSlowestHead() == 0);
    REQUIRE(counters.GetHead(0) == 0);
    REQUIRE(counters.GetHead(1) == 0);
    REQUIRE_THROWS(counters.GetHead(2));
    REQUIRE(counters.CalculateDelay(2) == -2);

    REQUIRE(!counters.IsAcquisitionFinished());

    Completion c{};
    c.frame_number = 32;
    c.module = 1;
    c.handle = 17;
    counters.UpdateCounters(&c);

    REQUIRE(counters.GetSlowestHead() == 0);
    REQUIRE(counters.GetHead(0) == 0);
    REQUIRE(counters.GetHead(1) == 32);

    REQUIRE(counters.CalculateDelay(31, 1) == 1);
    REQUIRE(counters.CalculateDelay(33, 1) == -1);
    REQUIRE(counters.GetBufferHandle(32, 1) == 17);
    REQUIRE(counters.GetBufferHandle(32, 0) == AcquisitionOnlineCounters::HandleNotFound);
    c.frame_number = 15;
    c.module = 0;
    counters.UpdateCounters(&c);

    REQUIRE(counters.GetSlowestHead() == 15);
    REQUIRE(counters.GetHead(0) == 15);
    REQUIRE(counters.GetHead(1) == 32);

    REQUIRE(counters.CalculateDelay(14) == 1);
    REQUIRE(counters.CalculateDelay(16) == -1);

    REQUIRE(counters.CalculateDelay(14, 0) == 1);
    REQUIRE(counters.CalculateDelay(16, 0) == -1);

    REQUIRE(counters.CalculateDelay(31, 1) == 1);
    REQUIRE(counters.CalculateDelay(33, 1) == -1);

    counters.SetAcquisitionFinished();

    REQUIRE(counters.GetSlowestHead() == 15);
    REQUIRE(counters.GetHead(0) == 15);
    REQUIRE(counters.GetHead(1) == 32);
    REQUIRE(counters.CalculateDelay(0) == 15);
    REQUIRE(counters.CalculateDelay(50) == 15-50);
}

TEST_CASE("AcquisitionDeviceCountersTest_OutOfBounds","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x(1,{2});
    x.NumTriggers(1).ImagesPerTrigger(50);

    AcquisitionOnlineCounters counters;
    counters.Reset(x, 0);

    Completion c{};
    c.frame_number = 50;
    REQUIRE_THROWS(counters.UpdateCounters(&c));

    c.frame_number = 80;
    REQUIRE_THROWS(counters.UpdateCounters(&c));
}