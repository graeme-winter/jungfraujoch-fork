// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_DIFFRACTIONSPOT_H
#define JUNGFRAUJOCH_DIFFRACTIONSPOT_H

#include "Coord.h"
#include "DiffractionExperiment.h"
#include "jfjoch.pb.h"

// Definition of Bragg spot
class DiffractionSpot {
    Coord coord;      // Coordinates in "data" array (not exactly detector configuration)
    int64_t pixel_count;
    int64_t photons;     // total photon count
    int64_t max_photons; // maximum number of counts per pixel in the spot
    uint64_t mean_frame;
    uint32_t min_col, max_col, min_line, max_line; // Limits of the spot in X and Y directions
    uint32_t first_frame, last_frame; // Limits of the spot in time direction
public:
    DiffractionSpot() = default;
    DiffractionSpot(const Coord &c, uint64_t frame, int64_t photons, int64_t max_count, int64_t pixel_count);
    DiffractionSpot(uint32_t col, uint32_t line, uint64_t frame, int64_t photons);
    DiffractionSpot& operator+=(const DiffractionSpot& spot);
    DiffractionSpot TransformCoordinates(const DiffractionExperiment &experiment, uint16_t data_stream = TASK_NO_DATA_STREAM);
    int64_t PixelCount() const;
    uint32_t Depth() const;
    uint32_t Size() const;
    int64_t Count() const;
    int64_t MaxCount() const;
    double Frame() const;
    double OmegaRad(const DiffractionExperiment &experiment) const;
    Coord RawCoord() const;
    Coord LabCoord(const DiffractionExperiment &experiment, uint16_t data_stream = TASK_NO_DATA_STREAM) const;
    double GetResolution(const DiffractionExperiment &experiment, uint16_t data_stream = TASK_NO_DATA_STREAM) const;
    Coord ReciprocalCoord(const DiffractionExperiment &experiment, uint16_t data_stream = TASK_NO_DATA_STREAM) const;
    Coord ReciprocalCoord3D(const DiffractionExperiment &experiment, uint16_t data_stream = TASK_NO_DATA_STREAM) const;
};

#endif //JUNGFRAUJOCH_DIFFRACTIONSPOT_H
