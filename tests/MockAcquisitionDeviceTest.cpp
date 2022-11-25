// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../fpga/host/MockAcquisitionDevice.h"

TEST_CASE("MockAcquisitionDevice") {
    std::vector<uint16_t> module_data(RAW_MODULE_SIZE, 765);

    DiffractionExperiment experiment(2, {4,4});

    experiment.NumTriggers(1).ImagesPerTrigger(5);
    MockAcquisitionDevice device(0, 128);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int m = 0; m < experiment.GetModulesNum(0); m++)
        device.LoadModuleGain(gain_from_file, m);

    device.StartAction(experiment);

    device.AddModule(1,0,module_data.data());
    device.AddModule(1,1,module_data.data());
    device.AddModule(4,0,module_data.data());
    device.AddModule(5,1,module_data.data());

    device.Terminate();
    device.WaitForActionComplete();

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(device.SaveStatistics(experiment, device_statistics));
    REQUIRE(device_statistics.packets_received_per_module_size() == 5 * 4);
    REQUIRE(device_statistics.packets_received_per_module(4*0+0) == 128);
    REQUIRE(device_statistics.packets_received_per_module(4*0+1) == 128);
    REQUIRE(device_statistics.packets_received_per_module(4*1+0) == 0);
    REQUIRE(device_statistics.packets_received_per_module(4*2+5) == 0);
    REQUIRE(device_statistics.packets_received_per_module(4*3+7) == 0);
    REQUIRE(device_statistics.packets_received_per_module(4*3+0) == 128);
    REQUIRE(device_statistics.packets_received_per_module(4*4+1) == 128);

    REQUIRE(memcmp(device.GetFrameBuffer(0,0), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(0,1), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(3,0), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(4,1), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
}

