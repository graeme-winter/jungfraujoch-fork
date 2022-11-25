// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_FASTFEEDBACKINDEXERWRAPPER_H
#define JUNGFRAUJOCH_FASTFEEDBACKINDEXERWRAPPER_H

#include "IndexerWrapper.h"
#include <Eigen/LU>
#include <Eigen/Dense>
#include "fast-feedback-indexer/indexer/src/indexer.h"

#define MAX_SPOTS 250

class FastFeedbackIndexerWrapper : public IndexerWrapper {
    fast_feedback::config_runtime<float> crt{};
    fast_feedback::config_persistent<float> cpers{};    // default persistent config

    Eigen::MatrixX3f coords{MAX_SPOTS + 3, 3}; // coordinate container
public:
    void Setup(const IndexingSettings &settings) override;
    std::vector<JFJochProtoBuf::UnitCell> Run(const std::vector<Coord> &coord) override;
};


#endif //JUNGFRAUJOCH_FASTFEEDBACKINDEXERWRAPPER_H
