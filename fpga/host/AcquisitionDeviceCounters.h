// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONDEVICECOUNTERS_H
#define JUNGFRAUJOCH_ACQUISITIONDEVICECOUNTERS_H

#include <condition_variable>
#include <map>
#include <shared_mutex>
#include <vector>

#include "../../common/DiffractionExperiment.h"
#include "../../common/Definitions.h"
#include "Completion.h"

// AcquisitionDeviceCounters use mutex for operation that change the state
// but not to operations that read status, given the latter are called very frequently
// The proper way to access the class is to first use WaitForFrame with 2+ delay (mutex/cond protected)
// and only access elements afterwards --> it is assumed in this situation all counters are constant and don't need
// to be checked synchronously

class AcquisitionDeviceCounters {
    mutable std::shared_mutex m;
    mutable std::condition_variable_any data_updated;

    std::vector<uint64_t> packet_mask_half;
    std::vector<bool> trigger;
    std::vector<uint64_t> handle_for_frame;

    uint64_t slowest_head;
    std::vector<uint64_t> head;
    bool acquisition_finished;
    uint64_t expected_frames;
    uint64_t nmodules;
public:
    static const uint16_t HandleNotFound = UINT16_MAX;

    void Reset(const DiffractionExperiment &experiment, uint16_t data_stream);
    void UpdateCounters(const Completion *c);
    void SetAcquisitionFinished();

    uint64_t GetHead(uint16_t module) const;
    uint64_t GetSlowestHead() const;

    void WaitForFrame(size_t curr_frame, uint16_t module = UINT16_MAX) const;
    int64_t CalculateDelay(size_t curr_frame, uint16_t module = UINT16_MAX) const; // mutex acquired indirectly
    bool IsPacketCollected(size_t frame, uint16_t module, uint16_t packet_number) const;
    uint64_t GetPacketMaskHalfModule(size_t frame, uint16_t half_module) const;
    uint64_t GetBufferHandle(size_t frame, uint16_t module) const;
    uint64_t GetBufferHandleAndClear(size_t frame, uint16_t module);
    uint16_t GetPacketCount(size_t frame, uint16_t module) const;
    uint16_t GetPacketCountHalfModule(size_t frame, uint16_t module) const;
    bool GetTriggerField(size_t frame, uint16_t module) const;
    uint64_t GetPedestalHandle(uint16_t module) const;

    bool GetAcqusitionFinished() const;

    // This is NOT protected by mutex - use only, when data acquisition is finished!
    const std::vector<uint64_t> &PacketMaskHalfModule() const;
};


#endif //JUNGFRAUJOCH_ACQUISITIONDEVICECOUNTERS_H
