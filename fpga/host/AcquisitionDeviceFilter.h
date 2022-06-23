// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONDEVICEFILTERTEST_H
#define JUNGFRAUJOCH_ACQUISITIONDEVICEFILTERTEST_H

#include "../../common/Definitions.h"
#include "../../common/DiffractionExperiment.h"
#include "Completion.h"

class AcquisitionDeviceFilter {
    std::vector<int64_t> sequence_starts;
    int64_t expected_triggers;
    int64_t encountered_triggers{0};

    int64_t frames_per_trigger;
    int64_t frame_number_last_trigger{0};
public:
    AcquisitionDeviceFilter();
    AcquisitionDeviceFilter(const DiffractionExperiment &x);
    uint64_t ProcessCompletion(uint64_t frame_number, bool trigger);
    uint64_t ProcessCompletion(Completion &c);
    const std::vector<int64_t> &TriggerSequenceFrameNumbers() const;
};


#endif //JUNGFRAUJOCH_ACQUISITIONDEVICEFILTERTEST_H
