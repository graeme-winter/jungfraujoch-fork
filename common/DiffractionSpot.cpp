// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DiffractionSpot.h"
#include "RawToConvertedGeometry.h"

DiffractionSpot::DiffractionSpot(const Coord &c, uint64_t frame, int64_t in_photons, int64_t max_count, int64_t in_pixel_count) {
    coord = c * in_photons;
    mean_frame = frame * in_photons;
    photons = in_photons;
    max_photons = max_count;
    pixel_count = in_pixel_count;
}

DiffractionSpot::DiffractionSpot(uint32_t col, uint32_t line, uint64_t frame, int64_t in_photons) {
    if (in_photons < 0) in_photons = 0;

    coord = Coord(col, line, 0) * (double)in_photons;
    pixel_count = 1;
    mean_frame = frame * in_photons;
    photons = in_photons;
    max_photons = in_photons;
    max_line = line;
    min_line = line;
    max_col = col;
    min_col = col;
    first_frame = frame;
    last_frame = frame;
}

DiffractionSpot& DiffractionSpot::operator+=(const DiffractionSpot &other) {
    this->coord += other.coord;
    this->photons += other.photons;
    this->max_photons = std::max(this->max_photons, other.max_photons);
    this->mean_frame += other.mean_frame;
    this->pixel_count += other.pixel_count;

    if (other.min_line < this->min_line) this->min_line = other.min_line;
    if (other.min_col < this->min_col) this->min_col = other.min_col;

    if (other.max_line > this->max_line) this->max_line = other.max_line;
    if (other.max_col > this->max_col) this->max_col = other.max_col;

    if (other.first_frame < this->first_frame) this->first_frame = other.first_frame;
    if (other.last_frame > this->last_frame) this->last_frame = other.last_frame;

    return *this;
}

uint32_t DiffractionSpot::Depth() const {
    return last_frame - first_frame + 1;
}

uint32_t DiffractionSpot::Size() const {
    return (max_line - min_line + 1) * (max_col - min_col + 1);
}

int64_t DiffractionSpot::Count() const {
    return photons;
}

int64_t DiffractionSpot::MaxCount() const {
    return max_photons;
}

double DiffractionSpot::Frame() const {
    return (double) mean_frame / (double)photons;
}

Coord DiffractionSpot::RawCoord() const {
    return {coord.x / (double)photons, coord.y / (double)photons, coord.z / (double)photons};
}

int64_t DiffractionSpot::PixelCount() const {
    return pixel_count;
}

double DiffractionSpot::OmegaRad(const DiffractionExperiment &experiment) const {
    return experiment.GetOmega(Frame()) * M_PI / 180.0;
}

Coord DiffractionSpot::LabCoord(const DiffractionExperiment &experiment, uint16_t data_stream) const {
    return {(coord.x  / (double)photons - experiment.GetBeamX_pxl()) * PIXEL_SIZE_IN_MM ,
            (coord.y  / (double)photons - experiment.GetBeamY_pxl()) * PIXEL_SIZE_IN_MM ,
                 experiment.GetDetectorDistance_mm()};
}

// The function moves p back into origin of rotation
// It calculates projection of the rotated vector (p0) on rotation coordinate system
inline Coord reciprocal_rotate(const Coord &p, const Coord &m1, const Coord &m2, const Coord &m3, double omega_in_radian) {
    double p0_m1 = (m1*p) * cos(omega_in_radian) + (m3*p) * sin(omega_in_radian);
    double p0_m2 = m2*p;
    double p0_m3 = (m3*p) * cos(omega_in_radian) - (m1*p) * sin(omega_in_radian);
    return m1 * p0_m1 + m2 * p0_m2 + m3 * p0_m3;
}

Coord DiffractionSpot::ReciprocalCoord(const DiffractionExperiment &experiment, uint16_t data_stream) const {
    return LabCoord(experiment, data_stream).Normalize() / experiment.GetWavelength_A()
            - experiment.GetScatteringVector();
}

Coord DiffractionSpot::ReciprocalCoord3D(const DiffractionExperiment &experiment, uint16_t data_stream) const {
    Coord p = ReciprocalCoord(experiment, data_stream);
    Coord S0 = experiment.GetScatteringVector();
    Coord m2 = experiment.GetRotationAxis();
    Coord m1, m3;
    m1 = m2 % S0;
    m1 = m1.Normalize();
    m3 = m1 % m2;
    Coord p0 = reciprocal_rotate(p, m1, m2, m3, OmegaRad(experiment));
    return p0;
}

double DiffractionSpot::GetResolution(const DiffractionExperiment &experiment, uint16_t data_stream) const {
    Coord lab = LabCoord(experiment, data_stream);

    // Assumes planar detector, 90 deg towards beam
    double beam_path = lab.Length();

    // It is possible that beam center is directly on edge, so in this case a very small number (1 micron) is added to beam_path
    // to avoid division by zero
    if (lab.z == beam_path) return -1.0;

    double cos_2theta = lab.z / beam_path; // 0.4472135955
    // cos(2theta) = cos(theta)^2 - sin(theta)^2
    // cos(2theta) = 1 - 2*sin(theta)^2
    // Technically two solutions for two theta, but it makes sense only to take positive one in this case
    double sin_theta = sqrt((1-cos_2theta)/2);
    return experiment.GetWavelength_A() / (2 * sin_theta);
}

DiffractionSpot DiffractionSpot::TransformCoordinates(const DiffractionExperiment &experiment, uint16_t data_stream) {
    DiffractionSpot ret(*this);
    ret.coord = RawToConvertedCoordinate(experiment, data_stream, RawCoord()) * photons;
    return ret;
}