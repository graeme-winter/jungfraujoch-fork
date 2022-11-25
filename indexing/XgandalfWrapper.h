// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_XGANDALFWRAPPER_H
#define JUNGFRAUJOCH_XGANDALFWRAPPER_H

#include "CrystalLattice.h"
#include "IndexerWrapper.h"

#include "adaptions/crystfel/Lattice.h"
#include "adaptions/crystfel/ExperimentSettings.h"
#include "adaptions/crystfel/IndexerPlain.h"

class XgandalfWrapper : public IndexerWrapper {
    std::unique_ptr<xgandalf::IndexerPlain> indexer;
    reciprocalPeaks_1_per_A_t peaks{};
    bool fast_xgandalf;
    const double min_unit_cell = 30.0;
    const double max_unit_cell = 250.0;
    const double tolerance = 0.02;
public:
    XgandalfWrapper(bool fast = true);
    ~XgandalfWrapper();
    void Setup(const IndexingSettings &settings) override;
    std::vector<JFJochProtoBuf::UnitCell> Run(const std::vector<Coord> &coord) override;
};


#endif //JUNGFRAUJOCH_XGANDALFWRAPPER_H
