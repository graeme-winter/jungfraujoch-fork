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

    JFJochProtoBuf::BrokerPersistentSettings settings, settings_save;
    settings.set_pedestal_g1_frames(-15);
    settings.set_pedestal_g0_frames(2378);
    REQUIRE(!broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetPedestalG0Frames() != 2378);

    settings.set_pedestal_g2_frames(2800);
    settings.set_pedestal_g1_frames(3000);
    settings.set_pedestal_g0_frames(2378);
    settings.set_use_storage_cells(true);
    settings.mutable_timing()->set_frame_time_us(600);
    settings.mutable_timing()->set_count_time_us(247);
    REQUIRE(broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetPedestalG0Frames() == 2378);
    REQUIRE(broker.Experiment().GetPedestalG1Frames() == 3000);
    REQUIRE(broker.Experiment().GetPedestalG2Frames() == 2800);
    REQUIRE(broker.Experiment().GetStorageCellNumber() == 16);
    REQUIRE(broker.Experiment().GetFrameTime() == std::chrono::microseconds(600));
    REQUIRE(broker.Experiment().GetFrameCountTime() == std::chrono::microseconds(247));

    REQUIRE(broker.GetSetup(nullptr, nullptr, &settings_save).ok());
    REQUIRE(settings_save.use_storage_cells());
    REQUIRE(!settings_save.has_run_number());
    REQUIRE(settings_save.mutable_timing()->count_time_us() == 247);
}


TEST_CASE("JFJochBrokerService_RunNumber") {
    Logger logger("JFJochBrokerService_Setup");
    JFJochBrokerService broker(logger);

    JFJochProtoBuf::BrokerPersistentSettings settings, settings_save;
    settings.set_run_number(30);
    REQUIRE(broker.Setup(nullptr, &settings, nullptr).ok());
    REQUIRE(broker.Experiment().GetRunNumber() == 30);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_summation(3);
    setup.set_name_pattern("run_number_test");
    setup.set_images_per_trigger(1);
    setup.set_photon_energy_kev(15.0);
    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());

    REQUIRE(broker.GetSetup(nullptr, nullptr, &settings_save).ok());
    REQUIRE(settings_save.has_run_number());
    REQUIRE(settings_save.run_number() == 31);

    setup.set_summation(-1);
    setup.set_name_pattern("run_number_test");
    setup.set_images_per_trigger(1);
    REQUIRE(!broker.Start(nullptr, &setup, nullptr).ok());

    REQUIRE(broker.GetSetup(nullptr, nullptr, &settings_save).ok());
    REQUIRE(settings_save.has_run_number());
    REQUIRE(settings_save.run_number() == 31);

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