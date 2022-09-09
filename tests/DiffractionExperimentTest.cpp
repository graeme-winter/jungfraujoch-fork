// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <google/protobuf/util/message_differencer.h>

#include "../common/DiffractionExperiment.h"


using namespace std::literals::chrono_literals;

TEST_CASE("DiffractionExperiment_FilePath","[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_NOTHROW(x.FilePrefix("x"));
    REQUIRE(x.GetFilePrefix() == "x");

    REQUIRE(x.GenerateMasterFilename() == "x_master.h5");
    REQUIRE(x.GenerateDataFilename(5) == "x_data_000006.h5");

    // trailing / needs to be ignored
    REQUIRE_THROWS(x.FilePrefix("/x"));
    REQUIRE_THROWS(x.FilePrefix("../x"));
    REQUIRE_THROWS(x.FilePrefix("x/../y"));

    x.Mode(DetectorMode::PedestalG0);
    REQUIRE(x.GenerateMasterFilename() == "x_pedestalG0_master.h5");
    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GenerateMasterFilename() == "x_pedestalG1_master.h5");
    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GenerateMasterFilename() == "x_pedestalG2_master.h5");

    x.Mode(DetectorMode::Conversion).RunNumber(57);
    REQUIRE(x.GenerateDataFilename(5) == "x_00057_data_000006.h5");

    x.ImagesPerFile(0);
    REQUIRE(x.GenerateDataFilename(5) == "x_00057_data.h5");

    x.Mode(DetectorMode::Conversion).RunNumber(DiffractionExperiment::RunNumberNotSet).ImagesPerFile(0);
    REQUIRE(x.GenerateDataFilename(0) == "x_data.h5");

    REQUIRE_NOTHROW(x.FilePrefix("x6_master.h5"));
    REQUIRE(x.GetFilePrefix() == "x6");

    REQUIRE_NOTHROW(x.FilePrefix("long_name/directory/file_master.h5"));
    REQUIRE(x.GetFilePrefix() == "long_name/directory/file");

    REQUIRE_NOTHROW(x.FilePrefix("_master.h5"));
    REQUIRE(x.GetFilePrefix() == "_master.h5");
}

TEST_CASE("DiffractionExperiment_ModeTxt","[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_NOTHROW(x.Mode_Text("pedestalG0"));
    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG0);
    REQUIRE(x.GetDetectorModeTxt() == "pedestalG0");

    REQUIRE_NOTHROW(x.Mode_Text("pedestalG1"));
    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG1);
    REQUIRE(x.GetDetectorModeTxt() == "pedestalG1");

    REQUIRE_NOTHROW(x.Mode_Text("pedestalG2"));
    REQUIRE(x.GetDetectorMode() == DetectorMode::PedestalG2);
    REQUIRE(x.GetDetectorModeTxt() == "pedestalG2");

    REQUIRE_NOTHROW(x.Mode_Text("conversion"));
    REQUIRE(x.GetDetectorMode() == DetectorMode::Conversion);
    REQUIRE(x.GetDetectorModeTxt() == "conversion");

    REQUIRE_NOTHROW(x.Mode_Text("raw"));
    REQUIRE(x.GetDetectorMode() == DetectorMode::Raw);
    REQUIRE(x.GetDetectorModeTxt() == "raw");

    REQUIRE_THROWS(x.Mode_Text("Conversion"));
    REQUIRE_THROWS(x.Mode_Text(""));
    REQUIRE_THROWS(x.Mode_Text("6"));
}

TEST_CASE("DiffractionExperiment_ImageAndFileNumber","[DiffractionExperiment]") {
    DiffractionExperiment x;

    x.ImagesPerTrigger(134).ImagesPerFile(100);
    REQUIRE(x.GetImageNum() == 134);
    REQUIRE(x.GetImagesPerFile() == 100);
    REQUIRE(x.GetImagesInFile(0) == 100);
    REQUIRE(x.GetImagesInFile(1) == 34);
    REQUIRE_THROWS(x.GetImagesInFile(2));
    REQUIRE_THROWS(x.GetImagesInFile(-1));
    REQUIRE(x.GetFilesNum() == 2);

    REQUIRE(x.GetFileForImage(0) == 0);
    REQUIRE(x.GetFileForImage(100) == 1);

    x.ImagesPerTrigger(78).ImagesPerFile(100);
    REQUIRE(x.GetImageNum() == 78);
    REQUIRE(x.GetImagesPerFile() == 100);
    REQUIRE(x.GetImagesInFile(0) == 78);
    REQUIRE(x.GetFilesNum() == 1);

    x.ImagesPerTrigger(78).ImagesPerFile(0);
    REQUIRE(x.GetImageNum() == 78);
    REQUIRE(x.GetImagesPerFile() == 78);
    REQUIRE(x.GetImagesInFile(0) == 78);
    REQUIRE(x.GetFilesNum() == 1);
    REQUIRE(x.GetFileForImage(0) == 0);
    REQUIRE(x.GetFileForImage(77) == 0);
}

TEST_CASE("DiffractionExperiment_ImageAndFileNumber_ActualImages","[DiffractionExperiment]") {
    DiffractionExperiment x;

    x.ImagesPerTrigger(134).ImagesPerFile(100);

    // with actual image number
    REQUIRE(x.GetFilesNum(100) == 1);
    REQUIRE(x.GetFilesNum(101) == 2);
    REQUIRE(x.GetImagesInFile(0, 91) == 91);
    REQUIRE_THROWS(x.GetImagesInFile(1, 91));
    REQUIRE(x.GetImagesInFile(0, 101) == 100);
    REQUIRE(x.GetImagesInFile(1, 101) == 1);
}

TEST_CASE("DiffractionExperiment_SequenceNumber","[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_THROWS(x.MeasurementSequenceNumber(-1));
    REQUIRE_THROWS(x.MeasurementSequenceNumber(INT32_MAX));

    REQUIRE_NOTHROW(x.MeasurementSequenceNumber(345));
    REQUIRE(x.GetMeasurementSequenceNumber() == 345);

    x.IncrementMeasurementSequenceNumber();
    REQUIRE(x.GetMeasurementSequenceNumber() == 346);

    REQUIRE_NOTHROW(x.MeasurementSequenceNumber(INT32_MAX - 1));
    x.IncrementMeasurementSequenceNumber();
    REQUIRE(x.GetMeasurementSequenceNumber() == 0);
}

TEST_CASE("DiffractionExperiment_Compression_Raw","[DiffractionExperiment]") {
    DiffractionExperiment x;
    for (auto i: {DetectorMode::Raw, DetectorMode::PedestalG0, DetectorMode::PedestalG1, DetectorMode::PedestalG2}) {
        x.Mode(i).Compression(CompressionAlgorithm::BSHUF_ZSTD, 5);
        REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::None);
        REQUIRE(x.GetCompressionAlgorithmText() == "off");
    }
}

TEST_CASE("DiffractionExperiment_Compression","[DiffractionExperiment]") {
    DiffractionExperiment x;

    // Compression
    x.Compression(CompressionAlgorithm::BSHUF_LZ4);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_LZ4);
    REQUIRE(x.GetCompressionBlockSize() == LZ4_BLOCK_SIZE);
    REQUIRE(x.GetCompressionLevel() == 0);
    REQUIRE(x.GetCompressionAlgorithmText() == "bslz4");

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionBlockSize() == ZSTD_BLOCK_SIZE);
    REQUIRE(x.GetCompressionLevel() == 0);
    REQUIRE(x.GetCompressionAlgorithmText() == "bszstd");

    x.Compression(CompressionAlgorithm::BSHUF_LZ4);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_LZ4);

    x.Compression(CompressionAlgorithm::None);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::None);
    REQUIRE(x.GetCompressionAlgorithmText() == "off");

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionLevel() == ZSTD_USE_JFJOCH_RLE);
    REQUIRE(x.GetCompressionAlgorithmText() == "bszstd_rle");

    x.CompressionBlockSize(512);
    REQUIRE(x.GetCompressionBlockSize() == 512);

    REQUIRE_THROWS(x.CompressionBlockSize(513));
    REQUIRE_THROWS(x.CompressionBlockSize(0));

    x.Compression(CompressionAlgorithm::BSHUF_LZ4, 3);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_LZ4);
    REQUIRE(x.GetCompressionLevel() == 3);
    REQUIRE(x.GetCompressionAlgorithmText() == "bslz4:3");

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionLevel() == 0);

    REQUIRE(x.GetCompressionBlockSize() == 512);

    x.Compression_Text("off");
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::None);

    x.Compression_Text("bslz4:5");
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_LZ4);
    REQUIRE(x.GetCompressionLevel() == 5);

    x.Compression_Text("bszstd:-1");
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionLevel() == -1);

    x.Compression_Text("bszstd_rle");
    REQUIRE(x.GetCompressionAlgorithm() == CompressionAlgorithm::BSHUF_ZSTD);
    REQUIRE(x.GetCompressionLevel() == ZSTD_USE_JFJOCH_RLE);

}

TEST_CASE("DiffractionExperiment_Timing","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.Mode(DetectorMode::Conversion);
    // Timing and frame count
    x.PedestalG0Frames(1000).ImagesPerTrigger(1000).NumTriggers(1).ImageTime(1000us);

    // Frame count analysis

    REQUIRE(x.GetFrameNum() == 1000);
    REQUIRE(x.GetFrameTime().count() == 1000);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetPixelDepth() == 2);
    REQUIRE(x.GetOverflow() == INT16_MAX - 4);
    REQUIRE(x.GetUnderflow() == INT16_MIN + 4);
    REQUIRE(x.GetFrameNumPerTrigger() == 1000);
    x.PedestalG0Frames(1000).PedestalG1Frames(2000).PedestalG2Frames(3000).NumTriggers(1)
        .ImagesPerTrigger(1000).ImageTime(6000us);

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

    x.NumTriggers(0);
    REQUIRE(x.GetFrameNum() == 6000);
    REQUIRE(x.GetImageNum() == 1000);
    REQUIRE(x.GetImageNumPerTrigger() == 1000);

    x.NumTriggers(2);
    REQUIRE(x.GetFrameNum() == 12000);
    REQUIRE(x.GetNumTriggers() == 2);
    REQUIRE(x.GetImageNum() == 2000);
    REQUIRE(x.GetImageNumPerTrigger() == 1000);
    REQUIRE(x.GetFrameNumPerTrigger() == 6000);


    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetFrameNum() == 12000);
    REQUIRE(x.GetImageNum() == 12000);
    REQUIRE(x.GetImageNumPerTrigger() == 6000);

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

TEST_CASE("DiffractionExperiment_Timing_FullSpeed","[DiffractionExperiment]") {
    DiffractionExperiment x;
    // Timing and frame count
    REQUIRE_THROWS(x.ImageTime(300us));

    x.ImageTime(550us);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetFrameTime().count() == 550);
    REQUIRE(x.IsDetectorFullSpeed());

    x.ImageTime(1020us);
    REQUIRE(x.GetSummation() == 1);
    REQUIRE(x.GetFrameTime().count() == 1020);
    REQUIRE(!x.IsDetectorFullSpeed());

    x.ImageTime(2ms).ForceFullSpeed(true);
    REQUIRE(x.GetSummation() == 4);
    REQUIRE(x.GetFrameTime().count() == 500);
    REQUIRE(x.IsDetectorFullSpeed());

    x.ImageTime(2ms).ForceFullSpeed(false);
    REQUIRE(x.GetSummation() == 2);
    REQUIRE(x.GetFrameTime().count() == 1000);
    REQUIRE(!x.IsDetectorFullSpeed());
}

TEST_CASE("DiffractionExperiment_ErrorWhenOverwriting","[DiffractionExperiment]") {
    DiffractionExperiment x;
    REQUIRE(!x.GetErrorWhenOverwriting());
    x.ErrorWhenOverwriting(true);
    REQUIRE(x.GetErrorWhenOverwriting());
    JFJochProtoBuf::JungfraujochSettings settings = x;
    DiffractionExperiment x2(settings);
    REQUIRE(x2.GetErrorWhenOverwriting());
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
    x.SetUnitCell(cell);

    REQUIRE(x.HasUnitCell());
    REQUIRE(x.GetUnitCell().c() == 30);

    JFJochProtoBuf::JungfraujochSettings settings = x;

    DiffractionExperiment y(settings);

    REQUIRE(google::protobuf::util::MessageDifferencer::Equivalent(y.GetUnitCell(), x.GetUnitCell()));
}

TEST_CASE("DiffractionExperiment_SpaceGroup","[DiffractionExperiment]") {
    DiffractionExperiment x;
    REQUIRE_THROWS(x.SpaceGroupNumber(999));
    REQUIRE_THROWS(x.SpaceGroup("P898"));

    REQUIRE_NOTHROW(x.SpaceGroup("P43212"));
    REQUIRE(x.GetSpaceGroupNumber() == 96);
    REQUIRE(x.GetCentering() == 'P');

    REQUIRE_NOTHROW(x.SpaceGroupNumber(21));
    REQUIRE(x.GetSpaceGroupNumber() == 21);
    REQUIRE(x.GetCentering() == 'C');
    REQUIRE(x.GetSpaceGroupName() == "C222");
}

/*
TEST_CASE("DiffractionExperiment_TimeUnits","[DiffractionExperiment]") {
    DiffractionExperiment x;

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":425} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 425);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":425.1} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 425);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":"425us"} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 425);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":"1ms"} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 1000);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":"1.0ms"} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 1000);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":"1.0 s"} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 1000000);

    REQUIRE_NOTHROW(x.Import(R"( {"shutter_delay":"1.0   sec"} )"_json));
    REQUIRE(x.GetShutterDelay().count() == 1000000);

    REQUIRE_THROWS(x.Import(R"( {"shutter_delay":"1.0 day"} )"_json));
    REQUIRE_THROWS(x.Import(R"( {"shutter_delay":""} )"_json));
    REQUIRE_THROWS(x.Import(R"( {"shutter_delay":"us"} )"_json));
    REQUIRE_THROWS(x.Import(R"( {"shutter_delay":"ms"} )"_json));
} */

TEST_CASE("DiffractionExperiment_IPv4Address","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.DataStreamModuleSize(1, {4, 4, 4}).Mode(DetectorMode::Conversion);

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

    REQUIRE(x.IPv4AddressToStr(x.GetDestIPv4Address(2)) == "64.1.124.113");
}

TEST_CASE("DiffractionExperiment_MacAddressToStr","[DiffractionExperiment]") {
    REQUIRE(DiffractionExperiment::MacAddressToStr(0xF1EEDDCCBBAA) == "aa:bb:cc:dd:ee:f1");
    REQUIRE(DiffractionExperiment::MacAddressToStr(0x0000DDCCBB00) == "00:bb:cc:dd:00:00");
}

TEST_CASE("DiffractionExperiment_MacAddressFromStr","[DiffractionExperiment]") {
    REQUIRE(DiffractionExperiment::MacAddressFromStr("aa:bb:cc:dd:ee:f1") == 0xF1EEDDCCBBAA);
    REQUIRE(DiffractionExperiment::MacAddressFromStr("11:22:33:44:55:66") == 0x665544332211);
    REQUIRE_THROWS(DiffractionExperiment::MacAddressFromStr("11:22:33:44:55:66:77"));
    REQUIRE_THROWS(DiffractionExperiment::MacAddressFromStr("11:22:33:44:55"));
    REQUIRE_THROWS(DiffractionExperiment::MacAddressFromStr("456:22:33:44:55"));
    REQUIRE_THROWS(DiffractionExperiment::MacAddressFromStr("xy:22:33:44:55"));
}

TEST_CASE("DiffractionExperiment_UDPAddress","[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.DataStreamModuleSize(1, {4, 4, 4}).Mode(DetectorMode::Conversion);

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

TEST_CASE("DiffractionExperiment_GetCol0OfModule","[DiffractionExperiment]") {
    int64_t gap_x = 8;
    DiffractionExperiment x(2, {4, 4}, gap_x, 36);
    x.UpsideDown(true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetCol0OfModule(0) == 0);
    REQUIRE(x.GetCol0OfModule(1) == CONVERTED_MODULE_COLS + gap_x);

    REQUIRE(x.GetCol0OfModule(4) == 0);
    REQUIRE(x.GetCol0OfModule(5) == CONVERTED_MODULE_COLS + gap_x);

    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetCol0OfModule(0) == 0);
    REQUIRE(x.GetCol0OfModule(1) == 0);

    REQUIRE(x.GetCol0OfModule(4) == 0);
    REQUIRE(x.GetCol0OfModule(5) == 0);
}


TEST_CASE("DiffractionExperiment_GetLine0OfModule","[DiffractionExperiment]") {
    int64_t gap_y = 36;
    DiffractionExperiment x(2, {4, 4}, 8, gap_y);
    x.UpsideDown(true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetLine0OfModule(0) == 4 * CONVERTED_MODULE_LINES - 1 + 3 * gap_y);
    REQUIRE(x.GetLine0OfModule(1) == 4 * CONVERTED_MODULE_LINES - 1 + 3 * gap_y);

    REQUIRE(x.GetLine0OfModule(4) == 2 * CONVERTED_MODULE_LINES - 1 + gap_y);
    REQUIRE(x.GetLine0OfModule(5) == 2 * CONVERTED_MODULE_LINES - 1 + gap_y);

    REQUIRE(x.GetLine0OfModule(6) == CONVERTED_MODULE_LINES - 1);
    REQUIRE(x.GetLine0OfModule(7) == CONVERTED_MODULE_LINES - 1);

    x.Mode(DetectorMode::Raw);
    REQUIRE(x.GetLine0OfModule(0) == 0);
    REQUIRE(x.GetLine0OfModule(1) == RAW_MODULE_LINES * 1);
    REQUIRE(x.GetLine0OfModule(4) == RAW_MODULE_LINES * 4);
    REQUIRE(x.GetLine0OfModule(5) == RAW_MODULE_LINES * 5);
}

TEST_CASE("DiffractionExperiment_DetectorGeometry","[DiffractionExperiment]") {
    DiffractionExperiment x(1, {4, 4, 6, 6}); // 10M configuration #1 - via constructor

    x.Mode(DetectorMode::Conversion);

    REQUIRE(x.GetHorizontalStacking() == 1);
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

    x.DataStreamModuleSize(2, {4, 4, 6, 6}).UpsideDown(true); // 10M configuration #2
    REQUIRE(x.GetHorizontalStacking() == 2);

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

    DiffractionExperiment x;

    const size_t gap_x = 8;
    const size_t gap_y = 36;

    x.DataStreamModuleSize(2, {4, 4, 6, 6}, gap_x,gap_y).Mode(DetectorMode::Conversion)
        .UpsideDown(false); // 10M configuration #1

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

    x.UpsideDown(true);
    //REQUIRE(x.GetPixel0OfModule(0) == (2*1030 + gap_x) * (514 + gap_y) * (18/2));
    //REQUIRE(x.GetPixel0OfModule(4) == (2*1030 + gap_x) * (514 + gap_y) * (16/2));
    //REQUIRE(x.GetPixel0OfModule(8) == (2*1030 + gap_x) * (514 + gap_y) * (12/2));
    //REQUIRE(x.GetPixel0OfModule(14) == (2*1030 + gap_x) * (514 + gap_y) * (6/2));

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

    x.TotalFlux(2e9);
    REQUIRE(x.GetTotalFlux() == Approx(2e9));

    x.Transmission(0.45);
    REQUIRE(x.GetTransmission() == Approx(0.45));
    REQUIRE_THROWS(x.Transmission(1.0001));

    x.SampleTemperature(150.0);
    REQUIRE(x.GetSampleTemperature() == Approx(150.0));

    x.DetectorDistance_mm(30.0).OmegaStart(50.0).OmegaIncrement(1.0).BeamX_pxl(1200).BeamY_pxl(1000).BeamSizeX_um(25)
            .BeamSizeY_um(75).TrackingID("ID0").ScatteringVector({1,2,3}).RotationAxis({4,5,6}).ImageTime(2700us);

    REQUIRE(x.GetBeamX_pxl() == Approx(1200));
    REQUIRE(x.GetBeamY_pxl() == Approx(1000));

    REQUIRE(x.GetDetectorDistance_mm() == Approx(30.0));
    REQUIRE(x.GetOmegaStart() == Approx(50.0));
    REQUIRE(x.GetOmegaIncrement() == Approx(1.0));
    REQUIRE(x.GetOmega(10.0) == Approx(50.0+10.0));
    REQUIRE(x.GetRotationSpeed() == Approx(1.0/2700e-6));
    REQUIRE(x.GetBeamSizeX_um() == Approx(25.0));
    REQUIRE(x.GetBeamSizeY_um() == Approx(75.0));
    REQUIRE(x.GetTrackingID() == "ID0");
    REQUIRE(x.GetScatteringVector().x == Approx(1.0/sqrt(14.0) / x.GetWavelength_A()));
    REQUIRE(x.GetScatteringVector().y == Approx(2.0/sqrt(14.0) / x.GetWavelength_A()));
    REQUIRE(x.GetScatteringVector().z == Approx(3.0/sqrt(14.0) / x.GetWavelength_A()));
    REQUIRE(x.GetRotationAxis().Length() == Approx(1.0));

    x.DetectorName("det1").SourceName("x").SourceNameShort("x1").InstrumentName("x2").InstrumentNameShort("x3");
    REQUIRE(x.GetDetectorName() == "det1");
    REQUIRE(x.GetSourceName() == "x");
    REQUIRE(x.GetSourceNameShort() == "x1");
    REQUIRE(x.GetInstrumentName() == "x2");
    REQUIRE(x.GetInstrumentNameShort() == "x3");

    REQUIRE_THROWS(x.PedestalG0Frames(-1));
    REQUIRE_THROWS(x.PedestalG1Frames(-1));
    REQUIRE_THROWS(x.PedestalG2Frames(-1));
}

TEST_CASE("DiffractionExperiment_TimeResolvedMode", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    x.ImagesPerTrigger(20).ImagesPerFile(50).NumTriggers(10).PedestalG0Frames(500).ImageTime(std::chrono::milliseconds(10));
    x.TimeResolvedMode(true);

    REQUIRE(x.GetNumTriggers() == 10);
    REQUIRE(x.GetImageNumPerTrigger() == 20);
    REQUIRE(x.GetFrameNumPerTrigger() == 20 * 10);

    REQUIRE(x.GetFilesNum() == x.GetImageNumPerTrigger());
    REQUIRE(x.GetImagesPerFile() == x.GetNumTriggers());
    
    REQUIRE(x.GetFrameNum() == 20 * 10 * 10);
    REQUIRE(x.GetImageNum() == 20 * 10);
    REQUIRE(x.GetImagesInFile(0) == 10);
    REQUIRE(x.GetImagesInFile(1) == 10);
    REQUIRE(x.GetImagesInFile(9) == 10);

    REQUIRE(x.GetFileForImage(0) == 0);
    REQUIRE(x.GetFileForImage(19) == 19);
    REQUIRE(x.GetFileForImage(39) == 19);
}

TEST_CASE("DiffractionExperiment_Preview", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    std::map<size_t, uint32_t> map, map2;
    x.ImagesPerTrigger(10000).ImageTime(1ms);
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


TEST_CASE("DiffractionExperiment_SpotFinding", "[DiffractionExperiment]") {
    DiffractionExperiment x;
    std::map<size_t, uint32_t> map, map2;
    x.ImagesPerTrigger(10000).ImageTime(1ms);
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
    x.ImageTime(std::chrono::microseconds(2200));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1100 - READOUT_TIME_IN_US));

    x.ImageTime(std::chrono::microseconds(1000));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1000 - READOUT_TIME_IN_US));

    x.ImageTime(std::chrono::microseconds(3000));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1000 - READOUT_TIME_IN_US));

    x.ForceFullSpeed(true);
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(500 - READOUT_TIME_IN_US));
    x.ForceFullSpeed(false);

    x.Mode(DetectorMode::PedestalG1);
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1000 - READOUT_TIME_IN_US));

    x.Mode(DetectorMode::PedestalG2);
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(1000 - READOUT_TIME_IN_US));

    x.ForceFullSpeed(true);
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(500 - READOUT_TIME_IN_US));

    x.CountTime(std::chrono::microseconds(10));
    REQUIRE(x.GetFrameCountTime() == std::chrono::microseconds(10));
}

TEST_CASE("DiffractionExperiment_ExportProtobuf","[DiffractionExperiment]") {
    DiffractionExperiment x,y;

    std::vector<DetectorMode> v = {DetectorMode::Raw, DetectorMode::Conversion,
                                   DetectorMode::PedestalG0, DetectorMode::PedestalG1, DetectorMode::PedestalG2};
    for (auto &i : v) {
        x.UpsideDown(false).DataStreamModuleSize(4, {4, 4, 8}).
                        Mode(i).FilePrefix("z").ImagesPerTrigger(20).NumTriggers(5).PedestalG0Frames(1345).
                        PedestalG1Frames(1876).PedestalG2Frames(654).OptimizeFrameTime(false).
                        PhotonEnergy_keV(16.0).BeamX_pxl(566).BeamY_pxl(1234).DetectorDistance_mm(145).
                        Transmission(0.5).TotalFlux(1e9).SampleTemperature(90).ForceFullSpeed(true).
                        ImageTime(std::chrono::microseconds(765)).CountTime(std::chrono::microseconds(10)).
                        PedestalG1G2FrameTime(std::chrono::milliseconds(10))
                .RotationAxis({3, 4, 5})
                .BaseIPv4Address("2.2.2.2").BaseUDPPort(64*76).TimeResolvedMode(true).MaskModuleEdges(true);

        JFJochProtoBuf::JungfraujochSettings settings_in_protobuf = x;
        REQUIRE_NOTHROW(y.Import(settings_in_protobuf));

        REQUIRE(! y.IsUpsideDown());
        REQUIRE(y.GenerateMasterFilename() == x.GenerateMasterFilename());
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
        REQUIRE(y.GetTransmission() == 0.5);
        REQUIRE(y.GetTotalFlux() == 1e9);

        REQUIRE(y.IsDetectorFullSpeed());
        REQUIRE(x.GetFrameNum() == y.GetFrameNum());
        REQUIRE(x.GetImageTime() == y.GetImageTime());
        REQUIRE(y.GetFrameCountTime().count() == 10);
        REQUIRE(y.GetRotationAxis().x == Approx(Coord(3,4,5).Normalize().x));
        REQUIRE(y.GetRotationAxis().y == Approx(Coord(3,4,5).Normalize().y));
        REQUIRE(y.GetRotationAxis().z == Approx(Coord(3,4,5).Normalize().z));
        REQUIRE(y.GetDestUDPPort(0,0) == 64*76);
        REQUIRE(y.GetDestIPv4Address(0) == 0x02020202);
        REQUIRE(y.GetTimeResolvedMode() == x.GetTimeResolvedMode());
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
    a.SampleTemperature(150);
    REQUIRE(a.GetSampleTemperature() == 150.0);
    REQUIRE(a.GetDetectorMode() == DetectorMode::Raw);

    DiffractionExperiment b(a);
    REQUIRE(b.GetSampleTemperature() == 150.0);
    b.SampleTemperature(100);
    REQUIRE(a.GetSampleTemperature() == 150.0);
    REQUIRE(b.GetSampleTemperature() == 100.0);
    REQUIRE(b.GetDetectorMode() == DetectorMode::Raw);

    DiffractionExperiment c = b;
    REQUIRE (c.GetSampleTemperature() == 100.0);
    c.SampleTemperature(10);
    REQUIRE (a.GetSampleTemperature() == 150.0);
    REQUIRE (b.GetSampleTemperature() == 100.0);
    REQUIRE (c.GetSampleTemperature() == 10.0);
}

TEST_CASE("DiffractionExperiment_ResToPxl","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.DetectorDistance_mm(75).Wavelength_A(1.0);

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

TEST_CASE("DiffractionExperiment_RadialIntegration_BkgEstimate","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);

    x.LowResForBkgEstimation_A(32.0);
    x.HighResForBkgEstimation_A(14.0);

    REQUIRE(x.GetHighQLimitForBkg_recipA() == Approx(2 * M_PI / 14.0));
    REQUIRE(x.GetLowQLimitForBkg_recipA() == Approx(2 * M_PI / 32.0));

    DiffractionExperiment y(x);
    REQUIRE(y.GetHighQLimitForBkg_recipA() == Approx(2 * M_PI / 14.0));
    REQUIRE(y.GetLowQLimitForBkg_recipA() == Approx(2 * M_PI / 32.0));
}

TEST_CASE("DiffractionExperiment_BkgEstPeriod","[DiffractionExperiment]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.DetectorDistance_mm(75).Wavelength_A(1.0);
    x.ImageTime(3ms);
    x.BackgroundEstimationPeriod(9ms);
    REQUIRE(x.GetBackgroundEstimationPeriod() == std::chrono::milliseconds(9));
    REQUIRE(x.GetBackgroundEstimationStride() == 3);
}

TEST_CASE("DiffractionExperiment_StorageCells","[DiffractionExperiment]") {
    const int64_t num_triggers = 20;
    DiffractionExperiment x;
    x.ImageTime(std::chrono::milliseconds(10)).ImagesPerTrigger(5).NumTriggers(num_triggers);
    REQUIRE(x.GetSummation() > 1);
    REQUIRE(x.GetImageNumPerTrigger() == 5);
    REQUIRE(x.GetNumTriggers() == num_triggers);
    REQUIRE(!x.GetTimeResolvedMode());

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
    REQUIRE(x.GetTimeResolvedMode());
    REQUIRE(x.GetImagesPerFile() == num_triggers);
    REQUIRE(x.GetFilesNum() == 16);

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