// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_INDEXERWRAPPER_H
#define JUNGFRAUJOCH_INDEXERWRAPPER_H

#include <vector>
#include <jfjoch.pb.h>

#include "../common/Coord.h"

struct IndexingSettings {
    uint64_t max_indexing_spots;
    JFJochProtoBuf::UnitCell unit_cell;
    bool has_unit_cell;
    char centering;
};

class IndexerWrapper {
public:
    virtual void Setup(const IndexingSettings& settings) = 0;
    virtual std::vector<JFJochProtoBuf::UnitCell> Run(const std::vector<Coord> &coord) = 0;
};


#endif //JUNGFRAUJOCH_INDEXERWRAPPER_H
