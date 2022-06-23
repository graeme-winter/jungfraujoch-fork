// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../fpga/host/MockAcquisitionDevice.h"

TEST_CASE("MockAcquisitionDevice") {
    std::vector<uint16_t> module_data(RAW_MODULE_SIZE, 765);

    DiffractionExperiment experiment(2, {4,4});

    experiment.NumTriggers(0).ImagesPerTrigger(5);
    MockAcquisitionDevice device(0, 128);

    for (int m = 0; m < experiment.GetModulesNum(0); m++)
        device.LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

    device.StartAction(experiment);

    device.AddModule(1,0,module_data.data());
    device.AddModule(1,1,module_data.data());
    device.AddModule(4,0,module_data.data());
    device.AddModule(5,1,module_data.data());

    device.Terminate();
    device.WaitForActionComplete();

    REQUIRE(device.GetPacketCount(0,0) == 128);
    REQUIRE(device.GetPacketCount(0,1) == 128);
    REQUIRE(device.GetPacketCount(3,0) == 128);
    REQUIRE(device.GetPacketCount(4,1) == 128);

    REQUIRE(memcmp(device.GetFrameBuffer(0,0), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(0,1), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(3,0), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    REQUIRE(memcmp(device.GetFrameBuffer(4,1), module_data.data(),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
}

