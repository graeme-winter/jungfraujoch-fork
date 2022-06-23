// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochCompressor.h"

#include <bitshuffle/bitshuffle_internals.h>
#include <zstd.h>
#include <lz4/lz4.h>

#include "JFJochException.h"


extern "C" {
void bshuf_write_uint64_BE(void *buf, uint64_t num);
void bshuf_write_uint32_BE(void *buf, uint32_t num);
}


JFJochBitShuffleCompressor::JFJochBitShuffleCompressor(const DiffractionExperiment &experiment) {
    elem_size = sizeof(uint32_t);
    exp_block_size = experiment.GetCompressionBlockSize();
    algorithm = experiment.GetCompressionAlgorithm();
    level = experiment.GetCompressionLevel();
}

size_t JFJochBitShuffleCompressor::CompressBlock(char *dest, const char *source, size_t nelements) {
// Assert nelements < block_size
    const char *src_ptr;

    size_t bshuf_ret = bshuf_trans_bit_elem(source, tmp_space.data(), nelements, elem_size);
    if (bshuf_ret < 0)
        throw JFJochException(JFJochExceptionCategory::Compression, "bshuf_trans_bit_elem error");
    src_ptr = tmp_space.data();

    size_t compressed_size;
    size_t src_size = nelements * elem_size;

    switch (algorithm) {
        case CompressionAlgorithm::BSHUF_LZ4:
            compressed_size = LZ4_compress_fast(src_ptr, dest + 4, src_size, LZ4_compressBound(src_size), level);
            break;
        case CompressionAlgorithm::BSHUF_ZSTD:
            if (level == ZSTD_USE_JFJOCH_RLE) {
                try {
                    compressed_size = zstd_compressor.Compress(((uint8_t *) dest) + 4, (uint64_t *) src_ptr,
                                                               src_size, src_size);
                } catch (const std::runtime_error &e) {
                    throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, e.what());
                }
            } else
                compressed_size = ZSTD_compress(dest + 4, ZSTD_compressBound(src_size),
                                                src_ptr, src_size, level);
                if (ZSTD_isError(compressed_size))
                    throw(JFJochException(JFJochExceptionCategory::Compression, ZSTD_getErrorName(compressed_size)));
            break;
        default:
            compressed_size = 0;
    }

    if (compressed_size == 0)
        throw JFJochException(JFJochExceptionCategory::Compression, "Algorithm not supported");

    bshuf_write_uint32_BE(dest, compressed_size);

    return compressed_size + 4;
}

size_t JFJochBitShuffleCompressor::Compress(char *dest, const char *source, size_t nelements, size_t in_elem_size) {
    elem_size = in_elem_size;

    if (exp_block_size == 0)
        block_size = bshuf_default_block_size(elem_size);
    else
        block_size = exp_block_size;

    if (tmp_space.size() < block_size * elem_size)
        tmp_space.resize(block_size * elem_size);

    if (block_size % BSHUF_BLOCKED_MULT)
        throw JFJochException(JFJochExceptionCategory::Compression, "Block size must be multiple of 8");

    size_t num_full_blocks = nelements / block_size;
    size_t reminder_size = nelements - num_full_blocks * block_size;
    size_t compressed_size = 0;

    for (int i = 0; i < num_full_blocks; i++)
        compressed_size += CompressBlock(dest + compressed_size,
                                         source + i * block_size * elem_size, block_size);

    size_t last_block_size = reminder_size - reminder_size % BSHUF_BLOCKED_MULT;
    if (last_block_size > 0)
        compressed_size += CompressBlock(dest + compressed_size,
                                         source + num_full_blocks * block_size * elem_size, last_block_size);

    size_t leftover_bytes = (reminder_size % BSHUF_BLOCKED_MULT) * elem_size;
    if (leftover_bytes > 0) {
        memcpy(dest + compressed_size, source + (num_full_blocks * block_size + last_block_size) * elem_size, leftover_bytes);
        compressed_size += leftover_bytes;
    }
    return compressed_size;
}

void BitShuffleCompressionHeader(char *dest, size_t npixel, size_t block_size, size_t elem_size) {
    bshuf_write_uint64_BE(dest, npixel * elem_size);
    bshuf_write_uint32_BE(dest + 8, block_size * elem_size);
}

JFJochDecompressor::JFJochDecompressor(const DiffractionExperiment &experiment) :
block_size(experiment.GetCompressionBlockSize()),
algorithm(experiment.GetCompressionAlgorithm()) {}

const char *JFJochDecompressor::Decompress(char *source, size_t source_size, size_t nelements, size_t elem_size) {
    if (decompression_buffer.size() < nelements * elem_size)
        decompression_buffer.resize(nelements * elem_size);

    int64_t ret;
    switch (algorithm) {
        case CompressionAlgorithm::None:
            return source;
        case CompressionAlgorithm::BSHUF_LZ4:
           if (bshuf_decompress_lz4(source + 12, decompression_buffer.data(), nelements,
                                    elem_size, block_size) != source_size - 12)
               throw JFJochException(JFJochExceptionCategory::Compression, "Decompression error");
           return decompression_buffer.data();
        case CompressionAlgorithm::BSHUF_ZSTD:
            ret = bshuf_decompress_zstd(source + 12, decompression_buffer.data(), nelements,
                                        elem_size, block_size);
            if (ret != source_size - 12)
                throw JFJochException(JFJochExceptionCategory::Compression, "Decompression error: "
                                                                            + std::to_string(ret) + " " + std::to_string(source_size - 12));
            return decompression_buffer.data();
        default:
            throw JFJochException(JFJochExceptionCategory::Compression, "Not implemented algorithm");
    }
}

