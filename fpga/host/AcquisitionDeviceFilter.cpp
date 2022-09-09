// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AcquisitionDeviceFilter.h"
#include <iostream>

AcquisitionDeviceFilter::AcquisitionDeviceFilter() {
    expected_frames = 0;
}

AcquisitionDeviceFilter::AcquisitionDeviceFilter(const DiffractionExperiment &x) {
    expected_frames = x.GetFrameNum();
}

const std::vector<int64_t> &AcquisitionDeviceFilter::TriggerSequenceFrameNumbers() const {
    return sequence_starts;
}

uint64_t AcquisitionDeviceFilter::ProcessCompletion(uint64_t frame_number, bool trigger) {
    // Always allow pedestal to go through
    if (frame_number == PEDESTAL_FRAME_ID)
        return frame_number;

    // frame number 0 is not allowed and should be ignored
    if (frame_number == 0)
        return Completion::FrameIgnore;

    // Frame numbers in JF start from 1, so need to be shifted to start from 0
    frame_number -= 1;

    if (frame_number >= expected_frames)
        return Completion::FrameAfterFilterEnd;
    else
        return frame_number;
}

uint64_t AcquisitionDeviceFilter::ProcessCompletion(Completion &c) {
    return ProcessCompletion(c.frame_number, c.trigger);
}