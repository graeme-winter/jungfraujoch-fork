// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_COMPLETION_H
#define JUNGFRAUJOCH_COMPLETION_H

#include <cstdint>

struct Completion {
    static const uint64_t MeasurementDone;

    uint64_t frame_number;
    uint64_t packet_mask[4];
    uint64_t bunchid;
    uint32_t debug;
    uint32_t handle;
    uint32_t timestamp;
    uint16_t packet_count;
    uint8_t module;
};

#endif //JUNGFRAUJOCH_COMPLETION_H
