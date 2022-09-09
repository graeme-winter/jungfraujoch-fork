// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFPedestal.h"

#include <numeric>
#include <bitshuffle/bitshuffle.h>

#include "JFJochException.h"

JFPedestal::JFPedestal()
        : pedestal(RAW_MODULE_SIZE, 0),
          pedestal_mask(RAW_MODULE_SIZE, 0),
          pedestal_rms(RAW_MODULE_SIZE, -1) {}

uint16_t& JFPedestal::operator[](size_t val) {
    return pedestal.at(val);
}

const uint16_t& JFPedestal::operator[](size_t val) const {
    return pedestal.at(val);
}

int16_t &JFPedestal::RMS(size_t val) {
    return pedestal_rms.at(val);
}

const int16_t &JFPedestal::RMS(size_t val) const {
    return pedestal_rms.at(val);
}

uint8_t &JFPedestal::Mask(size_t val) {
    return pedestal_mask.at(val);
}

const uint8_t &JFPedestal::Mask(size_t val) const {
    return pedestal_mask.at(val);
}

double JFPedestal::Mean() const {
    if (pedestal.empty())
        return 0.0;
    else
        return std::accumulate(pedestal.begin(), pedestal.end(), 0.0)
                / (4.0 * static_cast<double>(pedestal.size()));
}

double JFPedestal::MeanRMS() const {
    if (pedestal_rms.empty())
        return -1;
    else
        return std::accumulate(pedestal_rms.begin(), pedestal_rms.end(), 0.0)
               / static_cast<double>(pedestal_rms.size());
}

JFPedestal::operator JFJochProtoBuf::JFPedestal() const {
    JFJochProtoBuf::JFPedestal serialized;

    std::vector<char> tmp(bshuf_compress_zstd_bound(RAW_MODULE_SIZE, sizeof(uint16_t),
                                                    RAW_MODULE_SIZE));
    int64_t pedestal_size = bshuf_compress_zstd(pedestal.data(), tmp.data(), RAW_MODULE_SIZE,
                                               sizeof(uint16_t), RAW_MODULE_SIZE, 0);
    if (pedestal_size < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Compression error");

    serialized.set_pedestal(tmp.data(), pedestal_size);

    int64_t rms_size = bshuf_compress_zstd(pedestal_rms.data(), tmp.data(), RAW_MODULE_SIZE,
                                               sizeof(int16_t), RAW_MODULE_SIZE, 0);
    if (rms_size < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Compression error");

    serialized.set_rms(tmp.data(), rms_size);

    int64_t mask_size = bshuf_compress_zstd(pedestal_mask.data(), tmp.data(), RAW_MODULE_SIZE,
                                           sizeof(int8_t), RAW_MODULE_SIZE, 0);
    if (mask_size < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Compression error");

    serialized.set_mask(tmp.data(), mask_size);

    serialized.set_time_unix(collection_time);
    serialized.set_frames(frames);

    return serialized;
}

JFPedestal::JFPedestal(const JFJochProtoBuf::JFPedestal &serialized) : JFPedestal() {
    if (bshuf_decompress_zstd(serialized.pedestal().data(), pedestal.data(), RAW_MODULE_SIZE,
                              sizeof(uint16_t), RAW_MODULE_SIZE) < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Decompression error");

    if (bshuf_decompress_zstd(serialized.rms().data(), pedestal_rms.data(), RAW_MODULE_SIZE,
                              sizeof(int16_t), RAW_MODULE_SIZE) < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Decompression error");

    if (bshuf_decompress_zstd(serialized.mask().data(), pedestal_mask.data(), RAW_MODULE_SIZE,
                              sizeof(uint8_t), RAW_MODULE_SIZE) < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Decompression error");

    collection_time = serialized.time_unix();
    frames = serialized.frames();
}

std::vector<float> JFPedestal::GetPedestalFP() const {
    std::vector<float> tmp(RAW_MODULE_SIZE);

    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        tmp[i] = pedestal[i] * 0.25f;

    return tmp;
}

size_t JFPedestal::CountMaskedPixels() const {
    size_t ret = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (pedestal_mask[i] != 0) ret++;
    }
    return ret;
}