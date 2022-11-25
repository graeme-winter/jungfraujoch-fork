// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONONLINECOUNTERS_H
#define JUNGFRAUJOCH_ACQUISITIONONLINECOUNTERS_H

#include <condition_variable>
#include <shared_mutex>
#include <vector>

#include "../../common/DiffractionExperiment.h"
#include "../../common/Definitions.h"
#include "Completion.h"

// AcquisitionOnlineCounters are used for information that needs to be accessed during data collection,
// so has more stringent mutex requirements than AcquisitionOfflineCounters

class AcquisitionOnlineCounters {
    mutable std::shared_mutex m;
    mutable std::condition_variable_any data_updated;

    std::vector<uint64_t> handle_for_frame;
    std::vector<uint8_t> full_module_collected; // vector of bool might have weird concurrency behavior

    uint64_t slowest_head;
    std::vector<uint64_t> head;
    bool acquisition_finished;
    uint64_t expected_frames;
    uint64_t nmodules;
    const uint16_t expected_packets_per_module = 256;
public:
    static constexpr const uint64_t HandleNotFound = UINT64_MAX;

    void Reset(const DiffractionExperiment &experiment, uint16_t data_stream);
    void UpdateCounters(const Completion *c);
    void SetAcquisitionFinished();

    uint64_t GetHead(uint16_t module) const;
    uint64_t GetSlowestHead() const;

    void WaitForFrame(size_t curr_frame, uint16_t module = UINT16_MAX) const;
    int64_t CalculateDelay(size_t curr_frame, uint16_t module = UINT16_MAX) const; // mutex acquired indirectly
    uint64_t GetBufferHandle(size_t frame, uint16_t module) const;
    uint64_t GetBufferHandleAndClear(size_t frame, uint16_t module);
    bool IsFullModuleCollected(size_t frame, uint16_t module) const;
    bool IsAcquisitionFinished() const;
};


#endif //JUNGFRAUJOCH_ACQUISITIONONLINECOUNTERS_H
