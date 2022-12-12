// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHDECOMPRESS_H
#define JUNGFRAUJOCH_JFJOCHDECOMPRESS_H

#include <vector>
#include <cstring>

#include <bitshuffle/bitshuffle.h>
#include <bitshuffle/bitshuffle_internals.h>

#include "../compression/CompressionAlgorithmEnum.h"

#include "../common/JFJochException.h"

extern "C" {
    uint64_t bshuf_read_uint64_BE(void* buf);
};

template <class Td, class Ts>
void JFJochDecompress(std::vector<Td> &output, CompressionAlgorithm algorithm, std::vector<Ts> source_v,
                      size_t nelements) {
    size_t elem_size = sizeof(Td);
    output.resize(nelements * elem_size);
    size_t source_size = source_v.size() * sizeof(Ts);
    auto source = (uint8_t *) source_v.data();

    size_t block_size;
    if (algorithm != CompressionAlgorithm::NO_COMPRESSION) {
        if (bshuf_read_uint64_BE(source) != nelements * elem_size)
            throw JFJochException(JFJochExceptionCategory::Compression, "Mismatch in size");
        auto tmp = bshuf_read_uint32_BE(source + 8);
        block_size = tmp / elem_size;
    }

    switch (algorithm) {
        case CompressionAlgorithm::NO_COMPRESSION:
            if (source_size != nelements * elem_size)
                throw JFJochException(JFJochExceptionCategory::Compression, "Mismatch in size");
            memcpy(output.data(), source, source_size);
            break;
        case CompressionAlgorithm::BSHUF_LZ4:
            if (bshuf_decompress_lz4(source + 12, output.data(), nelements,
                                     elem_size, block_size) != source_size - 12)
                throw JFJochException(JFJochExceptionCategory::Compression, "Decompression error");
            break;
        case CompressionAlgorithm::BSHUF_ZSTD_RLE:
        case CompressionAlgorithm::BSHUF_ZSTD:
            if (bshuf_decompress_zstd(source + 12, output.data(), nelements,
                                      elem_size, block_size) != source_size - 12)
                throw JFJochException(JFJochExceptionCategory::Compression, "Decompression error");
            break;
        default:
            throw JFJochException(JFJochExceptionCategory::Compression, "Not implemented algorithm");
    }
}


#endif //JUNGFRAUJOCH_JFJOCHDECOMPRESS_H
