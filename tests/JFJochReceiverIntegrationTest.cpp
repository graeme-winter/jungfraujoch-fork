// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../fpga/host/JFJochReceiverTest.h"
#include "../fpga/host/HLSSimulatedDevice.h"
#include "../common/PedestalCalcCPU.h"

using namespace std::literals::chrono_literals;

TEST_CASE("JFJochReceiverTest_Raw", "[JFJochReceiver]") {
    DiffractionExperiment x;
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Raw);
    x.DataStreamModuleSize(1, {4});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(true)
            .ImagesPerTrigger(100).ImagesPerFile(10).PhotonEnergy_keV(12.4).Compression(CompressionAlgorithm::None);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        AcquisitionDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        aq_devices.emplace_back(test);
    }

    Logger logger("JFJochReceiverTest_Raw");

    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, "../../", false));
    REQUIRE(ret);
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.compressed_ratio() == 1.0);
    REQUIRE(output.compressed_size() == x.GetImageNum() * x.GetPixelDepth() * x.GetPixelsNum());
    REQUIRE(output.max_image_number_sent() == x.GetImageNum() - 1);
}

TEST_CASE("JFJochReceiverTest_Conversion", "[JFJochReceiver]") {
    DiffractionExperiment x;
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Conversion);
    x.DataStreamModuleSize(1, {2});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(true)
            .ImagesPerTrigger(32).PhotonEnergy_keV(12.4).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        AcquisitionDevice *test;
        test = new HLSSimulatedDevice(i, 64);
        aq_devices.emplace_back(test);
    }

    Logger logger("JFJochReceiverTest_Conversion");
    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, "../../", false));
    REQUIRE(ret);
    REQUIRE(output.efficiency() == 1.0);
    REQUIRE(output.images_sent() == x.GetImageNum());
}

TEST_CASE("JFJochReceiverTest_PedestalG1", "[JFJochReceiver]") {
    DiffractionExperiment x;
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

    x.Mode(DetectorMode::PedestalG1);
    x.DataStreamModuleSize(1, {1});
    x.DataStreamModuleSize(1, {1}).Mode(DetectorMode::PedestalG1).PedestalG0Frames(0)
            .PedestalG1Frames(nframes).NumTriggers(0).UseInternalPacketGenerator(false)
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

    JungfrauCalibration calib(x);
    pc.Export(calib, 0);

    Logger logger("JFJochReceiverTest_PedestalG1");

    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, "../../", false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);

    JungfrauCalibration out_calib = output.calibration();
    REQUIRE(abs(output.calibration().module_statistics(0).pedestal_g1_mean() - calib.MeanPedestal(1,0)) < 0.1);
    REQUIRE(abs(output.calibration().module_statistics(0).pedestal_rms_g1_mean() - calib.MeanPedestalRMS(1,0)) < 0.1);
    REQUIRE(output.calibration().module_statistics(0).masked_pixels() == 0);

}


TEST_CASE("JFJochReceiverTest_PedestalG0", "[JFJochReceiver]") {
    DiffractionExperiment x;
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
    x.DataStreamModuleSize(1, {1}).PedestalG0Frames(nframes)
            .NumTriggers(0).UseInternalPacketGenerator(false)
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

    JungfrauCalibration calib(x);
    pc.Export(calib, 0);

    Logger logger("JFJochReceiverTest_PedestalG0");
    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, "../../", false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == x.GetImageNum());
    REQUIRE(output.efficiency() == 1.0);

    JungfrauCalibration out_calib = output.calibration();

    REQUIRE(output.calibration().module_statistics(0).pedestal_g0_mean() == Approx(calib.MeanPedestal(0,0)));
    REQUIRE(output.calibration().module_statistics(0).pedestal_rms_g0_mean() == Approx(calib.MeanPedestalRMS(0,0)));
    REQUIRE(output.calibration().module_statistics(0).masked_pixels() == 0);

}

TEST_CASE("JFJochReceiverTest_PedestalG1_NoFrames", "[JFJochReceiver]") {
    DiffractionExperiment x;
    const uint16_t nthreads = 4;
    size_t nframes = 256;

    std::vector<uint16_t> pedestal_in(RAW_MODULE_SIZE*nframes);

    for (auto &i: pedestal_in) i = 16384 | 14000;

    x.DataStreamModuleSize(1, {1});
    x.Mode(DetectorMode::PedestalG1);
    x.PedestalG0Frames(0).PedestalG1Frames(256).NumTriggers(0)
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
    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    REQUIRE_NOTHROW(ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, "../../", false));
    REQUIRE(ret);
    REQUIRE(output.images_sent() == 0);
    REQUIRE(output.efficiency() < 1.0);

    REQUIRE(output.calibration().module_statistics(0).masked_pixels() == RAW_MODULE_SIZE);
}

TEST_CASE("JFJochReceiverTest_PacketLost_Raw", "[JFJochReceiver]") {
    DiffractionExperiment x;
    const uint16_t nthreads = 4;

    std::vector<uint16_t> frame_in(RAW_MODULE_SIZE);
    for (auto &i: frame_in) i = 776;

    x.Mode(DetectorMode::Raw);
    x.DataStreamModuleSize(1, {2});
    x.PedestalG0Frames(0).NumTriggers(0)
            .UseInternalPacketGenerator(false).ImagesPerTrigger(4).PhotonEnergy_keV(12.4);

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
            test->CreatePacket(x, 4, j, 0, frame_in.data() + (i * 128 + j) * 4096, false);
        test->CreatePackets(x, 4, 1, 1, frame_in.data(), false);
        test->CreateFinalPacket(x);

        for (int m = 0; m < x.GetModulesNum(i); m++)
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

        aq_devices.emplace_back(test);
    }
    Logger logger("JFJochReceiverTest_PacketLost_Raw");
    CommunicationBuffer buffer(256*1024*1024);
    JungfrauCalibration calib(x);

    size_t nframes = 0;
    std::vector<uint8_t> image;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    auto receiver_out = RunJFJochReceiverTest(tmp_devices, x, logger, calib, nframes, image, nthreads, false);

    REQUIRE(receiver_out.device_statistics(0).good_packets() == 8*128-1);
    REQUIRE(image.size() == 2 * RAW_MODULE_SIZE * sizeof(uint16_t));
    REQUIRE(nframes == x.GetImageNum());

    auto raw = (const uint16_t *) (image.data());
    CHECK(raw[0] == 776);
    CHECK(raw[256*1024+315] == 776);
    CHECK(raw[RAW_MODULE_SIZE-4095] == UINT16_MAX);
    CHECK(raw[RAW_MODULE_SIZE-1]    == UINT16_MAX);

    CHECK(raw[RAW_MODULE_SIZE+0] == 776);
    CHECK(raw[RAW_MODULE_SIZE+256*1024+315] == 776);
    CHECK(raw[2*RAW_MODULE_SIZE-1] == 776);

    REQUIRE(receiver_out.efficiency() < 1.0);
    REQUIRE(receiver_out.images_sent() == x.GetImageNum());
}

TEST_CASE("JFJochReceiverTest_Raw_TimeResolved", "[JFJochReceiver]") {
    DiffractionExperiment x;
    const uint16_t nthreads = 4;

    x.Mode(DetectorMode::Raw);
    x.DataStreamModuleSize(1, {1});
    x.NumTriggers(5)
            .UseInternalPacketGenerator(false).ImagesPerTrigger(25).PhotonEnergy_keV(12.4)
            .TimeResolvedMode(true).PedestalG0Frames(0);

    std::mt19937 g1(9999);
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    std::vector<uint16_t> frame_in(RAW_MODULE_SIZE * x.GetImageNum());
    for (auto &i: frame_in) i = dist(g1);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        auto test = new HLSSimulatedDevice(i, 64);
        test->CreatePackets(x, 1, x.GetImageNum(), 0, frame_in.data(), false);
        test->CreateFinalPacket(x);

        for (int m = 0; m < x.GetModulesNum(i); m++)
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

        aq_devices.emplace_back(test);
    }
    Logger logger("JFJochReceiverTest_Raw_TimeResolved");
    CommunicationBuffer buffer(256*1024*1024);
    JungfrauCalibration calib(x);

    size_t nframes = 0;
    std::vector<uint8_t> image;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    auto receiver_output = RunJFJochReceiverTest(tmp_devices, x, logger, calib, nframes, image,
                          nthreads, false);

    REQUIRE(nframes == x.GetImageNum());

    REQUIRE(image.size() == RAW_MODULE_SIZE * sizeof(uint16_t));
    REQUIRE(memcmp(image.data(),
                   frame_in.data() + RAW_MODULE_SIZE * (x.GetImageNum() - 1),
                   RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);

    REQUIRE(receiver_output.efficiency() == 1.0);
    REQUIRE(receiver_output.images_sent() == x.GetImageNum());
    REQUIRE(receiver_output.device_statistics(0).good_packets() == x.GetImageNum()*128);
}