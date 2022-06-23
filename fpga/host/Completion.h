// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_COMPLETION_H
#define JUNGFRAUJOCH_COMPLETION_H

#include <cstdint>

struct Completion {
    static const uint64_t FrameAfterFilterEnd;
    static const uint64_t FrameIgnore;
    static const uint64_t MeasurementDone;

    uint64_t frame_number;
    uint64_t packet_mask[2];
    uint32_t handle;
    uint8_t module;
    bool trigger;
};

#endif //JUNGFRAUJOCH_COMPLETION_H
