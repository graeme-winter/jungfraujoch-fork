// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <bitshuffle/bitshuffle.h>
#include <zstd.h>
#include <iostream>

#include "../compression/JFJochCompressor.h"
#include "../compression/JFJochDecompress.h"
#include "../common/DiffractionExperiment.h"

TEST_CASE("JFjochZstdCompressor_Raw_Block","[ZSTD]") {
    uint8_t src[128*8];
    uint8_t dst[128*8+3];

    for (int i = 0; i < 128*8; i++)
        src[i] = i % 256;

    size_t dst_size = JFJochZstdCompressor::RawBlock(dst, src, 128 * 8, false);

    REQUIRE(dst_size == 128*8+3);
    int diff = 0;
    for (int i = 0; i < 128*8; i++) {
        if (dst[i+3] != i % 256) diff++;
    }
    REQUIRE(diff == 0);
}

TEST_CASE("JFjochZstdCompressor_RLE_Block","[ZSTD]") {
    uint8_t dst[4];
    size_t dst_size = JFJochZstdCompressor::RLEBlock(dst, 0xAF, 128 * 8, false);

    REQUIRE(dst_size == 4);
    REQUIRE(dst[3] == 0xAF);
}

TEST_CASE("JFjochZstdCompressor_Frame_onlyRLE","[ZSTD]") {
    JFJochZstdCompressor compressor;
    uint8_t src[128 * 8];
    uint8_t dst[128 * 8 * 2];
    uint8_t decomp_buffer[128 * 8 * 2];

    for (int i = 0; i < 128 * 8; i++) {
        src[i] = 0;
        dst[i] = 0xFF;
    }

    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, 128*8, 128*8);
    REQUIRE(ZSTD_decompress(decomp_buffer, 128 * 8 * 2, dst, dst_size) == 8 * 128);
    size_t diff = 0;
    for (int i = 0; i < 128 * 8; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);

    REQUIRE(dst_size == 4 + 9);
    REQUIRE(dst[9 + 3] == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_onlyRLE_1block","[ZSTD]") {
    JFJochZstdCompressor compressor;
    uint8_t src[ZSTD_BLOCKSIZE_MAX * 2];
    uint8_t dst[ZSTD_BLOCKSIZE_MAX * 4];
    uint8_t decomp_buffer[ZSTD_BLOCKSIZE_MAX * 4];

    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX * 2; i++) {
        src[i] = 0;
        dst[i] = 0xFF;
    }

    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, ZSTD_BLOCKSIZE_MAX, ZSTD_BLOCKSIZE_MAX);
    REQUIRE(dst_size == 4 + 9);
    REQUIRE(dst[9 + 3] == 0);

    REQUIRE(ZSTD_decompress(decomp_buffer, ZSTD_BLOCKSIZE_MAX * 4, dst, dst_size) == ZSTD_BLOCKSIZE_MAX);
    size_t diff = 0;
    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_onlyRLE_2blocks","[ZSTD]") {
    JFJochZstdCompressor compressor;
    uint8_t src[ZSTD_BLOCKSIZE_MAX * 2];
    uint8_t dst[ZSTD_BLOCKSIZE_MAX * 4];
    uint8_t decomp_buffer[ZSTD_BLOCKSIZE_MAX * 4];

    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX * 2; i++) {
        src[i] = 0;
        dst[i] = 0xFF;
    }

    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, ZSTD_BLOCKSIZE_MAX * 2, ZSTD_BLOCKSIZE_MAX * 2);
    REQUIRE(dst_size == 8 + 9);
    REQUIRE(dst[9 + 3] == 0);
    REQUIRE(dst[9 + 7] == 0);

    REQUIRE(ZSTD_decompress(decomp_buffer, ZSTD_BLOCKSIZE_MAX * 4, dst, dst_size) == ZSTD_BLOCKSIZE_MAX * 2);
    size_t diff = 0;
    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX * 2; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_onlyRLE_0xFF_2blocks","[ZSTD]") {
    JFJochZstdCompressor compressor;
    uint8_t src[ZSTD_BLOCKSIZE_MAX * 2];
    uint8_t dst[ZSTD_BLOCKSIZE_MAX * 4];
    uint8_t decomp_buffer[ZSTD_BLOCKSIZE_MAX * 4];

    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX * 2; i++) {
        src[i] = 0xFF;
        dst[i] = 0x0;
    }

    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, ZSTD_BLOCKSIZE_MAX * 2, ZSTD_BLOCKSIZE_MAX * 2);
    REQUIRE(dst_size == 8 + 9);
    REQUIRE(dst[9 + 3] == 0xFF);
    REQUIRE(dst[9 + 7] == 0xFF);

    REQUIRE(ZSTD_decompress(decomp_buffer, ZSTD_BLOCKSIZE_MAX * 4, dst, dst_size) == ZSTD_BLOCKSIZE_MAX * 2);
    size_t diff = 0;
    for (int i = 0; i < ZSTD_BLOCKSIZE_MAX * 2; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);
}


TEST_CASE("JFjochZstdCompressor_Frame_onlyRAW","[ZSTD]") {
    JFJochZstdCompressor compressor;

    uint8_t src[128 * 8];
    uint8_t dst[128 * 8 * 2];
    uint8_t decomp_buffer[128 * 8 * 2];
    for (int i = 0; i < 128 * 8; i++)
        src[i] = 1 + i % 128;
    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, 128 * 8, 128*8);
    REQUIRE(ZSTD_decompress(decomp_buffer, 128 * 8 * 2, dst, dst_size) == 8 * 128);
    size_t diff = 0;
    for (int i = 0; i < 128 * 8; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);

    REQUIRE(dst_size == 3 + 9 + 128 * 8);

    diff = 0;
    for (int i = 0; i < 128 * 8; i++) {
        if (dst[i + 3 + 9] != 1 + i % 128)
            diff++;
    }
    REQUIRE(diff == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_mixed","[ZSTD]") {
    JFJochZstdCompressor compressor;

    uint8_t src[128 * 8];
    uint8_t dst[128 * 8 * 2];
    uint8_t decomp_buffer[128 * 8 * 2];

    for (int i = 0; i < 128*8; i++)
        src[i] = (i / 128 + 1) % 2 ;
    size_t dst_size = compressor.Compress(dst, (uint64_t *) src, 128 * 8, 128 * 8);
    REQUIRE(dst_size == 9 + 4 * 4 + 4 * (3+128));

    REQUIRE(ZSTD_decompress(decomp_buffer, 128*8*2, dst, dst_size) == 8 * 128);
    size_t diff = 0;
    for (int i = 0; i < 128*8; i++) if (src[i] != decomp_buffer[i]) diff++;
    REQUIRE(diff == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_simulated","[ZSTD]") {
    for(int seed = 0; seed < 200; seed++) {
        JFJochZstdCompressor compressor;

        // Predictable random number generator
        std::mt19937 g1(seed);
        std::uniform_int_distribution<int16_t> distribution;

        std::vector<int16_t> input(RAW_MODULE_SIZE);
        for (auto &i: input) i = distribution(g1);

        std::vector<char> input_shuffled(RAW_MODULE_SIZE * sizeof(uint16_t));
        bshuf_bitshuffle(input.data(), input_shuffled.data(), RAW_MODULE_SIZE, 2, 4096);
        std::vector<uint8_t> input_compressed(ZSTD_compressBound(RAW_MODULE_SIZE * sizeof(uint16_t)));
        std::vector<uint8_t> output(RAW_MODULE_SIZE * sizeof(uint16_t) * 2);

        size_t dst_size = compressor.Compress(input_compressed.data(), (uint64_t *) input_shuffled.data(),
                                              RAW_MODULE_SIZE * sizeof(uint16_t), RAW_MODULE_SIZE * sizeof(uint16_t));

        REQUIRE(ZSTD_decompress(output.data(), RAW_MODULE_SIZE * sizeof(uint16_t) * 2,
                                input_compressed.data(), dst_size) == RAW_MODULE_SIZE * sizeof(uint16_t));

        REQUIRE(memcmp(input_shuffled.data(), output.data(), RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
    }
}

TEST_CASE("JFjochZstdCompressor_Frame_zeroes","[ZSTD]") {
    JFJochZstdCompressor compressor;

    std::vector<char> input_shuffled(RAW_MODULE_SIZE * sizeof(uint16_t));
    for (auto &i: input_shuffled) i = 0x0u;

    std::vector<uint8_t> input_compressed(ZSTD_compressBound(RAW_MODULE_SIZE * sizeof(uint16_t)));
    std::vector<uint8_t> output(RAW_MODULE_SIZE * sizeof(uint16_t) * 2);

    size_t dst_size = compressor.Compress(input_compressed.data(), (uint64_t *) input_shuffled.data(),
                                          RAW_MODULE_SIZE * sizeof(uint16_t), RAW_MODULE_SIZE * sizeof(uint16_t));
    REQUIRE(dst_size == 9 + 4 * 8);
    REQUIRE(ZSTD_decompress(output.data(), RAW_MODULE_SIZE * sizeof(uint16_t) * 2,
                            input_compressed.data(), dst_size) == RAW_MODULE_SIZE * sizeof(uint16_t));

    REQUIRE(memcmp(input_shuffled.data(), output.data(), RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
}

TEST_CASE("JFjochZstdCompressor_Frame_ones","[ZSTD]") {
    JFJochZstdCompressor compressor;

    std::vector<char> input_shuffled(RAW_MODULE_SIZE * sizeof(uint16_t));
    for (auto &i: input_shuffled) i = 0xFFu;

    std::vector<uint8_t> input_compressed(ZSTD_compressBound(RAW_MODULE_SIZE * sizeof(uint16_t)));
    std::vector<uint8_t> output(RAW_MODULE_SIZE * sizeof(uint16_t) * 2);

    size_t dst_size = compressor.Compress(input_compressed.data(), (uint64_t *) input_shuffled.data(),
                                          RAW_MODULE_SIZE * sizeof(uint16_t), RAW_MODULE_SIZE * sizeof(uint16_t));
    REQUIRE(dst_size == 9 + 4 * 8);
    REQUIRE(ZSTD_decompress(output.data(), RAW_MODULE_SIZE * sizeof(uint16_t) * 2,
                            input_compressed.data(), dst_size) == RAW_MODULE_SIZE * sizeof(uint16_t));

    REQUIRE(memcmp(input_shuffled.data(), output.data(), RAW_MODULE_SIZE * sizeof(uint16_t)) == 0);
}

TEST_CASE("JFJochCompressor_JFJochDecompressor_ZSTD","[ZSTD]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.Compression(JFJochProtoBuf::BSHUF_ZSTD).Summation(34);

    std::vector<int32_t> image(x.GetPixelsNum());

    for (auto &i: image)
        i = 345;

    JFJochBitShuffleCompressor compressor(x.GetCompressionAlgorithmEnum());

    std::vector<char> tmp(x.GetPixelsNum() * sizeof(int32_t) * 4 + 12);

    auto tmp_size = compressor.Compress(tmp.data(), image);
    tmp.resize(tmp_size);

    std::vector<int32_t> output;
    REQUIRE_NOTHROW(JFJochDecompress(output, x.GetCompressionAlgorithmEnum(), tmp, x.GetPixelsNum()));
    REQUIRE(output.size() == x.GetPixelsNum() * x.GetPixelDepth());
    REQUIRE(memcmp(image.data(), output.data(), x.GetPixelsNum() * sizeof(int32_t)) == 0);
}

TEST_CASE("JFJochCompressor_JFJochDecompressor_LZ4","[ZSTD]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.Compression(JFJochProtoBuf::BSHUF_LZ4).Summation(45);

    std::vector<int32_t> image(x.GetPixelsNum());

    for (auto &i: image)
        i = 5678;

    JFJochBitShuffleCompressor compressor(x.GetCompressionAlgorithmEnum());

    std::vector<char> tmp(x.GetPixelsNum() * sizeof(int32_t) * 4 + 12);

    auto tmp_size = compressor.Compress(tmp.data(), image);
    tmp.resize(tmp_size);

    std::vector<int32_t> output;
    REQUIRE_NOTHROW(JFJochDecompress(output, x.GetCompressionAlgorithmEnum(), tmp, x.GetPixelsNum()));
    REQUIRE(output.size() == x.GetPixelsNum() * x.GetPixelDepth());
    REQUIRE(memcmp(image.data(), output.data(), x.GetPixelsNum() * sizeof(int32_t)) == 0);
}

TEST_CASE("JFJochDecompressor_None","[ZSTD]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.Compression(JFJochProtoBuf::NO_COMPRESSION).Summation(45);

    std::vector<int32_t> image(x.GetPixelsNum());
    for (auto &i: image)
        i = 578;

    std::vector<int32_t> output;
    REQUIRE_NOTHROW(JFJochDecompress(output, x.GetCompressionAlgorithmEnum(), image, x.GetPixelsNum()));
    REQUIRE(output.size() == x.GetPixelsNum() * x.GetPixelDepth());
    REQUIRE(memcmp(image.data(), output.data(), x.GetPixelsNum() * sizeof(int32_t)) == 0);
}

TEST_CASE("Bitshuffle_ZSTD","[ZSTD]") {
    std::vector<int32_t> image(RAW_MODULE_SIZE * sizeof(int32_t));
    std::vector<char> compressed(bshuf_compress_zstd_bound(RAW_MODULE_SIZE, 4, 0));
    std::vector<int32_t> decompressed(RAW_MODULE_SIZE * sizeof(int32_t));
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        image[i] = i;

    auto out_size = bshuf_compress_zstd(image.data(), compressed.data(), RAW_MODULE_SIZE, 4, 0,0);
    REQUIRE(out_size > 0);
    REQUIRE(bshuf_decompress_zstd(compressed.data(), decompressed.data(), RAW_MODULE_SIZE, 4, 0) == out_size);

    REQUIRE(memcmp(image.data(), decompressed.data(), RAW_MODULE_SIZE*sizeof(uint32_t)) == 0);
}