// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AcquisitionDeviceFilter.h"
#include <iostream>

AcquisitionDeviceFilter::AcquisitionDeviceFilter() {
    expected_triggers         = 0;
    frames_per_trigger        = 0;
}

AcquisitionDeviceFilter::AcquisitionDeviceFilter(const DiffractionExperiment &x) {
    if (x.GetTimeResolvedMode()) {
        expected_triggers = 0;
        frames_per_trigger = x.GetFrameNum();
    } else {
        expected_triggers = x.GetNumTriggers();
        frames_per_trigger = x.GetFrameNumPerTrigger();
    }
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

    if (expected_triggers == 0) {
        if (frame_number >= frames_per_trigger)
            return Completion::FrameAfterFilterEnd;
        else
            return frame_number;
    } else {
        int64_t delta = frame_number - frame_number_last_trigger;

        if (trigger
            && (encountered_triggers < expected_triggers)
            && ((encountered_triggers == 0) || ((delta > frames_per_trigger)))
                ) {
            // Frame is set as start of new sequence, if:
            // 1. Trigger was observed in this frame AND
            // 2. There was not enough triggers observed till now AND
            // 3. This frame is after previous sequence ended OR it is first frame with trigger

            frame_number_last_trigger = frame_number;
            sequence_starts.push_back(frame_number);
            delta = 0;
            encountered_triggers++;
        }

        if ((encountered_triggers == expected_triggers) && (delta >= frames_per_trigger + 5))
            return Completion::FrameAfterFilterEnd;
        else if ((encountered_triggers > 0) && (delta >= 0) && (delta < frames_per_trigger)) {
            // Trigger is set and frame number is in a proper window
            return (encountered_triggers - 1) * frames_per_trigger + delta;
        } else
            return Completion::FrameIgnore;
    }
}

uint64_t AcquisitionDeviceFilter::ProcessCompletion(Completion &c) {
    return ProcessCompletion(c.frame_number, c.trigger);
}