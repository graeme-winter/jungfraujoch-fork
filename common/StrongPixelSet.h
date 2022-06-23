// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_STRONGPIXELSET_H
#define JUNGFRAUJOCH_STRONGPIXELSET_H

#include <unordered_map>
#include <mutex>
#include "DiffractionExperiment.h"
#include "Coord.h"
#include "DiffractionSpot.h"


inline uint64_t strong_pixel_coord(uint16_t col, uint16_t line, uint32_t image) {
    uint64_t tmp1 = line;
    uint64_t tmp2 = image;
    return col + (tmp1 << 16u) + (tmp2 << 32u);
}

inline uint16_t line_from_strong_pixel(uint64_t strong_pixel) {
    return ((strong_pixel & 0xFFFF0000u) >> 16u);
}

inline uint16_t col_from_strong_pixel(uint64_t strong_pixel) {
    return (strong_pixel & 0x0000FFFFu);
}

inline uint32_t image_from_strong_pixel(uint64_t strong_pixel) {
    return ((strong_pixel & 0xFFFFFFFF00000000u) >> 32u);
}

class StrongPixelSet {
    mutable std::mutex m;
    std::unordered_map<uint64_t, int64_t> strong_pixel_map;
    void AddNeighbor(DiffractionSpot &spot, uint16_t col, uint16_t line, uint32_t image, bool connect_frames);
    DiffractionSpot BuildSpot(std::unordered_map<uint64_t, int64_t>::iterator &it, bool connect_frames);
public:
    size_t Count() const;
    void AddStrongPixel(uint16_t col, uint16_t line, uint32_t image, int64_t photons = 1);
    void FindSpots(const DiffractionExperiment &experiment, const JFJochProtoBuf::DataProcessingSettings &settings, std::vector<DiffractionSpot> &spots);
    size_t Common(const StrongPixelSet &set) const;
};


#endif //JUNGFRAUJOCH_STRONGPIXELSET_H
