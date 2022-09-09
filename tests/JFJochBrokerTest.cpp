// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../broker/JFJochBrokerService.h"
#include "../common/JFJochException.h"

TEST_CASE("JFJochBrokerService_States") {
    Logger logger("JFJochBrokerService_States");
    JFJochBrokerService broker(logger);

    JFJochProtoBuf::BrokerSetup setup;

    JFJochProtoBuf::BrokerStatus status;
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::NOT_INITIALIZED);

    REQUIRE(!broker.Start(nullptr, &setup, nullptr).ok());

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::IDLE);

    REQUIRE(broker.Deactivate(nullptr, nullptr, nullptr).ok());
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::NOT_INITIALIZED);
}

TEST_CASE("JFJochBrokerService_Setup") {
    Logger logger("JFJochBrokerService_Setup");
    JFJochBrokerService broker(logger);

    JFJochProtoBuf::BrokerPersistentSettings settings;
    settings.set_pedestal_g1_frames(-15);
    settings.set_pedestal_g0_frames(2378);
    REQUIRE(!broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetPedestalG0Frames() != 2378);

    settings.set_pedestal_g2_frames(2800);
    settings.set_pedestal_g1_frames(3000);
    settings.set_pedestal_g0_frames(2378);
    settings.set_use_storage_cells(true);
    settings.set_count_time_us(247);
    REQUIRE(broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetPedestalG0Frames() == 2378);
    REQUIRE(broker.Experiment().GetPedestalG1Frames() == 3000);
    REQUIRE(broker.Experiment().GetPedestalG2Frames() == 2800);
    REQUIRE(broker.Experiment().GetStorageCellNumber() == 16);
    REQUIRE(broker.Experiment().GetFrameCountTime() == std::chrono::microseconds(247));
}

TEST_CASE("JFJochBrokerService_StorageCells") {
    Logger logger("JFJochBrokerService_StorageCells");
    JFJochBrokerService broker(logger);

    JFJochProtoBuf::BrokerPersistentSettings settings;
    settings.set_use_storage_cells(true);
    REQUIRE(broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetStorageCellNumber() == 16);
    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());

    JFJochProtoBuf::JFCalibration calibration_pbuf;
    REQUIRE(broker.GetCalibration(nullptr, nullptr, &calibration_pbuf).ok());
    REQUIRE(calibration_pbuf.nstorage_cells() == 16);
}