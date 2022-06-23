// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later
// GPL

#ifndef JUNGFRAUJOCH_XGANDALFWRAPPER_H
#define JUNGFRAUJOCH_XGANDALFWRAPPER_H

#include "../common/DiffractionExperiment.h"
#include "../common/DiffractionSpot.h"
#include "../common/CrystalLattice.h"
#include "../common/ZMQWrappers.h"

#include "adaptions/crystfel/Lattice.h"
#include "adaptions/crystfel/ExperimentSettings.h"
#include "adaptions/crystfel/IndexerPlain.h"

enum class IndexingAlgorithm {Xgandalf, Xgandalf_fast};

struct IndexingSettings {
    IndexingAlgorithm algorithm;
    uint64_t max_indexing_spots;
    UnitCell unit_cell;
};

class XgandalfWrapper  {
    std::unique_ptr<xgandalf::IndexerPlain> indexer;
    reciprocalPeaks_1_per_A_t peaks{};

    const double min_unit_cell = 30.0;
    const double max_unit_cell = 250.0;
    const double tolerance = 0.02;
    void Setup(const IndexingSettings &settings);
    UnitCell unit_cell;
public:
    XgandalfWrapper();
    ~XgandalfWrapper();
    std::vector<UnitCell> Run(const IndexingSettings &settings, const std::vector<Coord> &coord);
};


#endif //JUNGFRAUJOCH_XGANDALFWRAPPER_H