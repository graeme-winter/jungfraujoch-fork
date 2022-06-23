// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHZSTDCOMPRESSOR_H
#define JUNGFRAUJOCH_JFJOCHZSTDCOMPRESSOR_H

#include <cstdint>
#include <cstddef>

class JFJochZstdCompressor {
    size_t BlockStateMachine(uint8_t *dst, const uint64_t *src, size_t frame_size64);
    size_t CompressFrame(uint8_t *dst, const uint64_t *src, size_t frame_size64);
    size_t DataBlock(uint8_t *dst, const void *src, uint32_t src_size, bool last);
public:
    JFJochZstdCompressor();
    static size_t RawBlock(uint8_t *dst, const void *src, uint32_t src_size, bool last);
    static size_t RLEBlock(uint8_t *dst, uint8_t src, uint32_t src_size, bool last);
    size_t Compress(uint8_t *dst, const uint64_t *src, size_t src_size, size_t frame_sizes);
};


#endif //JUNGFRAUJOCH_JFJOCHZSTDCOMPRESSOR_H
