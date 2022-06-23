// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochZstdCompressor.h"

#include <map>
#include <zstd.h>
#include <lz4/lz4.h>

#include <common/mem.h>
#include <stdexcept>

enum class CompressorState {RLE_0, RLE_FF, RAW};

#define BLOCK_RAW           0
#define BLOCK_RLE           1
#define BLOCK_COMP          2
#define LITERAL_COMPRESSED  2
#define LITERAL_TREELESS    3

JFJochZstdCompressor::JFJochZstdCompressor() {
    unsigned count[256];
    for (int i = 0; i < 256; i++) count[i] = i;
}

size_t JFJochZstdCompressor::RawBlock(uint8_t *dst, const void *src, uint32_t src_size, bool last) {
    size_t ret_value = 0;
    int32_t bytes_left = src_size;
    auto src_u8 = (uint8_t *)src;

    while (bytes_left > 0) {
        int32_t bytes_to_write = std::min(bytes_left,ZSTD_BLOCKSIZE_MAX);
        bytes_left -= bytes_to_write;
        uint32_t block_header = (bytes_to_write << 3) + (BLOCK_RAW<<1) + (((last && (bytes_left == 0)) ? 1 : 0));

        MEM_writeLE24(dst, block_header);
        memcpy(dst + 3, src_u8, bytes_to_write);

        src_u8 += bytes_to_write;
        ret_value += 3 + bytes_to_write;
        dst += 3 + bytes_to_write;
    }
    return ret_value;
}

size_t JFJochZstdCompressor::RLEBlock(uint8_t *dst, uint8_t src, uint32_t src_size, bool last) {
    size_t ret_value = 0;
    int32_t bytes_left = src_size;

    while (bytes_left > 0) {
        int32_t bytes_to_write = std::min(bytes_left, ZSTD_BLOCKSIZE_MAX);
        bytes_left -= bytes_to_write;
        uint32_t block_header = (bytes_to_write << 3) + (BLOCK_RLE<<1) + ((last && (bytes_left == 0))? 1 : 0);
        uint32_t block = block_header + (src<<24); // Little Endian!
        MEM_writeLE32(dst + ret_value, block);
        ret_value += 4;
    }
    return ret_value;
}

size_t JFJochZstdCompressor::DataBlock(uint8_t *dst, const void *src, uint32_t src_size, bool last) {
    return RawBlock(dst, src, src_size, last);
}

size_t JFJochZstdCompressor::BlockStateMachine(uint8_t *dst, const uint64_t *src, size_t frame_size64) {
    size_t dst_size = 0;

    CompressorState state;
    size_t seq_len = 8;

    if (src[0] == 0x0) state = CompressorState::RLE_0;
    else if (src[0] == UINT64_MAX) state = CompressorState::RLE_FF;
    else state = CompressorState::RAW;

    for (int i = 1; i < frame_size64; i++) {
        switch (state) {
            case CompressorState::RLE_0:
                if (src[i] == 0x0) seq_len += 8;
                else if ((src[i] & 0x00000000000000FF) != 0) {
                    dst_size += RLEBlock(dst + dst_size, 0x0, seq_len, false);
                    state = CompressorState::RAW;
                    seq_len = 8;
                } else if (src[i] == UINT64_MAX) {
                    dst_size += RLEBlock(dst + dst_size, 0x0, seq_len, false);
                    state = CompressorState::RLE_FF;
                    seq_len = 8;
                } else {
                    int delta1 = 1;
                    if      ((src[i] & 0x00FFFFFFFFFFFFFF) == 0) delta1 = 7;
                    else if ((src[i] & 0x0000FFFFFFFFFFFF) == 0) delta1 = 6;
                    else if ((src[i] & 0x000000FFFFFFFFFF) == 0) delta1 = 5;
                    else if ((src[i] & 0x00000000FFFFFFFF) == 0) delta1 = 4;
                    else if ((src[i] & 0x0000000000FFFFFF) == 0) delta1 = 3;
                    else if ((src[i] & 0x000000000000FFFF) == 0) delta1 = 2;

                    dst_size += RLEBlock(dst + dst_size, 0x0, seq_len + delta1, false);
                    state = CompressorState::RAW;
                    seq_len = 8 - delta1;
                }
                break;
            case CompressorState::RLE_FF:
                if (src[i] == UINT64_MAX) seq_len += 8;
                else {
                    dst_size += RLEBlock(dst + dst_size, 0xFFu, seq_len, false);
                    seq_len = 8;
                    if (src[i] == 0x0u)
                        state = CompressorState::RLE_0;
                    else
                        state = CompressorState::RAW;
                }
                break;
            case CompressorState::RAW:
                if (src[i] == 0x0u) {
                    dst_size += DataBlock(dst + dst_size, (uint8_t *) (src + i) - seq_len,
                                          seq_len, false);
                    state = CompressorState::RLE_0;
                    seq_len = 8;
                } else if ((i < frame_size64 - 1) && (src[i+1] == 0x0u) && ((src[i] & 0xFF00000000000000) == 0)) {
                    int delta2 = 1;
                    if      ((src[i] & 0xFFFFFFFFFFFFFF00) == 0) delta2 = 7;
                    else if ((src[i] & 0xFFFFFFFFFFFF0000) == 0) delta2 = 6;
                    else if ((src[i] & 0xFFFFFFFFFF000000) == 0) delta2 = 5;
                    else if ((src[i] & 0xFFFFFFFF00000000) == 0) delta2 = 4;
                    else if ((src[i] & 0xFFFFFF0000000000) == 0) delta2 = 3;
                    else if ((src[i] & 0xFFFF000000000000) == 0) delta2 = 2;
                    dst_size += DataBlock(dst + dst_size, (uint8_t *) (src + i) - seq_len,
                                          seq_len + 8 - delta2, false);
                    state = CompressorState::RLE_0;
                    seq_len = delta2;
                }  else if (src[i] == UINT64_MAX) {
                    dst_size += DataBlock(dst + dst_size, (uint8_t *) (src + i) - seq_len,
                                          seq_len, false);
                    state = CompressorState::RLE_FF;
                    seq_len = 8;
                } else seq_len += 8;
                break;

        }
    }
    if (state == CompressorState::RLE_0)
        dst_size += RLEBlock(dst + dst_size, 0x0  , seq_len, true);
    else if (state == CompressorState::RLE_FF)
        dst_size += RLEBlock(dst + dst_size, 0xFFu, seq_len, true);
    else
        dst_size += DataBlock(dst + dst_size, (uint8_t *) (src + frame_size64) - seq_len, seq_len, true);

    return dst_size;
}

size_t JFJochZstdCompressor::CompressFrame(uint8_t *dst, const uint64_t *src, size_t frame_size64) {
    //if (frame_size64 > 1000 * 1000)
    //    throw SLSException(SLSExceptionCategory::ZSTDCompressionError,
    //                       "ZSTD frame cannot be larger than 8 MB");

    // Assumptions:
    // One frame = One module = 512*1024*2 (2^20 == 1048576) - 514*1030*4 (2117680 < 2 ^22)
    // --> frame size needs 32-bit

    size_t dst_size = 0;

    // Magic number
    MEM_writeLE32(dst, ZSTD_MAGICNUMBER);

    dst_size += 4;

    // Frame header descriptor
    uint8_t frame_header_descriptor = (1<<7) + (1<<5);
    dst[dst_size] = frame_header_descriptor;
    dst_size += 1;

    // Frame size
    MEM_writeLE32(dst + dst_size, frame_size64*8);
    dst_size += 4;

    dst_size += BlockStateMachine(dst + dst_size, src, frame_size64);

    return dst_size;
}

size_t JFJochZstdCompressor::Compress(uint8_t *dst, const uint64_t *src, size_t src_size, size_t frame_size) {
    size_t dst_size = 0;

    if (frame_size % 8 != 0)
        throw std::runtime_error("ZSTD frame has to have size multiple of 8");

    if (src_size % frame_size != 0)
        throw std::runtime_error("ZSTD source has to be multiple of frame");

    size_t nframes = src_size / frame_size;

    size_t frame_size64 = frame_size / 8;

    for (int frame = 0; frame < nframes; frame++)
        dst_size += CompressFrame(dst + dst_size, src + frame * frame_size64, frame_size64);

    return dst_size;
}