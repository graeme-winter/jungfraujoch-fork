// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <iostream>
#include <bitshuffle/bitshuffle.h>

#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"

using namespace std::literals::chrono_literals;

extern "C" {
uint32_t bshuf_read_uint32_BE(const void* buf);
uint64_t bshuf_read_uint64_BE(const void* buf);
}

TEST_CASE("Bshuf_SSE", "[bitshuffle]") {
    REQUIRE (bshuf_using_SSE2() == 1);
}

TEST_CASE("FrameTransformation_Raw_NoCompression" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Raw).DataStreamModuleSize(1,{nmodules, nmodules});
    experiment.Compression(CompressionAlgorithm::None);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(1587);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<int16_t> output(experiment.GetPixelsNum());

    transformation.SetOutput(output.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    REQUIRE(transformation.PackStandardOutput() == experiment.GetPixelDepth() * experiment.GetPixelsNum());

    uint32_t diff_0 = 0;
    uint32_t diff_1 = 0;

    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++) {
        if (input_0[i] != output[i]) diff_0++;
        if (input_1[i] != output[i + nmodules*RAW_MODULE_SIZE]) diff_1++;
    }
    REQUIRE(diff_0 == 0);
    REQUIRE(diff_1 == 0);
}

TEST_CASE("FrameTransformation_Converted_NoCompression" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::None);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(1687);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<int16_t> output(experiment.GetPixelsNum());

    transformation.SetOutput(output.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    REQUIRE(transformation.PackStandardOutput() == experiment.GetPixelDepth() * experiment.GetPixelsNum());

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(input_0[511*1024+256]/2       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(input_0[256*1024+256]/4       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);

    REQUIRE(input_0[311*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(input_0[311*1024+256] / 2     == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);

    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(input_1[511*1024+256] /2      == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(input_1[(311+2*512)*1024]           == output[200 * 1030 * 2 + 0]);
    REQUIRE(input_1[(311+2*512)*1024+512] / 2   == output[200 * 1030 * 2 + 256*2+3]);

    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformation_Converted_bshuf_lz4" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::BSHUF_LZ4);

    FrameTransformation transformation(experiment);

    // Predictable random number generator
    std::mt19937 g1(23433);
    std::uniform_int_distribution<int16_t> distribution;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = distribution(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = distribution(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());

    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int16_t *output;
    REQUIRE_NOTHROW(output = (int16_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(input_0[511*1024+256]/2       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(input_0[256*1024+256]/4       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);

    REQUIRE(input_0[311*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(input_0[311*1024+256] / 2     == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);

    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(input_1[511*1024+256] /2      == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(input_1[(311+2*512)*1024]           == output[200 * 1030 * 2 + 0]);
    REQUIRE(input_1[(311+2*512)*1024+512] / 2   == output[200 * 1030 * 2 + 256*2+3]);

    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformation_Converted_bshuf_zstd" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(1987);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());


    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int16_t *output;
    REQUIRE_NOTHROW(output = (int16_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(input_0[511*1024+256]/2       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(input_0[256*1024+256]/4       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);

    REQUIRE(input_0[311*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(input_0[311*1024+256] / 2     == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);

    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(input_1[511*1024+256] /2      == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(input_1[(311+2*512)*1024]           == output[200 * 1030 * 2 + 0]);
    REQUIRE(input_1[(311+2*512)*1024+512] / 2   == output[200 * 1030 * 2 + 256*2+3]);

    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}


TEST_CASE("FrameTransformation_Converted_bshuf_zstd_mask_chip_edges" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).Compression(CompressionAlgorithm::BSHUF_ZSTD).MaskChipEdges(true);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(1987);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());


    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int16_t *output;
    REQUIRE_NOTHROW(output = (int16_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);

    REQUIRE(input_0[311*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 0]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 200 * 1030 * 2 + 258]);

    REQUIRE(input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(0                             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(input_1[(311+2*512)*1024]     == output[200 * 1030 * 2 + 0]);
    REQUIRE(0                             == output[200 * 1030 * 2 + 256*2+3]);

    REQUIRE(input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformation_Converted_bshuf_zstd_summation" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).ImageTime(4ms).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    REQUIRE(experiment.GetSummation() == 4);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(15097);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());

    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 1,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 1,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 2,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 2,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 3,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 3,i, 1));
    }

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int32_t *output;
    REQUIRE_NOTHROW(output = (int32_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(4 * input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(2 * input_0[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(    input_0[256*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);
    REQUIRE(4 * input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(4 * input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(4 * input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(2 * input_1[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(4 * input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(4 * input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformation_Converted_16bit_preview_summation" ,"") {
    uint16_t nmodules = 1;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(1,{nmodules}, 0, 0)
            .UpsideDown(false).ImageTime(4ms).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    REQUIRE(experiment.GetSummation() == 4);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(15097);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(RAW_MODULE_SIZE, 55);
    input_0[0] = 10000;
    input_0[1] = 500;
    input_0[2] = INT16_MIN;
    input_0[3] = INT16_MAX;
    input_0[4] = 345;
    input_0[5] = 0;
    input_0[256*1024] = 10000;

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());

    transformation.SetOutput(output_compressed.data());

    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 0,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 1,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 2,0, 0));
    REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data(), 3,0, 0));

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int32_t *output;
    REQUIRE_NOTHROW(output = (int32_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    auto output16 = transformation.GetPreview16BitImage();

    REQUIRE(output16[0] == INT16_MAX);
    REQUIRE(output[0]   == 4 * 10000);

    REQUIRE(output16[1] == 4 * 500);
    REQUIRE(output[1]   == 4 * 500);

    REQUIRE(output16[2] == INT16_MIN);
    REQUIRE(output[2]   == experiment.GetUnderflow());

    REQUIRE(output16[3] == INT16_MAX);
    REQUIRE(output[3]   == experiment.GetOverflow());

    REQUIRE(output16[4] == 4*345);
    REQUIRE(output[4]   == 4*345);

    REQUIRE(output16[5] == 0);
    REQUIRE(output[5]   == 0);

    REQUIRE(output16[1030*257] == 4 * 5000);
    REQUIRE(output[1030*257]   == 4 * 5000);
}

TEST_CASE("FrameTransformation_Converted_bshuf_zstd_summation_2frames" ,"") {
    // Ensure that previous frame is properly cleaned with PackStandardOutput()
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).ImageTime(4ms).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    REQUIRE(experiment.GetSummation() == 4);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(15097);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());

    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 2,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 2,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 2,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 2,i, 1));
    }

    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 1,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 1,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 2,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 2,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 3,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 3,i, 1));
    }

    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int32_t *output;
    REQUIRE_NOTHROW(output = (int32_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(4 * input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(2 * input_0[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(    input_0[256*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);
    REQUIRE(4 * input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(4 * input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(4 * input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(2 * input_1[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(4 * input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(4 * input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}

TEST_CASE("FrameTransformation_Converted_bshuf_zstd_summation_negatives" ,"") {
    uint16_t nmodules = 4;

    DiffractionExperiment experiment;

    experiment.Mode(DetectorMode::Conversion).DataStreamModuleSize(2,{nmodules, nmodules}, 0, 0)
            .UpsideDown(true).ImageTime(4ms).Compression(CompressionAlgorithm::BSHUF_ZSTD);

    REQUIRE(experiment.GetSummation() == 4);

    FrameTransformation transformation(experiment);

    std::mt19937 g1(12387);
    std::uniform_int_distribution<int16_t> dist;

    std::vector<int16_t> input_0(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_0[i] = dist(g1);

    std::vector<int16_t> input_1(nmodules*RAW_MODULE_SIZE);
    for (int i = 0; i < nmodules*RAW_MODULE_SIZE; i++)
        input_1[i] = dist(g1);

    std::vector<char> output_compressed(experiment.GetMaxCompressedSize());

    transformation.SetOutput(output_compressed.data());

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 0,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 0,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 1,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 1,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 2,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 2,i, 1));
    }

    for (int i = 0; i < nmodules; i++) {
        REQUIRE_NOTHROW(transformation.ProcessModule(input_0.data() + i * RAW_MODULE_SIZE, 3,i, 0));
        REQUIRE_NOTHROW(transformation.ProcessModule(input_1.data() + i * RAW_MODULE_SIZE, 3,i, 1));
    }
    size_t compressed_size;
    REQUIRE_NOTHROW(compressed_size = transformation.PackStandardOutput());

    REQUIRE(bshuf_read_uint64_BE(output_compressed.data()) == experiment.GetPixelsNum() * experiment.GetPixelDepth());
    REQUIRE(bshuf_read_uint32_BE(output_compressed.data()+8) == experiment.GetCompressionBlockSize() * experiment.GetPixelDepth());

    JFJochDecompressor decompressor(experiment);
    int32_t *output;
    REQUIRE_NOTHROW(output = (int32_t *) decompressor.Decompress(output_compressed.data(), compressed_size,
                                                                 experiment.GetPixelsNum(), experiment.GetPixelDepth()));

    REQUIRE(4 * input_0[511*1024]             == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 0]);
    REQUIRE(2 * input_0[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 258]);
    REQUIRE(    input_0[256*1024+256]         == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 2 * 1030 * 255 + 257]);

    REQUIRE(4 * input_0[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030]);
    REQUIRE(4 * input_0[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (2 * nmodules - 2) + 1030 + 800 + 6]);

    REQUIRE(4 * input_1[511*1024]             == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 0]);
    REQUIRE(2 * input_1[511*1024+256]         == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 258]);

    REQUIRE(4 * input_1[(511+512)*1024]       == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030]);
    REQUIRE(4 * input_1[(511+512)*1024 + 800] == output[CONVERTED_MODULE_SIZE * (nmodules - 2) + 1030 + 800 + 6]);
}
