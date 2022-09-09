// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <bitset>

#include "AcquisitionDeviceCounters.h"
#include "../../common/JFJochException.h"
#include <thread>

const uint16_t AcquisitionDeviceCounters::HandleNotFound;

void AcquisitionDeviceCounters::Reset(const DiffractionExperiment &experiment, uint16_t data_stream) {
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
    packet_mask_half = std::vector<uint64_t>(expected_frames * nmodules * 2, 0);
    trigger = std::vector<bool>(expected_frames * nmodules, false);
    timestamp = std::vector<uint32_t>(expected_frames * nmodules, false);
}

void AcquisitionDeviceCounters::UpdateCounters(const Completion *c) {
    std::unique_lock<std::shared_mutex> ul(m);
    if (c->frame_number == PEDESTAL_FRAME_ID)
        handle_for_frame[expected_frames * nmodules + c->module] =  c->handle;
    else if (c->frame_number >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                     "Frame number is out of bounds");
    else {
        if (head[c->module] < c->frame_number)
            head[c->module] = c->frame_number;

        if (c->frame_number > slowest_head)
            slowest_head = *std::min_element(head.begin(), head.end());

        handle_for_frame[c->frame_number * nmodules + c->module] = c->handle;
        trigger[c->frame_number * nmodules + c->module] = c->trigger;
        timestamp[c->frame_number * nmodules + c->module] = c->timestamp;
        packet_mask_half[2*(c->frame_number * nmodules + c->module)]    =  c->packet_mask[0];
        packet_mask_half[2*(c->frame_number * nmodules + c->module) + 1] = c->packet_mask[1];
    }

    data_updated.notify_all();
}

void AcquisitionDeviceCounters::SetAcquisitionFinished() {
    std::unique_lock<std::shared_mutex> ul(m);
    acquisition_finished = true;
    data_updated.notify_all();
}

uint16_t AcquisitionDeviceCounters::GetPacketCount(size_t frame, uint16_t module) const {
    return GetPacketCountHalfModule(frame, module * 2) + GetPacketCountHalfModule(frame, module * 2 + 1);
}

uint64_t AcquisitionDeviceCounters::GetBufferHandle(size_t frame, uint16_t module) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    return handle_for_frame.at(frame * nmodules + module);
}

uint64_t AcquisitionDeviceCounters::GetBufferHandleAndClear(size_t frame, uint16_t module) {
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

uint64_t AcquisitionDeviceCounters::GetPedestalHandle(uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    return handle_for_frame.at(expected_frames * nmodules + module);
}

uint16_t AcquisitionDeviceCounters::GetPacketCountHalfModule(size_t frame, uint16_t module) const {
    uint64_t tmp = GetPacketMaskHalfModule(frame, module);
    return std::bitset<64>(tmp).count();
}

uint64_t AcquisitionDeviceCounters::GetPacketMaskHalfModule(size_t frame, uint16_t half_module) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));
    if (half_module >= nmodules*2)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    return packet_mask_half.at(frame * nmodules * 2 + half_module);
}

bool AcquisitionDeviceCounters::GetTriggerField(size_t frame, uint16_t module) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");
    return trigger.at(frame * nmodules + module);
}

uint64_t AcquisitionDeviceCounters::GetHead(uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");
    return head[module];
}

uint64_t AcquisitionDeviceCounters::GetSlowestHead() const {
    return slowest_head;
}

void AcquisitionDeviceCounters::WaitForFrame(size_t curr_frame, uint16_t module) const {
    uint64_t slowest_head_tmp = (module == UINT16_MAX) ? GetSlowestHead() : GetHead(module);
    while (!acquisition_finished && (slowest_head_tmp < curr_frame)) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        slowest_head_tmp = (module == UINT16_MAX) ? GetSlowestHead() : GetHead(module);
    }
}

int64_t AcquisitionDeviceCounters::CalculateDelay(size_t curr_frame, uint16_t module) const {
    uint64_t slowest_head_tmp;

    if (module == UINT16_MAX)
        slowest_head_tmp = GetSlowestHead();
    else
        slowest_head_tmp = GetHead(module);

    return slowest_head_tmp - curr_frame;
}

bool AcquisitionDeviceCounters::GetAcqusitionFinished() const {
    return acquisition_finished;
}

const std::vector<uint64_t> &AcquisitionDeviceCounters::PacketMaskHalfModule() const {
    return packet_mask_half;
}

const std::vector<uint32_t> &AcquisitionDeviceCounters::Timestamps() const {
    return timestamp;
}

bool AcquisitionDeviceCounters::IsPacketCollected(size_t frame, uint16_t module, uint16_t packet_number) const {
    if (frame >= expected_frames)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Wrong frame number: " + std::to_string(frame));

    int16_t half_module = module * 2 + ((packet_number > 63) ? 1 : 0);

    if (half_module >= nmodules*2)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong module number");

    uint64_t mask_to_check = 1L << (packet_number % 64);
    return (packet_mask_half.at(frame * nmodules * 2 + half_module) & mask_to_check);
}