// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../fpga/host/AcquisitionDeviceFilter.h"

TEST_CASE("AcquisitionDeviceFilter_TimeResolved","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(567).ImagesPerTrigger(1).TimeResolvedMode(true);

    AcquisitionDeviceFilter filter(x);

    Completion c;
    c.frame_number = 555+1;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 555);

    c.frame_number = 567+1;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);
}

TEST_CASE("AcquisitionDeviceFilter_OneTrigger","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.Mode(DetectorMode::Conversion).NumTriggers(1).ImagesPerTrigger(10);
    REQUIRE(x.GetNumTriggers() == 1);
    REQUIRE(x.GetFrameNumPerTrigger() == 10);

    AcquisitionDeviceFilter filter(x);

    Completion c;

    c.frame_number = 1;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 1;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 6;
    REQUIRE(filter.ProcessCompletion(c) == 5);

    c.frame_number = 10;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 9);

    c.frame_number = 20;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);
}


TEST_CASE("AcquisitionDeviceFilter_ZeroTriggers","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(0).ImagesPerTrigger(10);
    AcquisitionDeviceFilter filter(x);

    Completion c;
    c.frame_number = 1;
    REQUIRE(filter.ProcessCompletion(c) == 0);


    c.frame_number = 6;
    REQUIRE(filter.ProcessCompletion(c) == 5);

    c.frame_number = 11;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);

    c.frame_number = 801;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);

    c.frame_number = PEDESTAL_FRAME_ID;
    REQUIRE(filter.ProcessCompletion(c) == PEDESTAL_FRAME_ID);

}

TEST_CASE("AcquisitionDeviceFilter_Completion","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(2).ImagesPerTrigger(10);
    AcquisitionDeviceFilter filter(x);

    Completion c{};
    c.frame_number = 1;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 20;
    REQUIRE(filter.ProcessCompletion(c) == 19);

    c.frame_number = 35;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);
}