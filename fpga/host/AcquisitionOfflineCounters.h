// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONOFFLINECOUNTERS_H
#define JUNGFRAUJOCH_ACQUISITIONOFFLINECOUNTERS_H

#include <vector>
#include <mutex>

#include <jfjoch.pb.h>

#include "Completion.h"
#include "../../common/DiffractionExperiment.h"


// AcquisitionOfflineCounters is used to store information that will be used AFTER data collection
class AcquisitionOfflineCounters {
    std::vector<Completion> completion;
    mutable std::mutex m;
public:
    void Reset(const DiffractionExperiment &x, uint16_t data_stream);
    void Add(Completion c);
    void FillStatistics(const DiffractionExperiment &x, uint16_t data_stream,
                        JFJochProtoBuf::AcquisitionDeviceStatistics& statistics) const;
};


#endif //JUNGFRAUJOCH_ACQUISITIONOFFLINECOUNTERS_H
