// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../broker/JFJochStateMachine.h"
#include "../common/JFJochException.h"

TEST_CASE("JFJochStateMachine_States") {
    Logger logger("JFJochStateMachine_States");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    JFJochState state;
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Inactive);

    REQUIRE_THROWS(state_machine.Start(setup));
    REQUIRE_THROWS(state_machine.Pedestal());

    REQUIRE_NOTHROW(state_machine.Initialize());
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Idle);

    REQUIRE_NOTHROW(state_machine.Deactivate());
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Inactive);
}

TEST_CASE("JFJochStateMachine_State_Pedestal") {
    Logger logger("JFJochStateMachine_State_Pedestal");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    state_machine.DebugOnly_SetState(JFJochState::Pedestal);

    JFJochState state;
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Pedestal);

    REQUIRE_THROWS(state_machine.Start(setup));
    REQUIRE_THROWS(state_machine.Pedestal());
    REQUIRE_THROWS(state_machine.Initialize());

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(500);
    settings.set_use_storage_cells(true);
    REQUIRE_THROWS(state_machine.SetDetectorSettings(settings));
}

TEST_CASE("JFJochStateMachine_State_Measure") {
    Logger logger("JFJochStateMachine_State_Measure");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    state_machine.DebugOnly_SetState(JFJochState::Measuring);

    JFJochState state;
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Measuring);

    REQUIRE_THROWS(state_machine.Start(setup));
    REQUIRE_THROWS(state_machine.Pedestal());
    REQUIRE_THROWS(state_machine.Initialize());

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(500);
    settings.set_use_storage_cells(true);
    REQUIRE_THROWS(state_machine.SetDetectorSettings(settings));
}

TEST_CASE("JFJochStateMachine_State_Error") {
    Logger logger("JFJochStateMachine_State_Error");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    state_machine.DebugOnly_SetState(JFJochState::Error);

    JFJochState state;
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Error);

    REQUIRE_THROWS(state_machine.Start(setup));
    REQUIRE_THROWS(state_machine.Pedestal());

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(500);
    settings.set_use_storage_cells(true);
    REQUIRE_NOTHROW(state_machine.SetDetectorSettings(settings));

    REQUIRE_NOTHROW(state_machine.Initialize());
    REQUIRE_NOTHROW(state = state_machine.GetState());
    REQUIRE(state == JFJochState::Idle);
}


TEST_CASE("JFJochStateMachine_Setup") {
    Logger logger("JJFJochStateMachine_Setup");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DetectorSettings settings, settings_save;
    settings.set_pedestal_g1_frames(-15);
    settings.set_pedestal_g0_frames(2378);
    REQUIRE_THROWS(state_machine.SetDetectorSettings(settings));
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetPedestalG0Frames() != 2378);

    settings.set_pedestal_g2_frames(2800);
    settings.set_pedestal_g1_frames(3000);
    settings.set_pedestal_g0_frames(2378);
    settings.set_use_storage_cells(true);
    settings.set_frame_time_us(600);
    settings.set_count_time_us(247);
    REQUIRE_NOTHROW(state_machine.SetDetectorSettings(settings));
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetPedestalG0Frames() == 2378);
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetPedestalG1Frames() == 3000);
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetPedestalG2Frames() == 2800);
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetStorageCellNumber() == 16);
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetFrameTime() == std::chrono::microseconds(600));
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetFrameCountTime() == std::chrono::microseconds(247));

    REQUIRE_NOTHROW(settings_save = state_machine.GetDetectorSettings());
    REQUIRE(settings_save.use_storage_cells());
    REQUIRE(settings_save.count_time_us() == 247);
}

TEST_CASE("JFJochStateMachine_StorageCells") {
    Logger logger("JFJochBrokerService_StorageCells");
    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(500);
    settings.set_use_storage_cells(true);
    REQUIRE_NOTHROW(state_machine.SetDetectorSettings(settings));
    REQUIRE(state_machine.NotThreadSafe_Experiment().GetStorageCellNumber() == 16);
    REQUIRE_NOTHROW(state_machine.Initialize());

    JFJochProtoBuf::JFCalibration calibration_pbuf;
    REQUIRE_NOTHROW(calibration_pbuf = state_machine.GetCalibration());
    REQUIRE(calibration_pbuf.nstorage_cells() == 16);
}