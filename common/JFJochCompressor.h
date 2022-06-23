// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H
#define JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H

#include <bitshuffle/bitshuffle.h>

#include <cstdint>
#include <cstddef>

#include "DiffractionExperiment.h"
#include "JFJochZstdCompressor.h"

void BitShuffleCompressionHeader(char *dest, size_t npixel, size_t block_size, size_t elem_size);

class JFJochBitShuffleCompressor {
    JFJochZstdCompressor zstd_compressor;
    CompressionAlgorithm algorithm;
    int level;
    size_t exp_block_size;
    size_t block_size;
    size_t elem_size;
    std::vector<char> tmp_space;
    size_t CompressBlock(char *dest, const char * source, size_t nelements);
public:
    explicit JFJochBitShuffleCompressor(const DiffractionExperiment &experiment);
    size_t Compress(char *dest, const char* source, size_t nelements, size_t elem_size);
};

template <class T> std::vector<T> bitshuffle(const std::vector<T> &input, size_t block_size) {
    std::vector<T> ret(input.size() * sizeof(T));
    bshuf_bitshuffle(input.data(), ret.data(), input.size(), sizeof(T), block_size);
    return ret;
}

class JFJochDecompressor {
    size_t block_size;
    CompressionAlgorithm algorithm;
    std::vector<char> decompression_buffer;
public:
    JFJochDecompressor(const DiffractionExperiment& experiment);
    const char *Decompress(char *source, size_t source_size, size_t nelements, size_t elem_size);
};

#endif //JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H
