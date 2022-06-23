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
    c.frame_number = 0;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);

    c.frame_number = 10;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 10;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 15;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 5);

    c.frame_number = 19;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 9);

    c.frame_number = 20;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);
}

TEST_CASE("AcquisitionDeviceFilter_TwoTriggers","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(2).ImagesPerTrigger(10);

    AcquisitionDeviceFilter filter(x);

    Completion c;
    c.frame_number = 0;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);

    c.frame_number = 10;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 10;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 15;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 5);

    c.frame_number = 19;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 9);

    c.frame_number = 20;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);

    c.frame_number = 220;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 10);

    c.frame_number = 225;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 15);

    c.frame_number = 230;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);

    c.frame_number = 229;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 19);

    REQUIRE(filter.TriggerSequenceFrameNumbers().size() == 2);
    REQUIRE(filter.TriggerSequenceFrameNumbers()[0] == 9);
    REQUIRE(filter.TriggerSequenceFrameNumbers()[1] == 219);
}


TEST_CASE("AcquisitionDeviceFilter_ZeroTriggers","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(0).ImagesPerTrigger(10);
    AcquisitionDeviceFilter filter(x);

    Completion c;
    c.frame_number = 1;
    c.trigger = false;
    REQUIRE(filter.ProcessCompletion(c) == 0);


    c.frame_number = 6;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 5);

    c.frame_number = 11;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);

    c.frame_number = 801;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);

    c.frame_number = PEDESTAL_FRAME_ID;
    REQUIRE(filter.ProcessCompletion(c) == PEDESTAL_FRAME_ID);

}

TEST_CASE("AcquisitionDeviceFilter_Completion","[AcquisitionDeviceFilter]") {
    DiffractionExperiment x;
    x.NumTriggers(2).ImagesPerTrigger(10);
    AcquisitionDeviceFilter filter(x);

    Completion c;
    c.frame_number = 1;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 0);

    c.frame_number = 20;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == 10);

    c.frame_number = 34;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameIgnore);

    c.frame_number = 35;
    c.trigger = true;
    REQUIRE(filter.ProcessCompletion(c) == Completion::FrameAfterFilterEnd);
}