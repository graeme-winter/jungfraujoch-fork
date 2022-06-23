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
    unit_cell = in_settings.unit_cell;

    if (!in_settings.unit_cell.empty()) {
        CrystalLattice lattice(unit_cell);
        CrystalLattice rl = lattice.ReciprocalLattice();
        Eigen::Matrix3f eigen_lattice;
        eigen_lattice << rl.vec_a.x, rl.vec_b.x, rl.vec_c.x, rl.vec_a.y, rl.vec_b.y, rl.vec_c.y, rl.vec_a.z, rl.vec_b.z, rl.vec_c.z;

        xgandalf::ExperimentSettings settings(12400,
                                              0.1,
                                              0.1f, 0.05f, 0.005f,
                                              eigen_lattice, tolerance, 0.1f);
        indexer = std::make_unique<xgandalf::IndexerPlain>(settings);
    } else {
        unit_cell = UnitCell();
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

std::vector<UnitCell> XgandalfWrapper::Run(const IndexingSettings &settings,
                                           const std::vector<Coord> &coord) {
    std::vector<UnitCell> ret;

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

        ret.emplace_back(a_orig.Length(), b_orig.Length(), c_orig.Length(),
                         angle_deg(b_orig, c_orig),angle_deg(a_orig, c_orig),
                         angle_deg(a_orig, b_orig));
    }
    return ret;
}