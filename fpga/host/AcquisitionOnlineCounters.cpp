// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>

#include "AcquisitionOnlineCounters.h"
#include "../../common/JFJochException.h"

void AcquisitionOnlineCounters::Reset(const DiffractionExperiment &experiment, uint16_t data_stream) {
    std::unique_lock<std::shared_mutex> ul(m);
    acquisition_finished = false;

    slowest_head = 0;

    if ((experiment.GetDetectorMode() == DetectorMode::PedestalG0) ||
        (experiment.GetDetectorMode() == DetectorMode::PedestalG1) ||
        (experiment.GetDetectorMode() == DetectorMode::PedestalG2))
        expected_frames = experiment.GetFrameNum();
    else
        expected_frames = experiment.GetImageNum() * experiment.GetSummation();

    nmodules = experiment.GetModulesNum(data_stream);
    head = std::vector<uint64_t>(nmodules);

    handle_for_frame = std::vector<uint64_t>((expected_frames+1) * nmodules, HandleNotFound);
    full_module_collected = std::vector<uint8_t>(expected_frames * nmodules);
}

void AcquisitionOnlineCounters::UpdateCounters(const Completion *c) {
    std::unique_lock<std::shared_mutex> ul(m);

    if (c->frame_number >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                     "Frame number is out of bounds");
    else {
        if (head[c->module] < c->frame_number)
            head[c->module] = c->frame_number;

        if (c->frame_number > slowest_head)
            slowest_head = *std::min_element(head.begin(), head.end());

        full_module_collected[c->frame_number * nmodules + c->module] = (c->packet_count == expected_packets_per_module);
        handle_for_frame[c->frame_number * nmodules + c->module] = c->handle;
    }

    data_updated.notify_all();
}

void AcquisitionOnlineCounters::SetAcquisitionFinished() {
    std::unique_lock<std::shared_mutex> ul(m);
    acquisition_finished = true;
    data_updated.notify_all();
}

uint64_t AcquisitionOnlineCounters::GetBufferHandle(size_t frame, uint16_t module) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    return handle_for_frame.at(frame * nmodules + module);
}

uint64_t AcquisitionOnlineCounters::GetBufferHandleAndClear(size_t frame, uint16_t module) {
    std::unique_lock<std::shared_mutex> ul(m);
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    uint64_t ret_val = handle_for_frame.at(frame * nmodules + module);
    handle_for_frame.at(frame * nmodules + module) = HandleNotFound;
    return ret_val;
}

uint64_t AcquisitionOnlineCounters::GetHead(uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");
    return head[module];
}

uint64_t AcquisitionOnlineCounters::GetSlowestHead() const {
    return slowest_head;
}

void AcquisitionOnlineCounters::WaitForFrame(size_t curr_frame, uint16_t module) const {
    uint64_t slowest_head_tmp = (module == UINT16_MAX) ? GetSlowestHead() : GetHead(module);
    while (!acquisition_finished && (slowest_head_tmp < curr_frame)) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        slowest_head_tmp = (module == UINT16_MAX) ? GetSlowestHead() : GetHead(module);
    }
}

int64_t AcquisitionOnlineCounters::CalculateDelay(size_t curr_frame, uint16_t module) const {
    uint64_t slowest_head_tmp;

    if (module == UINT16_MAX)
        slowest_head_tmp = GetSlowestHead();
    else
        slowest_head_tmp = GetHead(module);

    return slowest_head_tmp - curr_frame;
}

bool AcquisitionOnlineCounters::IsAcquisitionFinished() const {
    return acquisition_finished;
}

bool AcquisitionOnlineCounters::IsFullModuleCollected(size_t frame, uint16_t module) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));

    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");

    return full_module_collected[frame * nmodules + module];
}