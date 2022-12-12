// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <bitshuffle/bitshuffle.h>

#include "JFJochCompressor.h"
#include "MaxCompressedSize.h"

int64_t MaxCompressedSize(CompressionAlgorithm algorithm, int64_t pixels_number, uint16_t pixel_depth) {
    switch (algorithm) {
        case CompressionAlgorithm::BSHUF_LZ4:
            return bshuf_compress_lz4_bound(pixels_number, pixel_depth, JFJochBitShuffleCompressor::DefaultBlockSize) + 12;
        case CompressionAlgorithm::BSHUF_ZSTD:
        case CompressionAlgorithm::BSHUF_ZSTD_RLE:
            return bshuf_compress_zstd_bound(pixels_number, pixel_depth,JFJochBitShuffleCompressor::DefaultBlockSize) + 12;
        default:
            return pixels_number * pixel_depth;
    }
}

