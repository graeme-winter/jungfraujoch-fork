// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <google/protobuf/util/message_differencer.h>

#include "../common/DiffractionExperiment.h"
#include "../compression/JFJochCompressor.h"
#include "../common/NetworkAddressConvert.h"

using namespace std::literals::chrono_literals;

TEST_CASE("DiffractionExperiment_FilePath","[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_NOTHROW(x.FilePrefix("x"));
    REQUIRE(x.GetFilePrefix() == "x");

    // trailing / needs to be ignored
    REQUIRE_THROWS(x.FilePrefix("/x"));
    REQUIRE_THROWS(x.FilePrefix("../x"));
    REQUIRE_THROWS(x.FilePrefix("x/../y"));

    x.Mode(DetectorMode::PedestalG0);
    REQUIRE(x.GetFilePrefix() == "x");
    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetFilePrefix() == "x");
    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GetFilePrefix() == "x");

    REQUIRE_NOTHROW(x.FilePrefix("x6_master.h5"));
    REQUIRE(x.GetFilePrefix() == "x6");

    REQUIRE_NOTHROW(x.FilePrefix("long_name/directory/file_master.h5"));
    REQUIRE(x.GetFilePrefix() == "long_name/directory/file");

    REQUIRE_NOTHROW(x.FilePrefix("_master.h5"));
    REQUIRE(x.GetFilePrefix() == "_master.h5");
}

TEST_CASE("DiffractionExperiment_Compression_Raw","[DiffractionExperiment]") {
    DiffractionExperiment x;
    for (auto i: {DetectorMode::Raw, DetectorMode::PedestalG0, DetectorMode::PedestalG1, DetectorMode::PedestalG2}) {
        x.Mode(i).Compression(JFJochProtoBuf::BSHUF_ZSTD);
        REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::NO_COMPRESSION);
    }
}

TEST_CASE("DiffractionExperiment_Compression","[DiffractionExperiment]") {
    DiffractionExperiment x;

    // Compression
    x.Compression(JFJochProtoBuf::BSHUF_LZ4);
    REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::BSHUF_LZ4);
    REQUIRE(x.GetCompressionAlgorithmEnum() == CompressionAlgorithm::BSHUF_LZ4);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionAlgorithmEnum() == CompressionAlgorithm::BSHUF_ZSTD);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD_RLE);
    REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::BSHUF_ZSTD_RLE);
    REQUIRE(x.GetCompressionAlgorithmEnum() == CompressionAlgorithm::BSHUF_ZSTD_RLE);

    x.Compression(JFJochProtoBuf::NO_COMPRESSION);
    REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::NO_COMPRESSION);
    REQUIRE(x.GetCompressionAlgorithmEnum() == CompressionAlgorithm::NO_COMPRESSION);
}

TEST_CASE("DiffractionExperiment_Timing","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.Mode(DetectorMode::Conversion);
    // Timing and frame count
    x.PedestalG0Frames(1000).ImagesPerTrigger(1000).NumTriggers(1).FrameTime(1000us).Summation(1);

    // Frame count analysis

    REQUIRE(x.GetFrameNum() == 1000);
    REQUIRE(x.GetFrameTime().count() == 1000);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetPixelDepth() == 2);
    REQUIRE(x.GetOverflow() == INT16_MAX - 4);
    REQUIRE(x.GetUnderflow() == INT16_MIN + 4);
    REQUIRE(x.GetFrameNumPerTrigger() == 1000);
    x.PedestalG0Frames(1000).PedestalG1Frames(2000).PedestalG2Frames(3000).NumTriggers(1)
        .ImagesPerTrigger(1000).FrameTime(1000us).Summation(6);

    REQUIRE(x.GetImageNum() == 1000);
    REQUIRE(x.GetImageNumPerTrigger() == 1000);
    REQUIRE(x.GetFrameNum() == 6000);
    REQUIRE(x.GetFrameTime().count() == 1000);
    REQUIRE(x.GetFrameCountTime().count() == 1000 - READOUT_TIME_IN_US);
    REQUIRE(x.GetSummation() == 6);
    REQUIRE(x.GetImageTime().count() == 6*1000);
    REQUIRE(x.GetImageCountTime().count() == 6 * (1000 - READOUT_TIME_IN_US));
    REQUIRE(x.GetFrameNumPerTrigger() == 6000);
    REQUIRE(x.GetPedestalG0Frames() == 1000);
    REQUIRE(x.GetPedestalG1Frames() == 2000);
    REQUIRE(x.GetPedestalG2Frames() == 3000);
    REQUIRE(x.GetPixelDepth() == 4);
    REQUIRE(x.GetOverflow() == 8*INT16_MAX - 4);
    REQUIRE(x.GetUnderflow() == 8*INT16_MIN + 4);

    x.NumTriggers(2);
    REQUIRE(x.GetFrameNum() == 12000);
    REQUIRE(x.GetNumTriggers() == 2);
    REQUIRE(x.GetImageNum() == 2000);
    REQUIRE(x.GetImageNumPerTrigger() == 1000);
    REQUIRE(x.GetFrameNumPerTrigger() == 6000);

    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetFrameNum() == 2000);
    REQUIRE(x.GetImageNum() == 2000);
    REQUIRE(x.GetImageNumPerTrigger() == 1000);

    x.Mode(DetectorMode::PedestalG0);

    REQUIRE(x.GetFrameTime().count() == 1000);
    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG0);
    REQUIRE(x.GetFrameNum() == 1000);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetImageNumPerTrigger() == 0);

    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG1);
    REQUIRE(x.GetFrameNum() == 2000);
    REQUIRE(x.GetFrameTime().count() == 10000);
    REQUIRE(x.GetSummation() == 1);

    x.PedestalG1G2FrameTime(8345us);
    x.Mode(DetectorMode::PedestalG2);

    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG2);
    REQUIRE(x.GetFrameNum() == 3000);
    REQUIRE(x.GetFrameTime().count() == 8345);
    REQUIRE(x.GetSummation() == 1);
}

TEST_CASE("DiffractionExperiment_UnitCell","[DiffractionExperiment]") {
    DiffractionExperiment x;
    REQUIRE(!x.HasUnitCell());

    JFJochProtoBuf::UnitCell cell;
    cell.set_a(10);
    cell.set_b(20);
    cell.set_c(30);
    cell.set_alpha(90);
    cell.set_beta(90);
    cell.set_gamma(90);
    REQUIRE_NOTHROW(x.SetUnitCell(cell));

    REQUIRE(x.HasUnitCell());
    REQUIRE(x.GetUnitCell().c() == 30);
    REQUIRE_NOTHROW(x.SetUnitCell());
    REQUIRE(!x.HasUnitCell());
    REQUIRE_NOTHROW(x.SetUnitCell(cell));

    JFJochProtoBuf::JungfraujochSettings settings = x;

    DiffractionExperiment y(settings);

    REQUIRE(google::protobuf::util::MessageDifferencer::Equivalent(y.GetUnitCell(), x.GetUnitCell()));
}

TEST_CASE("DiffractionExperiment_IPv4Address","[DiffractionExperiment]") {
    DiffractionExperiment x(1, {4, 4, 4});
    x.Mode(DetectorMode::Conversion);

    uint32_t ndatastreams = 3;

    REQUIRE(x.GetDestIPv4Address(0) == 0x0132010a);
    REQUIRE(x.GetDestIPv4Address(1) == 0x0232010a);
    REQUIRE(x.GetDestIPv4Address(2) == 0x0332010a);
    REQUIRE(x.GetSrcIPv4Address(0) == 0x0132010a + ((ndatastreams+2 + 0) << 24));
    REQUIRE(x.GetSrcIPv4Address(23) == 0x0132010a + ((ndatastreams+2 + 23) << 24));
    REQUIRE_THROWS(x.GetDestIPv4Address(3));
    REQUIRE_THROWS(x.GetSrcIPv4Address(24));

    REQUIRE_THROWS(x.BaseIPv4Address("456"));
    REQUIRE_THROWS(x.BaseIPv4Address("257.1.1.1"));
    REQUIRE_THROWS(x.BaseIPv4Address("257.1.1.1111"));
    REQUIRE_THROWS(x.BaseIPv4Address("64.1.124.129")); // Last part above 128

    REQUIRE_NOTHROW(x.BaseIPv4Address("64.1.124.111"));
    REQUIRE(x.GetDestIPv4Address(0) == 0x6f7c0140u);
    REQUIRE(x.GetDestIPv4Address(1) == 0x707c0140u);
    REQUIRE(x.GetDestIPv4Address(2) == 0x717c0140u);
    REQUIRE(x.GetSrcIPv4Address(12) == 0x6f7c0140u + ((ndatastreams+2+12) << 24));

    REQUIRE(IPv4AddressToStr(x.GetDestIPv4Address(2)) == "64.1.124.113");
}

TEST_CASE("IPv4AddressToStr","") {
    REQUIRE(IPv4AddressToStr(0x0f32010a) == "10.1.50.15");
}

TEST_CASE("IPv4AddressFromStr","") {
    REQUIRE(IPv4AddressFromStr("10.1.50.15") == 0x0f32010a);
    REQUIRE_THROWS(IPv4AddressFromStr("256.257.0.1"));
    REQUIRE_THROWS(IPv4AddressFromStr("ff.ff.ff.1"));
}

TEST_CASE("MacAddressToStr","") {
    REQUIRE(MacAddressToStr(0xF1EEDDCCBBAA) == "aa:bb:cc:dd:ee:f1");
    REQUIRE(MacAddressToStr(0x0000DDCCBB00) == "00:bb:cc:dd:00:00");
}

TEST_CASE("MacAddressFromStr","") {
    REQUIRE(MacAddressFromStr("aa:bb:cc:dd:ee:f1") == 0xF1EEDDCCBBAA);
    REQUIRE(MacAddressFromStr("11:22:33:44:55:66") == 0x665544332211);
    REQUIRE_THROWS(MacAddressFromStr("11:22:33:44:55:66:77"));
    REQUIRE_THROWS(MacAddressFromStr("11:22:33:44:55"));
    REQUIRE_THROWS(MacAddressFromStr("456:22:33:44:55"));
    REQUIRE_THROWS(MacAddressFromStr("xy:22:33:44:55"));
}

TEST_CASE("DiffractionExperiment_UDPAddress","[DiffractionExperiment]") {
    DiffractionExperiment x(1, {4, 4, 4});
    x.Mode(DetectorMode::Conversion);

    REQUIRE(x.GetDestUDPPort(0, 0) % 64 == 0);
    REQUIRE(x.GetDestUDPPort(0, 1) % 64 == 2);
    REQUIRE(x.GetDestUDPPort(0, 2) % 64 == 4);

    REQUIRE_THROWS(x.GetDestUDPPort(0, 4));

    REQUIRE(x.GetDestUDPPort(2, 0) % 64 == 0);
    REQUIRE(x.GetDestUDPPort(2, 1) % 64 == 2);
    REQUIRE(x.GetDestUDPPort(2, 2) % 64 == 4);

    REQUIRE_THROWS(x.GetDestUDPPort(3, 0));

    REQUIRE_THROWS(x.BaseUDPPort(-1));
    REQUIRE_THROWS(x.BaseUDPPort(65536));
    REQUIRE_THROWS(x.BaseUDPPort(64*93+1));

    REQUIRE_NOTHROW(x.BaseUDPPort(64*93));
    REQUIRE(x.GetDestUDPPort(0, 0) == 64*93);
    REQUIRE(x.GetDestUDPPort(0, 2) == 64*93 + 4);

}

TEST_CASE("DiffractionExperiment_DetectorGeometry","[DiffractionExperiment]") {
    DiffractionExperiment x(1, {4, 4, 6, 6}); // 10M configuration #1 - via constructor

    x.Mode(DetectorMode::Conversion);

    REQUIRE(x.GetDataStreamsNum() == 4);

    REQUIRE(x.GetXPixelsNum() == 1030);
    REQUIRE(x.GetYPixelsNum() == 514 * 20);

    REQUIRE(x.GetModulesNum() == 20);
    REQUIRE(x.GetModulesNum(TASK_NO_DATA_STREAM) == 20);
    REQUIRE(x.GetModulesNum(0) == 4);
    REQUIRE(x.GetModulesNum(2) == 6);

    REQUIRE(x.GetFirstModuleOfDataStream(TASK_NO_DATA_STREAM) == 0);
    REQUIRE(x.GetFirstModuleOfDataStream(0) == 0);
    REQUIRE(x.GetFirstModuleOfDataStream(1) == 4);
    REQUIRE(x.GetFirstModuleOfDataStream(2) == 8);
    REQUIRE(x.GetFirstModuleOfDataStream(3) == 14);

    REQUIRE(x.GetPixelsNum() == 20 * 514 * 1030);

    x.Mode(DetectorMode::Raw);

    REQUIRE(x.GetPixelsNum() == 20 * 512 * 1024);
    REQUIRE(x.GetXPixelsNum() == 1024);
    REQUIRE(x.GetYPixelsNum() == 512 * 20);

    x.Mode(DetectorMode::Conversion);

    x = DiffractionExperiment(2, {4, 4, 6, 6}, 0, 0, true); // 10M configuration #2

    REQUIRE(x.GetPixelsNum() == 1030 * 514 * 20);
    REQUIRE(x.GetXPixelsNum() == 1030 * 2);
    REQUIRE(x.GetYPixelsNum() == 514 * 20 / 2);

    REQUIRE(x.GetPixel0OfModule(0) == 18 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2);
    REQUIRE(x.GetPixel0OfModule(1) == 18 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2 + CONVERTED_MODULE_COLS);

    REQUIRE(x.GetPixel0OfModule(12) == 6 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2);
    REQUIRE(x.GetPixel0OfModule(13) == 6 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2 + CONVERTED_MODULE_COLS);

    REQUIRE(x.GetPixel0OfModule(14) == 4 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2);
    REQUIRE(x.GetPixel0OfModule(15) == 4 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2 + CONVERTED_MODULE_COLS);

    REQUIRE(x.GetPixel0OfModule(16) == 2 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2);
    REQUIRE(x.GetPixel0OfModule(17) == 2 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2 + CONVERTED_MODULE_COLS);

    REQUIRE(x.GetPixel0OfModule(18) == 0 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2);
    REQUIRE(x.GetPixel0OfModule(19) == 0 * CONVERTED_MODULE_SIZE + 513 * CONVERTED_MODULE_COLS * 2 + CONVERTED_MODULE_COLS);
    REQUIRE_THROWS(x.GetPixel0OfModule(20));

    x.Mode(DetectorMode::Raw);

    REQUIRE(x.GetPixelsNum() == 1024 * 512 * 20);
    REQUIRE(x.GetXPixelsNum() == 1024);
    REQUIRE(x.GetYPixelsNum() == 512 * 20);
}

TEST_CASE("DiffractionExperiment_DetectorGeometry_gaps","[DiffractionExperiment]") {
    const size_t gap_x = 8;
    const size_t gap_y = 36;

    DiffractionExperiment x(2, {4, 4, 6, 6}, gap_x,gap_y, false);
    x.Mode(DetectorMode::Conversion);

    REQUIRE(x.GetDataStreamsNum() == 4);
    REQUIRE(x.GetPixelsNum() == (1030 * 2 + gap_x) * (514 * 10 + (10-1) * gap_y) );
    REQUIRE(x.GetXPixelsNum() == 1030 * 2 +gap_x);
    REQUIRE(x.GetYPixelsNum() == 514 * 10 + (10-1) * gap_y);

    REQUIRE(x.GetModulesNum() == 20);
    REQUIRE(x.GetModulesNum(0) == 4);
    REQUIRE(x.GetModulesNum(2) == 6);

    REQUIRE(x.GetFirstModuleOfDataStream(0) == 0);
    REQUIRE(x.GetFirstModuleOfDataStream(1) == 4);
    REQUIRE(x.GetFirstModuleOfDataStream(2) == 8);
    REQUIRE(x.GetFirstModuleOfDataStream(3) == 14);

    REQUIRE(x.GetPixel0OfModule(0) == (2*1030 + gap_x) * (514 + gap_y) * (0/2));
    REQUIRE(x.GetPixel0OfModule(4) == (2*1030 + gap_x) * (514 + gap_y) * (4/2));
    REQUIRE(x.GetPixel0OfModule(8) == (2*1030 + gap_x) * (514 + gap_y) * (8/2));
    REQUIRE(x.GetPixel0OfModule(14) == (2*1030 + gap_x) * (514 + gap_y) * (14/2));

    REQUIRE(x.GetPixel0OfModule(0) == (2*1030 + gap_x) * 0 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(1) == (2*1030 + gap_x) * 0 * (514 + gap_y)  + 1030 + gap_x);

    REQUIRE(x.GetPixel0OfModule(18) == (2*1030 + gap_x) * 9 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(19) == (2*1030 + gap_x) * 9 * (514 + gap_y)  + 1030 + gap_x);


}
TEST_CASE("DiffractionExperiment_DetectorGeometry_gaps_mirror_y","[DiffractionExperiment]") {
    const size_t gap_x = 8;
    const size_t gap_y = 36;

    DiffractionExperiment x(2, {4, 4, 6, 6}, gap_x,gap_y, true);
    x.Mode(DetectorMode::Conversion);

    REQUIRE(x.GetPixel0OfModule(0) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 9 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(1) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 9 * (514 + gap_y)  + 1030 + gap_x);

    REQUIRE(x.GetPixel0OfModule(14) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 2 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(15) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 2 * (514 + gap_y)  + 1030 + gap_x);

    REQUIRE(x.GetPixel0OfModule(16) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 1 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(17) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 1 * (514 + gap_y)  + 1030 + gap_x);

    REQUIRE(x.GetPixel0OfModule(18) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 0 * (514 + gap_y));
    REQUIRE(x.GetPixel0OfModule(19) == (2*1030+gap_x) * 513 + (2*1030 + gap_x) * 0 * (514 + gap_y)  + 1030 + gap_x);
    REQUIRE_THROWS(x.GetPixel0OfModule(20));
}

TEST_CASE("DiffractionExperiment_Metadata","[DiffractionExperiment]") {
    DiffractionExperiment x;

    x.PhotonEnergy_keV(6.0);
    REQUIRE(x.GetPhotonEnergy_keV() == 6.0);
    REQUIRE(x.GetWavelength_A() == Approx(12.39854 / 6.0));

    x.DetectorDistance_mm(30.0).BeamX_pxl(1200).BeamY_pxl(1000)
    .ScatteringVector({1,2,3}).Summation(3).FrameTime(900us);

    REQUIRE(x.GetBeamX_pxl() == Approx(1200));
    REQUIRE(x.GetBeamY_pxl() == Approx(1000));

    REQUIRE(x.GetDetectorDistance_mm() == Approx(30.0));
    REQUIRE(x.GetScatteringVector().x == Approx(1.0/sqrt(14.0) / x.GetWavelength_A()));
    REQUIRE(x.GetScatteringVector().y == Approx(2.0/sqrt(14.0) / x.GetWavelength_A()));
    REQUIRE(x.GetScatteringVector().z == Approx(3.0/sqrt(14.0) / x.GetWavelength_A()));

    REQUIRE_THROWS(x.PedestalG0Frames(-1));
    REQUIRE_THROWS(x.PedestalG1Frames(-1));
    REQUIRE_THROWS(x.PedestalG2Frames(-1));
}

TEST_CASE("DiffractionExperiment_Preview", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    std::map<size_t, uint32_t> map, map2;
    x.ImagesPerTrigger(10000).Summation(1).FrameTime(1ms);
    x.PreviewPeriod(0ms);
    REQUIRE(x.GetPreviewStride() == 0);
    REQUIRE(x.GetPreviewPeriod().count() == 0);

    x.PreviewPeriod(1s);
    REQUIRE(x.GetPreviewPeriod() == std::chrono::milliseconds(1000));
    REQUIRE(x.GetPreviewStride() == 1000);

    REQUIRE_THROWS(x.PreviewPeriod(-5ms));

    REQUIRE(x.GetPreviewStride(100ms) == 100);
    REQUIRE(x.GetPreviewStride(50ms)  == 50);
    REQUIRE(x.GetPreviewStride(1ms)   == 1);
    REQUIRE(x.GetPreviewStride(500us) == 1);
    REQUIRE(x.GetPreviewStride(0s)    == 0);

    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetPreviewStride() == 0);

    x.Mode(DetectorMode::PedestalG0);
    REQUIRE(x.GetPreviewStride() == 0);

    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetPreviewStride() == 0);

    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GetPreviewStride() == 0);
}

TEST_CASE("DiffractionExperiment_SpaceGroup", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    REQUIRE_THROWS(x.SpaceGroupNumber(-1));
    REQUIRE_THROWS(x.SpaceGroupNumber(500));
    REQUIRE_NOTHROW(x.SpaceGroupNumber(0));
    REQUIRE_NOTHROW(x.SpaceGroupNumber(200));
    REQUIRE(x.GetSpaceGroupNumber() == 200);
}

TEST_CASE("DiffractionExperiment_SampleName", "[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_NOTHROW(x.SampleName(""));
    REQUIRE(x.GetSampleName().empty());

    REQUIRE_NOTHROW(x.SampleName("lyso1"));
    REQUIRE(x.GetSampleName() == "lyso1");
}

TEST_CASE("DiffractionExperiment_SpotFinding", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    std::map<size_t, uint32_t> map, map2;
    x.ImagesPerTrigger(10000).Summation(1).FrameTime(1ms);
    x.SpotFindingPeriod(0ms);
    REQUIRE(x.GetSpotFindingStride() == 0);
    REQUIRE(x.GetSpotFindingPeriod().count() == 0);

    x.SpotFindingPeriod(1s);
    REQUIRE(x.GetSpotFindingPeriod() == std::chrono::milliseconds(1000));
    REQUIRE(x.GetSpotFindingStride() == 1000);
    REQUIRE(x.GetSpotFindingBin() == 1);

    x.SpotFindingPeriod(10s);
    REQUIRE(x.GetSpotFindingBin() == 1);

    x.SpotFindingPeriod(10ms);
    REQUIRE(x.GetSpotFindingBin() == 100);

    x.SpotFindingPeriod(4ms);
    REQUIRE(x.GetSpotFindingBin() == 250);

    x.SpotFindingPeriod(50ms);
    REQUIRE(x.GetSpotFindingBin() == 20);

    REQUIRE_THROWS(x.SpotFindingPeriod(-5ms));

    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetSpotFindingStride() == 0);

    x.Mode(DetectorMode::PedestalG0);
    REQUIRE(x.GetSpotFindingStride() == 0);

    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetSpotFindingStride() == 0);

    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GetSpotFindingStride() == 0);
}

TEST_CASE("DiffractionExperiment_FrameCountTime","[DiffractionExperiment]") {
    DiffractionExperiment x;

    x.FrameTime(std::chrono::microseconds(1200));
    REQUIRE(x.GetFrameTime() == std::chrono::microseconds(1200));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1200 - READOUT_TIME_IN_US));
    REQUIRE(x.GetImageTime() == std::chrono::microseconds(1200));
    REQUIRE(x.GetImageCountTime() == std::chrono::microseconds(1200 - READOUT_TIME_IN_US));

    x.FrameTime(std::chrono::microseconds(1200), std::chrono::microseconds(567));
    REQUIRE(x.GetFrameTime() == std::chrono::microseconds(1200));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(567));

    x.Summation(7);
    REQUIRE(x.GetFrameTime() == std::chrono::microseconds(1200));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(567));
    REQUIRE(x.GetImageTime() == std::chrono::microseconds(7*1200));
    REQUIRE(x.GetImageCountTime() == std::chrono::microseconds(7*567));
}

TEST_CASE("DiffractionExperiment_ExportProtobuf","[DiffractionExperiment]") {
    DiffractionExperiment x(4, {4, 4, 8}, 0, 0, false),y;

    std::vector<DetectorMode> v = {DetectorMode::Raw, DetectorMode::Conversion,
                                   DetectorMode::PedestalG0, DetectorMode::PedestalG1, DetectorMode::PedestalG2};
    for (auto &i : v) {
        x.Mode(i).FilePrefix("z").ImagesPerTrigger(20).NumTriggers(5).PedestalG0Frames(1345)
                .PedestalG1Frames(1876).PedestalG2Frames(654)
                .PhotonEnergy_keV(16.0).BeamX_pxl(566).BeamY_pxl(1234).DetectorDistance_mm(145)
                .FrameTime(std::chrono::microseconds(765),std::chrono::microseconds(10))
                .PedestalG1G2FrameTime(std::chrono::milliseconds(10))
                .BaseIPv4Address("2.2.2.2").BaseUDPPort(64*76).MaskModuleEdges(true);

        JFJochProtoBuf::JungfraujochSettings settings_in_protobuf = x;
        REQUIRE_NOTHROW(y.Import(settings_in_protobuf));

        REQUIRE(! y.IsUpsideDown());
        REQUIRE(y.GetFilePrefix() == x.GetFilePrefix());
        REQUIRE(x.GetDataStreamsNum() == y.GetDataStreamsNum());
        REQUIRE(x.GetXPixelsNum() == y.GetXPixelsNum());
        REQUIRE(x.GetModulesNum(2) == y.GetModulesNum(2));
        REQUIRE(x.GetDetectorMode() == y.GetDetectorMode());

        REQUIRE(y.GetPedestalG1Frames() == 1876);
        REQUIRE(y.GetPedestalG2Frames() == 654);
        REQUIRE(y.GetPhotonEnergy_keV() == 16.0);
        REQUIRE(y.GetBeamX_pxl() == 566);
        REQUIRE(y.GetBeamY_pxl() == 1234);
        REQUIRE(y.GetDetectorDistance_mm() == 145);

        REQUIRE(x.GetFrameNum() == y.GetFrameNum());
        REQUIRE(x.GetImageTime() == y.GetImageTime());
        REQUIRE(y.GetFrameCountTime().count() == x.GetFrameCountTime().count());
        REQUIRE(y.GetDestUDPPort(0,0) == 64*76);
        REQUIRE(y.GetDestIPv4Address(0) == 0x02020202);
        REQUIRE(y.GetPedestalG0Frames() == x.GetPedestalG0Frames());
        REQUIRE(y.GetMaskModuleEdges() == x.GetMaskModuleEdges());
        REQUIRE(y.GetMaskChipEdges() == x.GetMaskChipEdges());
    }
}

TEST_CASE("DiffractionExperiment_InternalPacketGenerator", "[DiffractionExperiment]") {
    DiffractionExperiment x;

    x.NumTriggers(50).ImagesPerTrigger(20);
    REQUIRE(x.GetNumTriggers() == 50);
    // Default is false
    REQUIRE(!x.IsUsingInternalPacketGen());
    x.UseInternalPacketGenerator(true);
    REQUIRE(x.IsUsingInternalPacketGen());
    REQUIRE(x.GetNumTriggers() == 50);
    REQUIRE(x.GetFrameNum() == 50*20);
}

TEST_CASE("DiffractionExperiment_CopyConstructor", "[DiffractionExperiment]") {
    DiffractionExperiment a = DiffractionExperiment().Mode(DetectorMode::Raw);
    a.BeamX_pxl(150);
    REQUIRE(a.GetBeamX_pxl() == 150.0);
    REQUIRE(a.GetDetectorMode() == DetectorMode::Raw);

    DiffractionExperiment b(a);
    REQUIRE(b.GetBeamX_pxl() == 150.0);
    b.BeamX_pxl(100);
    REQUIRE(a.GetBeamX_pxl() == 150.0);
    REQUIRE(b.GetBeamX_pxl() == 100.0);
    REQUIRE(b.GetDetectorMode() == DetectorMode::Raw);

    DiffractionExperiment c = b;
    REQUIRE (c.GetBeamX_pxl() == 100.0);
    c.BeamX_pxl(10);
    REQUIRE (a.GetBeamX_pxl() == 150.0);
    REQUIRE (b.GetBeamX_pxl() == 100.0);
    REQUIRE (c.GetBeamX_pxl() == 10.0);
}

TEST_CASE("DiffractionExperiment_ResToPxl","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.DetectorDistance_mm(75).PhotonEnergy_keV(WVL_1A_IN_KEV);

    // sin(theta) = 1/2
    // theta = 30 deg
    // tan(2 * theta) = sqrt(3)
    REQUIRE(x.ResToPxl(1.0) == Approx(1000 * sqrt(3)));

    // sin(theta) = 1/4
    // theta = 14.47 deg
    // tan(2 * theta) = 0.55328333517
    REQUIRE(x.ResToPxl(2.0) == Approx(1000 * 0.55328333517));
}

TEST_CASE("DiffractionExperiment_RadialIntegration_LowQ","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);

    REQUIRE_THROWS(x.LowQForRadialInt_recipA(0));
    REQUIRE_THROWS(x.LowQForRadialInt_recipA(-1));
    REQUIRE_THROWS(x.LowQForRadialInt_recipA(50));

    x.LowQForRadialInt_recipA(4);
    REQUIRE(x.GetLowQForRadialInt_recipA() == Approx(4));

    REQUIRE_THROWS(x.LowResForRadialInt_A(0));
    REQUIRE_THROWS(x.LowResForRadialInt_A(-1));

    x.LowResForRadialInt_A(5.0);
    REQUIRE(x.GetLowQForRadialInt_recipA() == Approx(2 * M_PI / 5.0));

    DiffractionExperiment y(x);
    REQUIRE(y.GetLowQForRadialInt_recipA() == Approx(2 * M_PI / 5.0));
}


TEST_CASE("DiffractionExperiment_RadialIntegration_HighQ","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);

    REQUIRE_THROWS(x.HighQForRadialInt_recipA(0));
    REQUIRE_THROWS(x.HighQForRadialInt_recipA(-1));
    REQUIRE_THROWS(x.HighQForRadialInt_recipA(50));

    x.HighQForRadialInt_recipA(8);
    REQUIRE(x.GetHighQForRadialInt_recipA() == Approx(8.0));

    REQUIRE_THROWS(x.HighResForRadialInt_A(0));
    REQUIRE_THROWS(x.HighResForRadialInt_A(-1));

    x.HighResForRadialInt_A(3.0);
    REQUIRE(x.GetHighQForRadialInt_recipA() == Approx(2 * M_PI / 3.0));

    DiffractionExperiment y(x);
    REQUIRE(y.GetHighQForRadialInt_recipA() == Approx(2 * M_PI / 3.0));
}

TEST_CASE("DiffractionExperiment_RadialIntegration_QSpacing","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);

    x.QSpacingForRadialInt_recipA(0.456);
    REQUIRE(x.GetQSpacingForRadialInt_recipA() == Approx(0.456));

    DiffractionExperiment y(x);
    REQUIRE(y.GetQSpacingForRadialInt_recipA() == Approx(0.456));
}

TEST_CASE("DiffractionExperiment_BkgEstPeriod","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.DetectorDistance_mm(75).PhotonEnergy_keV(WVL_1A_IN_KEV);
    x.FrameTime(1ms).Summation(3);
    x.BackgroundEstimationPeriod(9ms);
    REQUIRE(x.GetBackgroundEstimationPeriod() == std::chrono::milliseconds(9));
    REQUIRE(x.GetBackgroundEstimationStride() == 3);
}

TEST_CASE("DiffractionExperiment_StorageCells","[DiffractionExperiment]") {
    const int64_t num_triggers = 20;
    DiffractionExperiment x;
    x.FrameTime(std::chrono::milliseconds(1)).Summation(1).ImagesPerTrigger(5).NumTriggers(num_triggers);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetImageNumPerTrigger() == 5);
    REQUIRE(x.GetNumTriggers() == num_triggers);

    x.StorageCells(3);
    REQUIRE(x.GetStorageCellNumber() == 3);
    x.StorageCells(7);
    REQUIRE(x.GetStorageCellNumber() == 7);

    x.StorageCells(16);
    REQUIRE(x.GetStorageCellNumber() == 16);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetImageNumPerTrigger() == x.GetStorageCellNumber());
    REQUIRE(x.GetFrameNumPerTrigger() == x.GetStorageCellNumber());
    REQUIRE(x.GetImageNum() == x.GetStorageCellNumber() * num_triggers);
    REQUIRE(x.GetFrameNum() == x.GetStorageCellNumber() * num_triggers);

    x.UseInternalPacketGenerator(true);
    REQUIRE(x.GetImageNumPerTrigger() == x.GetStorageCellNumber());
    REQUIRE(x.GetFrameNumPerTrigger() == x.GetStorageCellNumber());
    REQUIRE(x.GetImageNum() == x.GetStorageCellNumber() * num_triggers);
    REQUIRE(x.GetFrameNum() == x.GetStorageCellNumber() * num_triggers);
}

TEST_CASE("DiffractionExperiment_StorageCells_Pedestal","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.PedestalG0Frames(1456).PedestalG1Frames(323).PedestalG2Frames(456).StorageCells(16);

    x.Mode(DetectorMode::PedestalG0);
    REQUIRE(x.GetStorageCellNumber() == 16);
    REQUIRE(x.GetFrameNum() == 1456 * 16);

    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetStorageCellNumber() == 2);
    REQUIRE(x.GetFrameNum() == 323 * 2);

    x.StorageCells(11);
    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GetStorageCellNumber() == 2);
    REQUIRE(x.GetFrameNum() == 456 * 2);
}


TEST_CASE("DiffractionExperiment_IsPixelSigned","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.DetectorType(JFJochProtoBuf::JUNGFRAU).Mode(DetectorMode::Conversion);
    REQUIRE(x.IsPixelSigned());
    x.DetectorType(JFJochProtoBuf::EIGER);
    REQUIRE(!x.IsPixelSigned());
    x.DetectorType(JFJochProtoBuf::JUNGFRAU).Mode(DetectorMode::Raw);
    REQUIRE(!x.IsPixelSigned());
}

TEST_CASE("DiffractionExperiment_DetectorType","[DiffractionExperiment]") {
    DiffractionExperiment x,y;
    REQUIRE(x.GetDetectorType() == JFJochProtoBuf::JUNGFRAU); // JF is default
    x.DetectorType(JFJochProtoBuf::EIGER);
    REQUIRE(x.GetDetectorType() == JFJochProtoBuf::EIGER);

    JFJochProtoBuf::JungfraujochSettings settings_in_protobuf = x;
    REQUIRE_NOTHROW(y.Import(settings_in_protobuf));

    REQUIRE(y.GetDetectorType() == JFJochProtoBuf::EIGER);
}

TEST_CASE("DiffractionExperiment_DetectorInput_MultiTriggger","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.FrameTime(700us).Summation(1).ImagesPerTrigger(350).NumTriggers(7);
    JFJochProtoBuf::DetectorInput ret = x;
    REQUIRE(ret.modules_num() == 8);
    REQUIRE(ret.period_us() == x.GetFrameTime().count());
    REQUIRE(ret.count_time_us() == x.GetFrameCountTime().count());
    REQUIRE(ret.num_triggers() == 8);
    REQUIRE(ret.num_frames() == 350);
    REQUIRE(ret.storage_cell_number() == 1);
    REQUIRE(ret.mode() == JFJochProtoBuf::CONVERSION);
}

TEST_CASE("DiffractionExperiment_DetectorInput_NoTriggger","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.FrameTime(1200us).Summation(1).ImagesPerTrigger(350).NumTriggers(1);
    JFJochProtoBuf::DetectorInput ret = x;
    REQUIRE(ret.modules_num() == 8);
    REQUIRE(ret.period_us() == x.GetFrameTime().count());
    REQUIRE(ret.count_time_us() == x.GetFrameCountTime().count());
    REQUIRE(ret.num_triggers() == 1);
    REQUIRE(ret.num_frames() == 350 + DELAY_FRAMES_STOP_AND_QUIT);
    REQUIRE(ret.storage_cell_number() == 1);
    REQUIRE(ret.mode() == JFJochProtoBuf::CONVERSION);
}

TEST_CASE("DiffractionExperiment_DetectorInput_PedestalG2","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.FrameTime(1200us).Summation(1).PedestalG2Frames(4560).NumTriggers(1).Mode(DetectorMode::PedestalG2);
    JFJochProtoBuf::DetectorInput ret = x;
    REQUIRE(ret.modules_num() == 8);
    REQUIRE(ret.period_us() == x.GetFrameTime().count());
    REQUIRE(ret.count_time_us() == x.GetFrameCountTime().count());
    REQUIRE(ret.num_triggers() == 1);
    REQUIRE(ret.num_frames() == 4560 + DELAY_FRAMES_STOP_AND_QUIT);
    REQUIRE(ret.storage_cell_number() == 1);
}

TEST_CASE("DiffractionExperiment_DetectorInput_StorageCell","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.FrameTime(1200us).Summation(1).NumTriggers(4560).StorageCells(7);
    JFJochProtoBuf::DetectorInput ret = x;
    REQUIRE(ret.modules_num() == 8);
    REQUIRE(ret.period_us() == 7 * (x.GetFrameTime().count() + 10));
    REQUIRE(ret.count_time_us() == x.GetFrameCountTime().count());
    REQUIRE(ret.num_triggers() == 4560 + 1);
    REQUIRE(ret.num_frames() == 1);
    REQUIRE(ret.storage_cell_number() == 7);
    REQUIRE(ret.storage_cell_delay() == 5);
    REQUIRE(ret.storage_cell_start() == x.GetStorageCellStart());
}

TEST_CASE("DiffractionExperiment_LoadDatasetSettings", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.ImagesPerTrigger(567).BeamY_pxl(324).ScatteringVector({1,0,0})
            .Compression(JFJochProtoBuf::BSHUF_ZSTD);
    JFJochProtoBuf::DatasetSettings settings;
    settings.set_images_per_trigger(234);
    settings.set_ntrigger(56);
    settings.set_summation(4);
    settings.set_beam_x_pxl(23.4);
    settings.set_beam_y_pxl(123.4);
    settings.set_photon_energy_kev(WVL_1A_IN_KEV);
    settings.set_detector_distance_mm(57.6);
    settings.set_data_file_count(5);
    settings.set_space_group_number(45);
    settings.set_sample_name("lyso1");

    REQUIRE_NOTHROW(x.LoadDatasetSettings(settings));

    REQUIRE(x.GetScatteringVector().x == 0);
    REQUIRE(x.GetImageNumPerTrigger() == 234);
    REQUIRE(x.GetBeamY_pxl() == Approx(123.4));
    REQUIRE(x.GetSpaceGroupNumber() == 45);
    REQUIRE(x.GetCompressionAlgorithm() == JFJochProtoBuf::BSHUF_LZ4);
}

TEST_CASE("DiffractionExperiment_ImageTimeUs", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.FrameTime(500us);

    JFJochProtoBuf::DatasetSettings settings;
    settings.set_images_per_trigger(234);
    settings.set_ntrigger(56);
    settings.set_image_time_us(2500);
    settings.set_beam_x_pxl(23.4);
    settings.set_beam_y_pxl(123.4);
    settings.set_photon_energy_kev(WVL_1A_IN_KEV);
    settings.set_detector_distance_mm(57.6);
    settings.set_data_file_count(5);
    settings.set_space_group_number(45);
    settings.set_sample_name("lyso1");

    REQUIRE_NOTHROW(x.LoadDatasetSettings(settings));
    REQUIRE(x.GetSummation() == 5);

    settings.set_image_time_us(2501);
    REQUIRE_THROWS(x.LoadDatasetSettings(settings));

    settings.set_image_time_us(0);
    REQUIRE_THROWS(x.LoadDatasetSettings(settings));

    settings.set_image_time_us(-2500);
    REQUIRE_THROWS(x.LoadDatasetSettings(settings));

}

TEST_CASE("DiffractionExperiment_LoadDatasetSettings_Invalid", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.ImagesPerTrigger(567).BeamY_pxl(324).ScatteringVector({1,0,0})
            .Compression(JFJochProtoBuf::BSHUF_ZSTD);
    JFJochProtoBuf::DatasetSettings settings;
    settings.set_images_per_trigger(-1);
    settings.set_ntrigger(56);
    settings.set_summation(4);
    settings.set_beam_x_pxl(23.4);
    settings.set_beam_y_pxl(123.4);
    settings.set_photon_energy_kev(WVL_1A_IN_KEV);
    settings.set_detector_distance_mm(57.6);
    settings.set_data_file_count(5);
    settings.set_space_group_number(45);
    settings.set_sample_name("lyso1");

    REQUIRE_THROWS(x.LoadDatasetSettings(settings));

    REQUIRE(x.GetScatteringVector().x == Approx(1));
    REQUIRE(x.GetImageNumPerTrigger() == 567);
    REQUIRE(x.GetBeamY_pxl() == Approx(324));
    REQUIRE(x.GetSpaceGroupNumber() == 0);
}

TEST_CASE("DiffractionExperiment_LoadDetectorSettings", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.PedestalG0Frames(456).PedestalG1Frames(1234).PedestalG2Frames(123);

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(600);
    settings.set_count_time_us(400);
    settings.set_use_storage_cells(true);
    settings.set_use_internal_packet_generator(true);
    settings.set_collect_raw_data(true);
    settings.set_pedestal_g0_frames(5000);
    settings.set_pedestal_g1_frames(100);
    settings.set_pedestal_g2_frames(150);
    REQUIRE_NOTHROW(x.LoadDetectorSettings(settings));

    REQUIRE(x.GetFrameTime().count() == 600);
    REQUIRE(x.GetFrameCountTime().count() == 400);
    REQUIRE(x.IsUsingInternalPacketGen());
    REQUIRE(x.GetStorageCellNumber() == 16);
    REQUIRE(x.GetDetectorMode() == DetectorMode::Raw);
    REQUIRE(x.GetPedestalG0Frames() == 5000);
    REQUIRE(x.GetPedestalG1Frames() == 100);
    REQUIRE(x.GetPedestalG2Frames() == 150);
}

TEST_CASE("DiffractionExperiment_LoadDetectorSettings_invalid", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.PedestalG0Frames(456).PedestalG1Frames(1234).PedestalG2Frames(123);
    x.FrameTime(525us).Mode(DetectorMode::Conversion);
    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(600);
    settings.set_count_time_us(800);
    settings.set_use_storage_cells(true);
    settings.set_use_internal_packet_generator(true);
    settings.set_collect_raw_data(true);
    settings.set_pedestal_g0_frames(5000);
    settings.set_pedestal_g1_frames(100);
    settings.set_pedestal_g2_frames(150);
    REQUIRE_THROWS(x.LoadDetectorSettings(settings));

    REQUIRE(x.GetFrameTime().count() == 525);
    REQUIRE(x.GetFrameCountTime().count() == 525 - READOUT_TIME_IN_US);
    REQUIRE(!x.IsUsingInternalPacketGen());
    REQUIRE(x.GetStorageCellNumber() == 1);
    REQUIRE(x.GetDetectorMode() == DetectorMode::Conversion);
    REQUIRE(x.GetPedestalG0Frames() == 456);
    REQUIRE(x.GetPedestalG1Frames() == 1234);
    REQUIRE(x.GetPedestalG2Frames() == 123);
}

TEST_CASE("DiffractionExperiment_LoadDetectorSettings_inferred", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.PedestalG0Frames(456).PedestalG1Frames(1234).PedestalG2Frames(123);

    JFJochProtoBuf::DetectorSettings settings;
    settings.set_frame_time_us(600);

    REQUIRE_NOTHROW(x.LoadDetectorSettings(settings));

    REQUIRE(x.GetFrameTime().count() == 600);
    REQUIRE(x.GetFrameCountTime().count() == 600 - READOUT_TIME_IN_US);
    REQUIRE(!x.IsUsingInternalPacketGen());
    REQUIRE(x.GetStorageCellNumber() == 1);
    REQUIRE(x.GetDetectorMode() == DetectorMode::Conversion);
    REQUIRE(x.GetPedestalG0Frames() == 456);
    REQUIRE(x.GetPedestalG1Frames() == 1234);
    REQUIRE(x.GetPedestalG2Frames() == 123);
}

TEST_CASE("DiffractionExperiment_DefaultDataProcessingSettings","[DiffractionExperiment]") {
    REQUIRE_NOTHROW(DiffractionExperiment::CheckDataProcessingSettings(
            DiffractionExperiment::DefaultDataProcessingSettings()));
}