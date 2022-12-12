// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../fpga/receiver/JFJochReceiverTest.h"
#include "../fpga/host/HLSSimulatedDevice.h"
#include "../common/PedestalCalcCPU.h"
#include "../common/TestImagePusher.h"

using namespace std::literals::chrono_literals;

TEST_CASE("JFJochReceiverTest_Raw", "[JFJochReceiver]") {
    DiffractionExperiment x(1, {4});
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(true)
            .ImagesPerTrigger(100).DataFileCount(16).PhotonEnergy_keV(12.4).Compression(JFJochProtoBuf::NO_COMPRESSION);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        AcquisitionDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        aq_devices.emplace_back(test);
    }

    Logger logger("JFJochReceiverTest_Raw");

    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, false));
    REQUIRE(ret);
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.compressed_ratio() == 1.0);
    REQUIRE(output.compressed_size() == x.GetImageNum() * x.GetPixelDepth() * x.GetPixelsNum());
    REQUIRE(output.max_image_number_sent() == x.GetImageNum() - 1);
    REQUIRE(!output.cancelled());
}

TEST_CASE("JFJochReceiverTest_Conversion", "[JFJochReceiver]") {
    Logger logger("JFJochReceiverTest_Conversion");

    DiffractionExperiment x(1, {2});
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(true)
            .ImagesPerTrigger(32).DataFileCount(16).PhotonEnergy_keV(12.4).Compression(JFJochProtoBuf::BSHUF_ZSTD);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        AcquisitionDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        test->EnableLogging(&logger);
        aq_devices.emplace_back(test);
    }

    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads,  false));
    REQUIRE(ret);
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.images_sent() == x.GetImageNum());

    REQUIRE(output.device_statistics(0).timestamp_size() == 32 * 2);
    REQUIRE(output.device_statistics(0).timestamp(30 * 2) == 0xABCDEF);
    REQUIRE(!output.cancelled());
}

TEST_CASE("JFJochReceiverTest_Conversion_StorageCell", "[JFJochReceiver]") {
    Logger logger("JFJochReceiverTest_Conversion_StorageCell");

    DiffractionExperiment x(1, {2});
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Conversion);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(true)
            .ImagesPerTrigger(32).DataFileCount(16).PhotonEnergy_keV(12.4).Compression(JFJochProtoBuf::BSHUF_ZSTD).StorageCells(16);

    REQUIRE(x.GetImageNum() == 16);
    REQUIRE(x.GetStorageCellNumber() == 16);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        AcquisitionDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        test->EnableLogging(&logger);
        aq_devices.emplace_back(test);
    }

    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads,  false));
    REQUIRE(ret);
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.images_sent() == x.GetImageNum());

    REQUIRE(output.device_statistics(0).timestamp_size() == 16 * 2);
    REQUIRE(output.device_statistics(0).timestamp(15 * 2) == 0xABCDEF);
    REQUIRE(!output.cancelled());
}

TEST_CASE("JFJochReceiverTest_PedestalG1", "[JFJochReceiver]") {

    const uint16_t nthreads = 4;
    size_t nframes = 256;

    std::vector<uint16_t> pedestal_in(RAW_MODULE_SIZE*nframes);

    for (auto &i: pedestal_in) i = 16384 | 14000;

    // Predictable random number generator
    std::mt19937 g1(1900);
    std::normal_distribution<double> distribution(14000, 100);

    for (auto &i: pedestal_in) {
        uint16_t number = distribution(g1);
        if (number < 20) number = 20;
        if (number > 16300) number = 16300;
        i = 16384 | number;
    }

    DiffractionExperiment x(1, {1});
    x.Mode(DetectorMode::PedestalG1).PedestalG0Frames(0)
            .PedestalG1Frames(nframes).NumTriggers(1).UseInternalPacketGenerator(false)
            .ImagesPerTrigger(0).PhotonEnergy_keV(12.4);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        HLSSimulatedDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        test->CreatePackets(x, 1, nframes, 0, pedestal_in.data(), false);
        test->CreateFinalPacket(x);
        aq_devices.emplace_back(test);
    }

    PedestalCalcCPU pc(x, -1);
    for (int i = 0; i < nframes; i++)
        pc.AnalyzeImage(pedestal_in.data() + i * RAW_MODULE_SIZE);

    Logger logger("JFJochReceiverTest_PedestalG1");

    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);

    JFPedestal ref_pedestal;
    pc.Export(ref_pedestal, 0);
    JFPedestal out_pedestal = output.pedestal_result(0);
    REQUIRE(abs(ref_pedestal.Mean() - out_pedestal.Mean()) < 0.1);
    REQUIRE(abs(ref_pedestal.MeanRMS() - out_pedestal.MeanRMS()) < 0.1);
    REQUIRE(out_pedestal.CountMaskedPixels() == 0);
    REQUIRE(!output.cancelled());
}

TEST_CASE("JFJochReceiverTest_PedestalG2_storage_cell", "[JFJochReceiver]") {

    const uint16_t nthreads = 4;
    size_t nframes = 150;

    std::vector<uint16_t> pedestal_in(RAW_MODULE_SIZE*nframes), pedestal_in2(RAW_MODULE_SIZE*nframes);;

    // Predictable random number generator
    std::mt19937 g2(1900);
    std::normal_distribution<double> distribution1(13000, 100);
    std::normal_distribution<double> distribution2(12000, 100);

    for (auto &i: pedestal_in) {
        uint16_t number = distribution1(g2);
        if (number < 20) number = 20;
        if (number > 16300) number = 16300;
        i = 32768 | 16384 | number;
    }

    for (auto &i: pedestal_in2) {
        uint16_t number = distribution2(g2);
        if (number < 20) number = 20;
        if (number > 16300) number = 16300;
        i = 32768 | 16384 | number;
    }
    DiffractionExperiment x(1, {1});
    x.Mode(DetectorMode::PedestalG2).PedestalG0Frames(0)
            .PedestalG2Frames(nframes).NumTriggers(1).UseInternalPacketGenerator(false)
            .ImagesPerTrigger(0).PhotonEnergy_keV(12.4).StorageCells(16);

    REQUIRE(x.GetStorageCellNumber() == 2);
    REQUIRE(x.GetFrameNum() == nframes * 2);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        HLSSimulatedDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        for (int j = 0; j < nframes; j++) {
            test->CreatePackets(x, 2 * j + 1, 1, 0,
                                pedestal_in2.data() + j * RAW_MODULE_SIZE, false);
            test->CreatePackets(x, 2 * j + 2, 1, 0,
                                pedestal_in.data() + j * RAW_MODULE_SIZE, false);
        }
        test->CreateFinalPacket(x);
        aq_devices.emplace_back(test);
    }

    PedestalCalcCPU pc(x, -1);
    for (int i = 0; i < nframes; i++)
        pc.AnalyzeImage(pedestal_in.data() + i * RAW_MODULE_SIZE);

    Logger logger("JFJochReceiverTest_PedestalG1");

    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);

    JFPedestal ref_pedestal;
    pc.Export(ref_pedestal, 0);
    JFPedestal out_pedestal = output.pedestal_result(0);
    REQUIRE(abs(ref_pedestal.Mean() - out_pedestal.Mean()) < 0.1);
    REQUIRE(abs(ref_pedestal.MeanRMS() - out_pedestal.MeanRMS()) < 0.1);
    REQUIRE(out_pedestal.CountMaskedPixels() == 0);
}

TEST_CASE("JFJochReceiverTest_PedestalG0", "[JFJochReceiver]") {
    DiffractionExperiment x(1, {1});
    const uint16_t nthreads = 4;
    size_t nframes = 500;

    std::vector<uint16_t> pedestal_in(RAW_MODULE_SIZE*nframes);

    for (auto &i: pedestal_in) i = 16384 | 14000;

    // Predictable random number generator
    std::mt19937 g1(1900);
    std::normal_distribution<double> distribution(3000, 50);

    for (auto &i: pedestal_in) {
        uint16_t number = distribution(g1);
        if (number < 20) number = 20;
        if (number > 16300) number = 16300;
        i = number;
    }

    x.Mode(DetectorMode::PedestalG0);
    x.PedestalG0Frames(nframes)
            .NumTriggers(1).UseInternalPacketGenerator(false)
            .ImagesPerTrigger(0).PhotonEnergy_keV(12.4);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        HLSSimulatedDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        test->CreatePackets(x, 1, nframes, 0, pedestal_in.data(), false);
        test->CreateFinalPacket(x);
        aq_devices.emplace_back(test);
    }

    PedestalCalcCPU pc(x, -1);
    for (int i = 0; i < nframes; i++)
        pc.AnalyzeImage(pedestal_in.data() + i * RAW_MODULE_SIZE);

    Logger logger("JFJochReceiverTest_PedestalG0");
    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads,false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);

    JFPedestal ref_pedestal;
    pc.Export(ref_pedestal, 0);
    JFPedestal out_pedestal = output.pedestal_result(0);
    REQUIRE(abs(ref_pedestal.Mean() - out_pedestal.Mean()) < 0.1);
    REQUIRE(abs(ref_pedestal.MeanRMS() - out_pedestal.MeanRMS()) < 0.1);
    REQUIRE(out_pedestal.CountMaskedPixels() == 0);
}

TEST_CASE("JFJochReceiverTest_PedestalG0_StorageCell", "[JFJochReceiver]") {
    DiffractionExperiment x(1, {1});
    const uint16_t nthreads = 4;
    size_t nframes = 140;

    std::vector<uint16_t> pedestal_in_0(RAW_MODULE_SIZE);
    std::vector<uint16_t> pedestal_in_1(RAW_MODULE_SIZE);
    std::vector<uint16_t> pedestal_in_2(RAW_MODULE_SIZE);
    std::vector<uint16_t> pedestal_in_3(RAW_MODULE_SIZE);

    for (auto &i: pedestal_in_0) i = 5670;
    for (auto &i: pedestal_in_1) i = 4560;
    for (auto &i: pedestal_in_2) i = 3450;
    for (auto &i: pedestal_in_3) i = 2000;

    x.Mode(DetectorMode::PedestalG0).StorageCells(4);
    x.PedestalG0Frames(nframes)
            .NumTriggers(1).UseInternalPacketGenerator(false)
            .ImagesPerTrigger(0).PhotonEnergy_keV(12.4);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    HLSSimulatedDevice *test;
    test = new HLSSimulatedDevice(0, 64);
    for (int i = 0; i < nframes; i++) {
        test->CreatePackets(x, i*4+1, 1, 0, pedestal_in_0.data(), false);
        test->CreatePackets(x, i*4+2, 1, 0, pedestal_in_1.data(), false);
        test->CreatePackets(x, i*4+3, 1, 0, pedestal_in_2.data(), false);
        test->CreatePackets(x, i*4+4, 1, 0, pedestal_in_3.data(), false);
    }
    test->CreateFinalPacket(x);
    aq_devices.emplace_back(test);

    Logger logger("JFJochReceiverTest_PedestalG0_StorageCell");
    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.device_statistics(0).good_packets() == nframes * 4 * 128);
    REQUIRE(output.pedestal_result_size() == 4);
    JFPedestal out_pedestal_0 = output.pedestal_result(0);
    JFPedestal out_pedestal_1 = output.pedestal_result(1);
    JFPedestal out_pedestal_2 = output.pedestal_result(2);
    JFPedestal out_pedestal_3 = output.pedestal_result(3);

    REQUIRE(out_pedestal_0.Mean() == Approx(5670));
    REQUIRE(out_pedestal_1.Mean() == Approx(4560));
    REQUIRE(out_pedestal_2.Mean() == Approx(3450));
    REQUIRE(out_pedestal_3.Mean() == Approx(2000));
}

TEST_CASE("JFJochReceiverTest_PedestalG1_NoFrames", "[JFJochReceiver]") {
    DiffractionExperiment x(1, {1});
    const uint16_t nthreads = 4;
    size_t nframes = 256;

    std::vector<uint16_t> pedestal_in(RAW_MODULE_SIZE*nframes);

    for (auto &i: pedestal_in) i = 16384 | 14000;

    x.Mode(DetectorMode::PedestalG1);
    x.PedestalG0Frames(0).PedestalG1Frames(256).NumTriggers(1)
            .UseInternalPacketGenerator(false).ImagesPerTrigger(0).PhotonEnergy_keV(12.4);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        HLSSimulatedDevice *test;
        test = new HLSSimulatedDevice(i, nframes + 8);
        test->CreatePackets(x, 1, PEDESTAL_WINDOW_SIZE - 1, 0, pedestal_in.data(), false);
        test->CreateFinalPacket(x);
        aq_devices.emplace_back(test);
    }

    Logger logger("JFJochReceiverTest_PedestalG1_NoFrames");
    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads,  false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == 0);
    REQUIRE(output.efficiency() < 1.0);

    JFPedestal out_pedestal = output.pedestal_result(0);
    REQUIRE(out_pedestal.CountMaskedPixels() == RAW_MODULE_SIZE);
}

TEST_CASE("JFJochReceiverTest_PacketLost_Raw", "[JFJochReceiver]") {
    DiffractionExperiment x(1, {2});
    const uint16_t nthreads = 4;

    std::vector<uint16_t> frame_in(RAW_MODULE_SIZE);
    for (auto &i: frame_in) i = 776;

    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1)
            .UseInternalPacketGenerator(false).ImagesPerTrigger(4).PhotonEnergy_keV(12.4);

    auto gain_from_file = GainCalibrationFromTestFile();

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        auto test = new HLSSimulatedDevice(i, 64);
        test->CreatePackets(x, 1, 1, 0, frame_in.data(), false);
        test->CreatePackets(x, 1, 1, 1, frame_in.data(), false);
        test->CreatePackets(x, 2, 1, 0, frame_in.data(), false);
        test->CreatePackets(x, 2, 1, 1, frame_in.data(), false);
        test->CreatePackets(x, 3, 1, 0, frame_in.data(), false);
        test->CreatePackets(x, 3, 1, 1, frame_in.data(), false);

        for (int j = 0; j < 127; j++)
            test->CreatePacketJF(x, 4, j, 0, frame_in.data() + (i * 128 + j) * 4096, false);
        test->CreatePackets(x, 4, 1, 1, frame_in.data(), false);
        test->CreateFinalPacket(x);

        for (int m = 0; m < x.GetModulesNum(i); m++)
            REQUIRE_NOTHROW(test->LoadModuleGain(gain_from_file, m));

        aq_devices.emplace_back(test);
    }
    Logger logger("JFJochReceiverTest_PacketLost_Raw");

    JFCalibration calib(x);

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    TestImagePusher pusher(x.GetImageNum() - 1);
    auto receiver_out = RunJFJochReceiverTest(tmp_devices, pusher, x, logger, calib, nthreads, false);

    const auto image = pusher.GetImage();

    REQUIRE(receiver_out.device_statistics(0).good_packets() == 8*128-1);
    REQUIRE(image.size() == 2 * RAW_MODULE_SIZE * sizeof(uint16_t));
    REQUIRE(pusher.GetCounter() == x.GetImageNum());

    auto raw = (const uint16_t *) (image.data());
    CHECK(raw[0] == UINT16_MAX);
    CHECK(raw[256*1024+315] == UINT16_MAX);
    CHECK(raw[RAW_MODULE_SIZE-4095] == UINT16_MAX);
    CHECK(raw[RAW_MODULE_SIZE-1]    == UINT16_MAX);

    CHECK(raw[RAW_MODULE_SIZE+0] == 776);
    CHECK(raw[RAW_MODULE_SIZE+256*1024+315] == 776);
    CHECK(raw[2*RAW_MODULE_SIZE-1] == 776);

    REQUIRE(receiver_out.efficiency() < 1.0);
    REQUIRE(receiver_out.images_sent() == x.GetImageNum());
}
