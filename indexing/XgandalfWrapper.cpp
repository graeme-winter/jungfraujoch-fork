// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later
// Derived work of CrystFEL and XGandalf
// GPL

#include "XgandalfWrapper.h"
#include "ExperimentSettings.h"
#include "IndexerPlain.h"

#define MAX_ASSEMBLED_LATTICES_COUNT (10)

XgandalfWrapper::XgandalfWrapper() {
    allocReciprocalPeaks(&peaks);
}

XgandalfWrapper::~XgandalfWrapper() {
    freeReciprocalPeaks(peaks);
}

void XgandalfWrapper::Setup(const IndexingSettings &in_settings) {
    if (in_settings.has_unit_cell) {
        CrystalLattice lattice(in_settings.unit_cell);
        auto ucl = lattice.Uncenter(in_settings.centering);
        auto rl = ucl.ReciprocalLattice();
        Eigen::Matrix3f eigen_lattice;
        eigen_lattice <<
                rl.vec[0].x, rl.vec[1].x, rl.vec[2].x,
                rl.vec[0].y, rl.vec[1].y, rl.vec[2].y,
                rl.vec[0].z, rl.vec[1].z, rl.vec[2].z;

        xgandalf::ExperimentSettings settings(12400,
                                              0.1,
                                              0.1f, 0.05f, 0.005f,
                                              eigen_lattice, tolerance, 0.1f);
        indexer = std::make_unique<xgandalf::IndexerPlain>(settings);
    } else {
        xgandalf::ExperimentSettings settings(12400,
                                              0.1,
                                              0.1, 0.05, 0.005,
                                              min_unit_cell, max_unit_cell, 0.1);

        indexer = std::make_unique<xgandalf::IndexerPlain>(settings);
    }

    switch (in_settings.algorithm) {
        case IndexingAlgorithm::Xgandalf_fast:
            indexer->setGradientDescentIterationsCount(xgandalf::IndexerPlain::GradientDescentIterationsCount::few);
            indexer->setSamplingPitch(xgandalf::IndexerPlain::SamplingPitch::standard);
            break;
        case IndexingAlgorithm::Xgandalf:
        default:
            indexer->setGradientDescentIterationsCount(xgandalf::IndexerPlain::GradientDescentIterationsCount::manyMany);
            indexer->setSamplingPitch(xgandalf::IndexerPlain::SamplingPitch::denseWithSeondaryMillerIndices);
            break;
    }
    if (in_settings.max_indexing_spots <= 0)
        indexer->setMaxPeaksToUseForIndexing(30);
    else
        indexer->setMaxPeaksToUseForIndexing(in_settings.max_indexing_spots);
}

std::vector<JFJochProtoBuf::UnitCell> XgandalfWrapper::Run(const IndexingSettings &settings,
                                           const std::vector<Coord> &coord) {
    std::vector<JFJochProtoBuf::UnitCell> ret;

    if (coord.size() < 4)
        return ret;

    Setup(settings);

    peaks.peakCount = std::min<int32_t>(coord.size(), MAX_PEAK_COUNT_FOR_INDEXER);

    for (int i = 0; i < peaks.peakCount; i++) {
        peaks.coordinates_x[i] = coord[i].x;
        peaks.coordinates_y[i] = coord[i].y;
        peaks.coordinates_z[i] = coord[i].z;
    }

    Lattice_t assembledLattices[MAX_ASSEMBLED_LATTICES_COUNT];
    int assembledLatticesCount;
    IndexerPlain_index(indexer.get(),
                       assembledLattices,
                       &assembledLatticesCount,
                       MAX_ASSEMBLED_LATTICES_COUNT,
                       peaks,
                       nullptr);

    for (int i = 0; i < assembledLatticesCount; i++) {
        Coord a_orig(assembledLattices[0].ax, assembledLattices[0].ay, assembledLattices[0].az);
        Coord b_orig(assembledLattices[0].bx, assembledLattices[0].by, assembledLattices[0].bz);
        Coord c_orig(assembledLattices[0].cx, assembledLattices[0].cy, assembledLattices[0].cz);

        JFJochProtoBuf::UnitCell cell;
        cell.set_a(a_orig.Length());
        cell.set_b(b_orig.Length());
        cell.set_c(c_orig.Length());
        cell.set_alpha(angle_deg(b_orig, c_orig));
        cell.set_beta(angle_deg(a_orig, c_orig));
        cell.set_gamma(angle_deg(a_orig, b_orig));
        ret.emplace_back(cell);
    }
    return ret;
}