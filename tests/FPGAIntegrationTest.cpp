// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <random>

#include <bitshuffle/bitshuffle.h>
#include "../common/PedestalCalcCPU.h"
#include "../fpga/host/HLSSimulatedDevice.h"
#include "FPGAUnitTest.h"
#include "../fpga/host/GenerateOpenCAPIDeviceName.h"

using namespace std::literals::chrono_literals;

TEST_CASE("HLS_C_Simulation_internal_packet_generator", "[FPGA][Full]") {
    const uint16_t nmodules = 4;

    DiffractionExperiment x(1, {nmodules});

    x.Mode(DetectorMode::Raw);
    x.UseInternalPacketGenerator(true).ImagesPerTrigger(4).PedestalG0Frames(0);

    HLSSimulatedDevice test(0, 64);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.OutputStream().size() == 1);

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(test.SaveStatistics(x, device_statistics));
    REQUIRE(device_statistics.bytes_received() == 128 * nmodules * 4 * JUNGFRAU_PACKET_SIZE_BYTES);
    REQUIRE(device_statistics.mask_entries_per_module() == 2); // JUNGFRAU
    REQUIRE(device_statistics.packets_expected_per_module() == 128); // JUNGFRAU
    REQUIRE(device_statistics.packet_mask_size() == nmodules * 4 * 2);
    REQUIRE(device_statistics.packets_received_per_module_size() == nmodules * 4);


    for (int image = 0; image < 4; image++) {

        for (int m = 0; m < nmodules; m++) {
            REQUIRE(device_statistics.packets_received_per_module(image * nmodules + m) == 128);
            for (int i = 0; i < 2; i++) {
                REQUIRE(device_statistics.packet_mask((nmodules * image + m) * 2 + i) == UINT64_MAX);
            }

            auto imageBuf = (uint16_t *) test.GetFrameBuffer(image, m);
            for (int i = 0; i < RAW_MODULE_SIZE; i++)
                REQUIRE(imageBuf[i] == i % 65536);
        }
    }
}

TEST_CASE("HLS_C_Simulation_internal_packet_generator_4KB", "[FPGA][Full]") {
    const uint16_t nmodules = 4;

    DiffractionExperiment x(1, {nmodules});

    x.Mode(DetectorMode::Raw).DetectorType(JFJochProtoBuf::EIGER);
    x.UseInternalPacketGenerator(true).ImagesPerTrigger(4).PedestalG0Frames(0);

    HLSSimulatedDevice test(0, 64);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.OutputStream().size() == 1);

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(test.SaveStatistics(x, device_statistics));
    REQUIRE(device_statistics.bytes_received() == 128 * nmodules * 4 * JUNGFRAU_PACKET_SIZE_BYTES);
    REQUIRE(device_statistics.mask_entries_per_module() == 4); // EIGER
    REQUIRE(device_statistics.packets_expected_per_module() == 256); // EIGER
    REQUIRE(device_statistics.packet_mask_size() == nmodules * 4 * 4);
    REQUIRE(device_statistics.packets_received_per_module_size() == nmodules * 4);


    for (int image = 0; image < 4; image++) {

        for (int m = 0; m < nmodules; m++) {
            REQUIRE(device_statistics.packets_received_per_module(image * nmodules + m) == 256);
            for (int i = 0; i < 4; i++) {
                REQUIRE(device_statistics.packet_mask((nmodules * image + m) * 4 + i) == UINT64_MAX);
            }

            auto imageBuf = (uint16_t *) test.GetFrameBuffer(image, m);
            for (int i = 0; i < RAW_MODULE_SIZE; i++)
                REQUIRE(imageBuf[i] == i % 65536);
        }
    }
}

TEST_CASE("HLS_C_Simulation_internal_packet_generator_custom_frame", "[FPGA][Full]") {
    const uint16_t nmodules = 4;

    DiffractionExperiment x(1, {nmodules});

    std::vector<uint16_t> test_frame(MODULES_INTERNAL_PACKET_GEN * RAW_MODULE_SIZE);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    for (auto &i: test_frame)
        i = dist(g1);

    x.Mode(DetectorMode::Raw);
    x.UseInternalPacketGenerator(true).ImagesPerTrigger(4).PedestalG0Frames(0);

    HLSSimulatedDevice test(0, 64);
    test.SetCustomInternalGeneratorFrame(test_frame);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.OutputStream().size() == 1);

    REQUIRE(test.GetBytesReceived() == 128 * nmodules * 4 * JUNGFRAU_PACKET_SIZE_BYTES);

    for (int image = 0; image < 4; image++) {
        for (int m = 0; m < nmodules; m++) {
            auto imageBuf = (uint16_t *) test.GetFrameBuffer(image, m);
            for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                if (imageBuf[i] != test_frame[(m % MODULES_INTERNAL_PACKET_GEN) * RAW_MODULE_SIZE + i])
                std::cout << m << " " << i << " " << imageBuf[i] << std::endl;
                REQUIRE(imageBuf[i] == test_frame[(m % MODULES_INTERNAL_PACKET_GEN) * RAW_MODULE_SIZE + i]);
            }
        }
    }
}

TEST_CASE("HLS_C_Simulation_check_raw", "[FPGA][Full]") {
    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);

    for (int i = 0; i < 20; i++) {
        LoadBinaryFile("../../tests/test_data/mod5_raw" + std::to_string(i)+".bin", raw_frames.data() + i * RAW_MODULE_SIZE, RAW_MODULE_SIZE);
    }

    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).ImagesPerTrigger(5).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);
    test.CreatePackets(x, 1, 5, 0, raw_frames.data(), true);
    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.GetSlowestHead() == 0);

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 5 * 128 * JUNGFRAU_PACKET_SIZE_BYTES);

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(test.SaveStatistics(x, device_statistics));
    REQUIRE(device_statistics.bytes_received() == 5 * 128 * JUNGFRAU_PACKET_SIZE_BYTES);
    REQUIRE(device_statistics.efficiency() == Approx(0.25));
    REQUIRE(device_statistics.packets_received_per_module_size() == 5 * x.GetModulesNum());

    uint64_t diffs = 0;
    for (int image = 0; image < 5; image++) {
        REQUIRE(device_statistics.packets_received_per_module(image * x.GetModulesNum()) == 128);
        REQUIRE(device_statistics.packets_received_per_module(image * x.GetModulesNum() + 1) == 0);
        REQUIRE(device_statistics.packets_received_per_module(image * x.GetModulesNum() + 3) == 0);
        for (int j = 0; j < RAW_MODULE_SIZE; j++) {
            if (raw_frames[image*RAW_MODULE_SIZE+j] != ((uint16_t *) test.GetFrameBuffer(image, 0))[j]) diffs++;
        }
    }

    REQUIRE(diffs == 0);
}

TEST_CASE("HLS_C_Simulation_check_cancel", "[FPGA][Full]") {
    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).ImagesPerTrigger(5).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);

    REQUIRE_NOTHROW(test.StartAction(x));
    test.ActionAbort();

    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.GetSlowestHead() == 0);

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 0);
}


TEST_CASE("HLS_C_Simulation_check_cancel_conversion", "[FPGA][Full]") {
    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).ImagesPerTrigger(5).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, i));

    REQUIRE_NOTHROW(test.StartAction(x));
    test.ActionAbort();

    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.GetSlowestHead() == 0);

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 0);
}

TEST_CASE("HLS_C_Simulation_check_delay", "[FPGA][Full]") {
    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);

    Completion c;
    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).ImagesPerTrigger(3).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);

    test.CreatePacketJF(x, 1, 0, 0, data, false);
    test.CreatePacketJF(x, 1, 0, 1, data, false);
    test.CreatePacketJF(x, 1, 0, 2, data, false);
    test.CreatePacketJF(x, 1, 0, 3, data, false);

    test.CreatePacketJF(x, 2, 0, 0, data, false);
    test.CreatePacketJF(x, 2, 0, 1, data, false);
    test.CreatePacketJF(x, 2, 0, 2, data, false);
    test.CreatePacketJF(x, 2, 0, 3, data, false);

    test.CreatePacketJF(x, 3, 0, 0, data, false);
    test.CreatePacketJF(x, 3, 0, 1, data, false);
    test.CreatePacketJF(x, 3, 0, 2, data, false);
    test.CreatePacketJF(x, 3, 0, 3, data, false);

    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));

    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE(test.CalculateDelay(0) == 2);
    REQUIRE(test.CalculateDelay(0, 0) == 2);
    REQUIRE_THROWS(test.CalculateDelay(0, 4)); // The module was never collected
    REQUIRE(test.CalculateDelay(1) == 1);
    REQUIRE(test.CalculateDelay(1, 0) == 1);
    REQUIRE(test.CalculateDelay(2) == 0);
    REQUIRE(test.CalculateDelay(2, 0) == 0);
    REQUIRE_THROWS(test.CalculateDelay(67, MAX_MODULES_FPGA));
}

TEST_CASE("HLS_C_Simulation_check_lost_frame_raw", "[FPGA][Full]") {
    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);

    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    for (int i = 0; i < 4096; i++) data[i] = i;
    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).ImagesPerTrigger(3).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);

    test.CreatePacketJF(x, 1, 0, 0, data, false);
    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == JUNGFRAU_PACKET_SIZE_BYTES);

    REQUIRE(test.GetFrameBuffer(0,0)[0] == 0);
    REQUIRE(test.GetFrameBuffer(0,0)[1] == 1);
    REQUIRE(test.GetFrameBuffer(0,0)[4095] == 4095);
    REQUIRE(test.GetFrameBuffer(0,1)[0] == -1);
    REQUIRE(test.GetFrameBuffer(0,1)[RAW_MODULE_SIZE-1] == -1);
}

TEST_CASE("HLS_C_Simulation_check_lost_frame_conversion", "[FPGA][Full]") {
    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);

    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    for (int i = 0; i < 4096; i++) data[i] = i;
    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).ImagesPerTrigger(3).NumTriggers(1);

    HLSSimulatedDevice test(0, 64);

    auto gain_from_file = GainCalibrationFromTestFile();
    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, i));

    test.CreatePacketJF(x, 1, 0, 0, data, false);
    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == JUNGFRAU_PACKET_SIZE_BYTES);

    REQUIRE(test.GetFrameBuffer(0,1)[0] == PIXEL_OUT_LOST);
    REQUIRE(test.GetFrameBuffer(0,1)[RAW_MODULE_SIZE-1] == PIXEL_OUT_LOST);
    REQUIRE(test.GetFrameBuffer(1,0)[0] == PIXEL_OUT_LOST);
    REQUIRE(test.GetFrameBuffer(2,1)[556] == PIXEL_OUT_LOST);
}

TEST_CASE("HLS_C_Simulation_check_single_packet", "[FPGA][Full]") {
    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);

    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).ImagesPerTrigger(3).NumTriggers(1);
    HLSSimulatedDevice test(0, 64);

    test.CreatePacketJF(x, 1, 0, 0, data, false);
    test.CreatePacketJF(x, 1, 64, 0, data, false);
    test.CreatePacketJF(x, 1, 0, 2, data, false);

    test.CreatePacketJF(x, 1, 2, 3, data, false);
    test.CreatePacketJF(x, 1, 3, 3, data, false);

    test.CreatePacketJF(x, 1, 1, 3, data, false);
    test.CreatePacketJF(x, 1, 0, 3, data, false);
    test.CreatePacketJF(x, 1, 64, 3, data, false);

    test.CreatePacketJF(x, 1, 5, 0, data, false);
    test.CreatePacketJF(x, 1, 4, 0, data, false);

    test.CreatePacketJF(x, 1, 67, 1, data, false);
    test.CreatePacketJF(x, 1, 66, 1, data, false);
    test.CreatePacketJF(x, 1, 68, 1, data, false);

    test.CreatePacketJF(x, 3, 1, 0, data, false);
    test.CreatePacketJF(x, 2, 1, 0, data, false);

    test.CreatePacketJF(x, 4, 1, 0, data, false);

    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 16 * JUNGFRAU_PACKET_SIZE_BYTES);

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(test.SaveStatistics(x, device_statistics));
    REQUIRE(device_statistics.packets_received_per_module().size() == 3 * x.GetModulesNum());
    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 0 + 0) == 4);
    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 0 + 1) == 3);
    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 0 + 2) == 1);
    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 0 + 3) == 5);

    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 1 + 0) == 1);
    REQUIRE(device_statistics.packets_received_per_module(x.GetModulesNum() * 2 + 0) == 1);
    REQUIRE(device_statistics.good_packets() == 15);
    REQUIRE(device_statistics.bytes_received() == 16 * JUNGFRAU_PACKET_SIZE_BYTES);
    REQUIRE(device_statistics.packets_expected_per_module() == 128);
    REQUIRE(device_statistics.timestamp().size() == 3*4);
    REQUIRE(device_statistics.timestamp(0) == 0x00FEDCBAL );

    REQUIRE(device_statistics.packet_mask_size() == 4 * 3 * 2);
    REQUIRE(device_statistics.packet_mask(0*4*2+0) == (uint64_t(1)<<0) + (uint64_t(1)<<4) + (uint64_t(1)<<5));
    REQUIRE(device_statistics.packet_mask(0*4*2+1) == (uint64_t(1)<<0));
    REQUIRE(device_statistics.packet_mask(0*4*2+2) == 0);
    REQUIRE(device_statistics.packet_mask(0*4*2+3) == (uint64_t(1)<<2) + (uint64_t(1)<<3) + (uint64_t(1)<<4));
    REQUIRE(device_statistics.packet_mask(1*4*2+0) == (uint64_t(1)<<1));
}

TEST_CASE("HLS_C_Simulation_check_convert_full_range", "[FPGA][Full]") {
    std::vector<uint16_t> data(65536);
    std::vector<double> gain(3 * RAW_MODULE_SIZE);

    JFPedestal pedestal_g0, pedestal_g1, pedestal_g2;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal_g0[i] = 0;
        pedestal_g1[i] = 14500;
        pedestal_g2[i] = 14500;
    }

    for (int i = 0; i < 65536; i++) {
        data[i] = i;
    }

    std::vector<double> energy_values = {6.0, 12.4, 17.7, 5, 4.5, 3.7};

    DiffractionExperiment x(1, {4});
    x.Mode(DetectorMode::Conversion);
    HLSSimulatedDevice test(0, 64);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, i));

    for (const auto energy : energy_values) {
        x.PedestalG0Frames(0).NumTriggers(1).ImagesPerTrigger(1).PhotonEnergy_keV(energy);

        REQUIRE(x.GetPhotonEnergy_keV() == Approx(energy));

        JFCalibration c_in(x);
        c_in.Pedestal(0,0) = pedestal_g0;
        c_in.Pedestal(0,1) = pedestal_g1;
        c_in.Pedestal(0,2) = pedestal_g2;

        test.InitializeCalibration(x, c_in);

        for (int i = 0; i < 16; i++)
            test.CreatePacketJF(x, 1, i, 0, data.data() + i * 4096, true);

        test.CreateFinalPacket(x);

        REQUIRE_NOTHROW(test.StartAction(x));
        REQUIRE_NOTHROW(test.WaitForActionComplete());

        REQUIRE_NOTHROW(test.OutputStream().read());
        REQUIRE(test.OutputStream().size() == 0);
        REQUIRE(test.GetBytesReceived() == 16 * JUNGFRAU_PACKET_SIZE_BYTES);

        double mean_error = CheckConversion(x, c_in, gain_from_file,
                                            data.data(), test.GetFrameBuffer(0,0), 65536);

        REQUIRE(mean_error < 0.5);
    }
}

TEST_CASE("HLS_C_Simulation_internal_packet_generator_convert_full_range", "[FPGA][Full]") {
    double energy = 6.0;

    DiffractionExperiment x(1, {1});
    std::vector<uint16_t> data(RAW_MODULE_SIZE);
    JFPedestal pedestal_g0, pedestal_g1, pedestal_g2;
    std::vector<double> gain(3 * RAW_MODULE_SIZE);

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal_g0[i] = 0 + (i / 65536) * 1000 + 100 * (i % 5);
        pedestal_g1[i] = 14500 - (i / 65536) * 1000 + 100 * (i % 3);
        pedestal_g2[i] = 14500 - + (i / 65536) * 1000;
    }

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        data[i] = i % RAW_MODULE_SIZE;
    }

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).ImagesPerTrigger(1).UseInternalPacketGenerator(true).PhotonEnergy_keV(energy);
    REQUIRE(x.GetPhotonEnergy_keV() == Approx(energy));

    JFCalibration c(x);
    c.Pedestal(0,0) = pedestal_g0;
    c.Pedestal(0,1) = pedestal_g1;
    c.Pedestal(0,2) = pedestal_g2;

    HLSSimulatedDevice test(0, 64);

    auto gain_from_file = GainCalibrationFromTestFile();

    REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, 0));

    REQUIRE_NOTHROW(test.InitializeCalibration(x, c));
    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 128 * JUNGFRAU_PACKET_SIZE_BYTES);

    double mean_error = CheckConversion(x, c, gain_from_file,
                                        data.data(), test.GetFrameBuffer(0,0), RAW_MODULE_SIZE);

    REQUIRE(mean_error < 0.5);
}

TEST_CASE("HLS_C_Simulation_check_2_trigger_convert", "[FPGA][Full]") {
    std::vector<float> pedestal_g0(RAW_MODULE_SIZE), pedestal_g1(RAW_MODULE_SIZE), pedestal_g2(RAW_MODULE_SIZE);

    LoadBinaryFile("../../tests/test_data/mod5_pedeG0.bin", pedestal_g0.data(), RAW_MODULE_SIZE);
    LoadBinaryFile("../../tests/test_data/mod5_pedeG1.bin", pedestal_g1.data(), RAW_MODULE_SIZE);
    LoadBinaryFile("../../tests/test_data/mod5_pedeG2.bin", pedestal_g2.data(), RAW_MODULE_SIZE);

    std::vector<uint16_t> raw_frames(RAW_MODULE_SIZE*20);
    std::vector<float> conv_frames(RAW_MODULE_SIZE*20);

    for (int i = 0; i < 20; i++) {
        LoadBinaryFile("../../tests/test_data/mod5_raw" + std::to_string(i)+".bin", raw_frames.data() + i * RAW_MODULE_SIZE, RAW_MODULE_SIZE);
        LoadBinaryFile("../../tests/test_data/mod5_conv" + std::to_string(i)+".bin", conv_frames.data() + i * RAW_MODULE_SIZE, RAW_MODULE_SIZE);
    }

    DiffractionExperiment x(1,{4});
    uint16_t data[4096];

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(2).ImagesPerTrigger(5);

    HLSSimulatedDevice test(0, 64);

    JFCalibration c(x);
    REQUIRE_NOTHROW(c.Pedestal(0, 0).LoadPedestal(pedestal_g0));
    REQUIRE_NOTHROW(c.Pedestal(0, 1).LoadPedestal(pedestal_g1));
    REQUIRE_NOTHROW(c.Pedestal(0, 2).LoadPedestal(pedestal_g2));

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, i));

    REQUIRE_NOTHROW(test.InitializeCalibration(x, c));

    // send some frames without trigger (to be ignored)
    test.CreatePackets(x, 1, 10, 0, raw_frames.data(), true);
    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));

    REQUIRE(!test.IsDone());

    test.WaitForActionComplete();

    REQUIRE(test.IsDone());

    // address properly aligned
    REQUIRE((uint64_t) test.GetFrameBuffer(0, 0) % 128 == 0);

    REQUIRE(test.GetSlowestHead() == 0);
    REQUIRE(test.GetHead(0) == 9);

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    JFJochProtoBuf::AcquisitionDeviceStatistics device_statistics;
    REQUIRE_NOTHROW(test.SaveStatistics(x, device_statistics));
    REQUIRE(device_statistics.packets_received_per_module().size() == 10*x.GetModulesNum());

    REQUIRE(device_statistics.efficiency() == Approx(0.25));
    REQUIRE(device_statistics.good_packets() == 128*10);
    REQUIRE(device_statistics.packets_expected_per_module() == 128);
    REQUIRE(device_statistics.bytes_received() == 128 * 10 * JUNGFRAU_PACKET_SIZE_BYTES);
    REQUIRE(device_statistics.packet_mask_size() == 10*2*x.GetModulesNum());

    double mean_error = 0.0;
    for (int image = 0; image < 10; image++) {

        for (int m = 0; m < x.GetModulesNum(); m++) {
            REQUIRE(device_statistics.packets_received_per_module(image * x.GetModulesNum() + m) == ((m == 0) ? 128 : 0)); // only one module
            for (int i = 0; i < 2; i++) {
                REQUIRE(device_statistics.packet_mask((image * x.GetModulesNum() + m) * 2 + 1) == ((m == 0) ? UINT64_MAX : 0));
            }
        }

        for (int j = 0; j < RAW_MODULE_SIZE; j++) {
            if ((test.GetFrameBuffer(image, 0)[j] < 30000) && (test.GetFrameBuffer(image, 0)[j] > -30000)) {
                float diff = (conv_frames[image * RAW_MODULE_SIZE + j] - (float) test.GetFrameBuffer(image, 0)[j]);
                mean_error += diff * diff;
            }
        }
    }

    mean_error = sqrt(mean_error/ (10*RAW_MODULE_SIZE));
    std::cout << "Mean error " << mean_error << std::endl;
    REQUIRE(mean_error < 0.30);
}

TEST_CASE("HLS_C_Simulation_check_wrong_packet_size", "[FPGA][Full]") {
    DiffractionExperiment x(1,{1});
    uint16_t data[8192];

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).ImagesPerTrigger(5);

    HLSSimulatedDevice test(0, 64);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(test.LoadModuleGain(gain_from_file, i));

    JFCalibration c(x);
    REQUIRE_NOTHROW(test.InitializeCalibration(x, c));

    // send some frames with wrong size or tuser=1
    test.CreatePacketJF(x, 1, 0, 0, data, true, 0);
    test.CreatePacketJF(x, 1, 1, 0, data, true, -1);
    test.CreatePacketJF(x, 1, 2, 0, data, true, 2);
    test.CreatePacketJF(x, 1, 3, 0, data, true, 0);
    test.CreatePacketJF(x, 1, 4, 0, data, true, -5);
    test.CreatePacketJF(x, 1, 5, 0, data, true, 0, 1);
    test.CreatePacketJF(x, 1, 6, 0, data, true, 0);
    test.CreatePacketJF(x, 1, 7, 0, data, true, 0);
    test.CreatePacketJF(x, 1, 8, 0, data, true, 7);
    test.CreatePacketJF(x, 1, 9, 0, data, true, 100);
    test.CreatePacketJF(x, 1, 10, 0, data, true, 2);
    test.CreatePacketJF(x, 1, 11, 0, data, true, 0);
    test.CreatePacketJF(x, 1, 12, 0, data, true, -80);
    test.CreatePacketJF(x, 1, 13, 0, data, true, 100);
    test.CreatePacketJF(x, 1, 118, 0, data, true, 0);

    test.CreateFinalPacket(x);

    REQUIRE_NOTHROW(test.StartAction(x));

    REQUIRE(!test.IsDone());

    test.WaitForActionComplete();

    REQUIRE(test.IsDone());

    REQUIRE(test.GetBytesReceived() == 6 * JUNGFRAU_PACKET_SIZE_BYTES);
}

TEST_CASE("HLS_DataCollectionFSM","[OpenCAPI]") {
    ActionConfig act_reg;

    STREAM_512 raw0;
    STREAM_512 raw1;

    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr1;

    ap_uint<1> run_data_collection = 0;
    ap_uint<1> cancel_data_collection = 0;
    ap_uint<1> idle_data_collection;

    act_reg.mode = MODE_CONV;

    // state = WAIT_FOR_START

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);
    REQUIRE(idle_data_collection == 1);
    REQUIRE(addr1.empty());
    REQUIRE(raw1.empty());

    run_data_collection = 1;

    // state = WAIT_FOR_START
    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);
    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.empty());
    REQUIRE(raw1.empty());

    // state = WAIT_FOR_START_LOW
    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);
    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.empty());
    REQUIRE(raw1.empty());

    // state = WAIT_FOR_START_LOW

    run_data_collection = 0;

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);
    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.empty());
    REQUIRE(raw1.empty());

    // state = START
    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);

    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.size() == 1);
    REQUIRE(raw1.size() == 1);

    // state = INIT

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);

    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.size() == 1);
    REQUIRE(raw1.size() == 1);

    // state = INIT

    cancel_data_collection = 1;

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);

    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.size() == 1);
    REQUIRE(raw1.size() == 1);

    // state = LAST

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);

    REQUIRE(idle_data_collection == 0);
    REQUIRE(addr1.size() == 2);
    REQUIRE(raw1.size() == 2);

    // state = WAIT_FOR_START

    data_collection_fsm(raw0, raw1,
                        addr0, addr1,
                        run_data_collection,
                        cancel_data_collection,
                        idle_data_collection,
                        act_reg.mode,
                        act_reg.one_over_energy,
                        act_reg.frames_internal_packet_gen,
                        act_reg.nmodules,
                        act_reg.nstorage_cells);

    REQUIRE(idle_data_collection == 1);
    REQUIRE(addr1.size() == 2);
    REQUIRE(raw1.size() == 2);

    auto packet = raw1.read();
    REQUIRE(packet.last == 0);
    REQUIRE(packet.dest == 0);

    packet = raw1.read();
    REQUIRE(packet.last);
    REQUIRE(packet.dest == 0);
    auto addr = addr1.read();

    addr = addr1.read();
    REQUIRE(addr_last_flag(addr));
}

TEST_CASE("OpenCAPIDevice_GenerateOpenCAPIDevice","[OpenCAPI]") {
    REQUIRE(GenerateOpenCAPIDeviceName(4) == "/dev/ocxl/IBM,oc-snap.0004:00:00.1.0");
    REQUIRE(GenerateOpenCAPIDeviceName(5) == "/dev/ocxl/IBM,oc-snap.0005:00:00.1.0");
    REQUIRE(GenerateOpenCAPIDeviceName(6) == "/dev/ocxl/IBM,oc-snap.0006:00:00.1.0");
    REQUIRE(GenerateOpenCAPIDeviceName(7) == "/dev/ocxl/IBM,oc-snap.0007:00:00.1.0");
}

TEST_CASE("HLS_C_Simulation_internal_packet_generator_storage_cell_convert_G0", "[FPGA][Full]") {
    DiffractionExperiment x(1, {2});

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).ImagesPerTrigger(16).UseInternalPacketGenerator(true)
    .PhotonEnergy_keV(10.0).StorageCells(16);

    HLSSimulatedDevice test(0, 64);

    std::vector<double> tmp(3 * RAW_MODULE_SIZE, 50);
    JFModuleGainCalibration gain(tmp);

    REQUIRE_NOTHROW(test.LoadModuleGain(gain, 0));
    REQUIRE_NOTHROW(test.LoadModuleGain(gain, 1));

    std::vector<uint16_t> data(RAW_MODULE_SIZE);
    for (auto &i: data)
        i = 16000;
    REQUIRE_NOTHROW(test.SetCustomInternalGeneratorFrame(data));

    JFCalibration c(x);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < RAW_MODULE_SIZE; j++) {
            c.Pedestal(0, 0, i)[j] = (15 - i) * 500 * 4;
            c.Pedestal(1, 0, i)[j] = i * 1000 * 4;
        }
    }
    REQUIRE_NOTHROW(test.InitializeCalibration(x, c));
    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 2*16*128 * JUNGFRAU_PACKET_SIZE_BYTES);

    for (int i = 0; i < 16; i++) {
        REQUIRE(test.GetFrameBuffer(i,0)[511*764] == 32 - 15 + (i % 16));
        REQUIRE(test.GetFrameBuffer(i,1)[200*145] == 32 - 2 * (i % 16));
    }
}

TEST_CASE("HLS_C_Simulation_internal_packet_generator_storage_cell_convert_G1", "[FPGA][Full]") {
    DiffractionExperiment x(1, {2});

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).ImagesPerTrigger(16).UseInternalPacketGenerator(true)
            .PhotonEnergy_keV(10.0).StorageCells(16);

    HLSSimulatedDevice test(0, 64);

    std::vector<double> tmp(3 * RAW_MODULE_SIZE, -1);
    JFModuleGainCalibration gain(tmp);

    REQUIRE_NOTHROW(test.LoadModuleGain(gain, 0));
    REQUIRE_NOTHROW(test.LoadModuleGain(gain, 1));

    std::vector<uint16_t> data(RAW_MODULE_SIZE);
    for (auto &i: data)
        i = 16384 | 10;
    REQUIRE_NOTHROW(test.SetCustomInternalGeneratorFrame(data));

    JFCalibration c(x);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < RAW_MODULE_SIZE; j++) {
            c.Pedestal(0, 1, i)[j] = (17 - i) * 10 * 4;
            c.Pedestal(1, 1, i)[j] = i * 20 * 4;
        }
    }
    REQUIRE_NOTHROW(test.InitializeCalibration(x, c));
    REQUIRE_NOTHROW(test.StartAction(x));
    REQUIRE_NOTHROW(test.WaitForActionComplete());

    REQUIRE_NOTHROW(test.OutputStream().read());
    REQUIRE(test.OutputStream().size() == 0);

    REQUIRE(test.GetBytesReceived() == 32*128*JUNGFRAU_PACKET_SIZE_BYTES);

    for (int i = 0; i < 16; i++) {
        REQUIRE(test.GetFrameBuffer(i,0)[511*764] == 17 - (i % 16) - 1);
        REQUIRE(test.GetFrameBuffer(i,1)[200*145] == 2 * (i % 16) - 1);
    }
}