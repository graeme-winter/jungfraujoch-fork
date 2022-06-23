// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../fpga/host/AcquisitionDeviceCounters.h"

TEST_CASE("AcquisitionDeviceCountersTest","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x;
    x.NumTriggers(1).ImagesPerTrigger(50).DataStreamModuleSize(1,{2});

    AcquisitionDeviceCounters counters;
    counters.Reset(x, 0);
    REQUIRE(counters.GetSlowestHead() == 0);
    REQUIRE(counters.GetHead(0) == 0);
    REQUIRE(counters.GetHead(1) == 0);
    REQUIRE_THROWS(counters.GetHead(2));
    REQUIRE(counters.CalculateDelay(2) == -2);

    REQUIRE(!counters.GetAcqusitionFinished());

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
    REQUIRE(counters.GetBufferHandle(32, 0) == AcquisitionDeviceCounters::HandleNotFound);
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

TEST_CASE("AcquisitionDeviceCountersTest_Pedestal","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x;
    x.NumTriggers(1).ImagesPerTrigger(50).DataStreamModuleSize(1,{2});

    AcquisitionDeviceCounters counters;
    counters.Reset(x, 0);

    Completion c{};
    c.frame_number = PEDESTAL_FRAME_ID;
    c.module = 1;
    c.handle = 17;
    counters.UpdateCounters(&c);

    REQUIRE(counters.GetPedestalHandle(0) == AcquisitionDeviceCounters::HandleNotFound);
    REQUIRE(counters.GetPedestalHandle(1) == 17);
    REQUIRE(counters.CalculateDelay(49,1) == -49); // receiving pedestal frame has no effect on delay calculation
}

TEST_CASE("AcquisitionDeviceCountersTest_PacketMask","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x;
    x.NumTriggers(1).ImagesPerTrigger(50).DataStreamModuleSize(1,{2});

    AcquisitionDeviceCounters counters;
    counters.Reset(x, 0);

    Completion c{};
    c.frame_number = 45;
    c.module = 1;
    c.handle = 17;
    c.packet_mask[0] = UINT64_MAX;
    c.packet_mask[1] = UINT64_MAX - 1 - 2 - 4 - 8 - 16;
    counters.UpdateCounters(&c);

    c.frame_number = 45;
    c.module = 0;
    c.handle = 14;
    c.packet_mask[0] = 0;
    c.packet_mask[1] = UINT64_MAX;
    counters.UpdateCounters(&c);

    REQUIRE(counters.GetPacketMaskHalfModule(45, 0) == 0);
    REQUIRE(counters.GetPacketCountHalfModule(45,0) == 0);

    REQUIRE(counters.GetPacketMaskHalfModule(45, 1) == UINT64_MAX);
    REQUIRE(counters.GetPacketCountHalfModule(45,1) == 64);

    REQUIRE(!counters.IsPacketCollected(45, 0, 0));
    REQUIRE(!counters.IsPacketCollected(45, 0, 63));
    REQUIRE(counters.IsPacketCollected(45, 0, 64));
    REQUIRE(counters.IsPacketCollected(45, 0, 127));

    REQUIRE(counters.GetPacketCount(45, 0) == 64);

    REQUIRE(counters.GetPacketMaskHalfModule(45, 2) == UINT64_MAX);
    REQUIRE(counters.GetPacketCountHalfModule(45,2) == 64);

    REQUIRE(counters.GetPacketMaskHalfModule(45, 3) == UINT64_MAX - 1 - 2 - 4 - 8 - 16);
    REQUIRE(counters.GetPacketCountHalfModule(45,3) == 64 - 5);

    REQUIRE(counters.GetPacketCount(45, 1) == 128 - 5);

    REQUIRE(counters.IsPacketCollected(45, 1, 0));
    REQUIRE(counters.IsPacketCollected(45, 1, 63));
    REQUIRE(!counters.IsPacketCollected(45, 1, 64));
    REQUIRE(!counters.IsPacketCollected(45, 1, 65));
    REQUIRE(!counters.IsPacketCollected(45, 1, 66));
    REQUIRE(!counters.IsPacketCollected(45, 1, 67));
    REQUIRE(!counters.IsPacketCollected(45, 1, 68));

    REQUIRE(counters.IsPacketCollected(45, 1, 69));
    REQUIRE(counters.IsPacketCollected(45, 1, 70));
    REQUIRE(counters.IsPacketCollected(45, 1, 127));
}

TEST_CASE("AcquisitionDeviceCountersTest_OutOfBounds","[AcquisitionDeviceCounters]") {
    DiffractionExperiment x;
    x.NumTriggers(1).ImagesPerTrigger(50).DataStreamModuleSize(1,{2});

    AcquisitionDeviceCounters counters;
    counters.Reset(x, 0);

    Completion c{};
    c.frame_number = 50;
    REQUIRE_THROWS(counters.UpdateCounters(&c));

    c.frame_number = 80;
    REQUIRE_THROWS(counters.UpdateCounters(&c));
}