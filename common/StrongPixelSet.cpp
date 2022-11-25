// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <bitset>
#include "StrongPixelSet.h"

void StrongPixelSet::AddStrongPixel(uint16_t col, uint16_t line, uint32_t image, int64_t photons) {
    std::lock_guard<std::mutex> lg(m);
    uint64_t key = strong_pixel_coord(col, line, image);
    strong_pixel_map[key] = photons;
}

inline void StrongPixelSet::AddNeighbor(DiffractionSpot &spot, uint16_t col, uint16_t line, uint32_t image) {
    uint64_t coord = strong_pixel_coord(col, line, image);
    auto iter = strong_pixel_map.find(coord);

    if (iter != strong_pixel_map.end())
        spot += BuildSpot(iter);
}

// Creates a continuous spot
// strong pixels are loaded into dictionary (one dictionary per frame)
// and routine checks if neighboring pixels are also in dictionary (likely in log(N) time)
DiffractionSpot StrongPixelSet::BuildSpot(std::unordered_map<uint64_t, int64_t>::iterator &it) {

    uint16_t col = col_from_strong_pixel(it->first);
    uint16_t line = line_from_strong_pixel(it->first);
    uint64_t frame = image_from_strong_pixel(it->first);

    DiffractionSpot spot(col, line, frame, it->second);

    strong_pixel_map.erase(it); // Remove strong pixel from the dictionary, so it is not processed again

    AddNeighbor(spot, col+1, line  , frame);
    AddNeighbor(spot, col+1, line+1, frame);
    AddNeighbor(spot, col+1, line-1, frame);

    AddNeighbor(spot, col-1, line, frame);
    AddNeighbor(spot, col-1, line+1, frame);
    AddNeighbor(spot, col-1, line-1, frame);

    AddNeighbor(spot, col, line+1, frame);
    AddNeighbor(spot, col, line-1, frame);

    return spot;
}

void StrongPixelSet::FindSpots(const DiffractionExperiment &experiment, const JFJochProtoBuf::DataProcessingSettings &settings,
                               std::vector<DiffractionSpot> &spots) {
    std::lock_guard<std::mutex> lg(m);

    std::multimap<double, DiffractionSpot> spots_map;

    while (!strong_pixel_map.empty()) {
        auto iter = strong_pixel_map.begin();
        DiffractionSpot spot = BuildSpot(iter);
        double d = spot.GetResolution(experiment);

        if ((spot.PixelCount() <= settings.max_pix_per_spot())
            && (spot.PixelCount() >= settings.min_pix_per_spot())
            && (!settings.has_low_resolution_limit() || (d <= settings.low_resolution_limit()))
            && (!settings.has_high_resolution_limit() || (d >= settings.high_resolution_limit())))
            spots_map.insert(std::make_pair(-static_cast<float>(d), spot));
    }

    for (auto &[x, spot]: spots_map)
        spots.push_back(spot);

    if (experiment.GetMaxSpotCount() > 0)
        spots.resize(std::min<size_t>(spots.size(), experiment.GetMaxSpotCount()));
}

size_t StrongPixelSet::Count() const {
    return strong_pixel_map.size();
}

size_t StrongPixelSet::Common(const StrongPixelSet &set) const {
    std::lock_guard<std::mutex> lg(m);
    size_t ret = 0;
    for (const auto& pixel: strong_pixel_map) {
        if (set.strong_pixel_map.count(pixel.first) == 1)
            ret++;
    }
    return ret;
}