// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H
#define JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H

#include <bitshuffle/bitshuffle.h>

#include <vector>
#include <cstdint>
#include <cstddef>
#include "CompressionAlgorithmEnum.h"

#include "JFJochZstdCompressor.h"

#define ZSTD_USE_JFJOCH_RLE 0xFFFF
#define LZ4_BLOCK_SIZE  4096
#define ZSTD_BLOCK_SIZE 4096

class JFJochBitShuffleCompressor {
    JFJochZstdCompressor zstd_compressor;
    CompressionAlgorithm algorithm;
    int level;
    size_t exp_block_size;
    std::vector<char> tmp_space;
    size_t CompressBlock(char *dest, const char * source, size_t nelements, size_t elem_size);
public:
    JFJochBitShuffleCompressor(CompressionAlgorithm algorithm, size_t exp_block_size, int level);
    template<class T>
    size_t Compress(void *dest, const std::vector<T> &src) {
        return Compress((char *) dest, (char *) src.data(), src.size(), sizeof(T));
    };
    size_t Compress(char *dest, const char* source, size_t nelements, size_t elem_size);
};

template <class T> std::vector<T> bitshuffle(const std::vector<T> &input, size_t block_size) {
    std::vector<T> ret(input.size() * sizeof(T));
    bshuf_bitshuffle(input.data(), ret.data(), input.size(), sizeof(T), block_size);
    return ret;
}



#endif //JUNGFRAUJOCH_JFJOCHCOMPRESSOR_H
