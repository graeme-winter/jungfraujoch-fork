// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MockAcquisitionDevice.h"
#include "../../common/JFJochException.h"
#include <bitset>

void MockAcquisitionDevice::HW_ReadActionRegister(ActionConfig *job) {
    memcpy(job, &cfg, sizeof(ActionConfig));
}

void MockAcquisitionDevice::HW_WriteActionRegister(const ActionConfig *job) {
    memcpy(&cfg, job,  sizeof(ActionConfig));
}

void MockAcquisitionDevice::HW_StartAction() {}

bool MockAcquisitionDevice::HW_ReadMailbox(uint32_t values[4]) {
    values[0] = mailbox_fifo.GetBlocking();
    values[1] = mailbox_fifo.GetBlocking();
    values[2] = mailbox_fifo.GetBlocking();
    values[3] = mailbox_fifo.GetBlocking();
    return true;
}

void MockAcquisitionDevice::HW_SetCancelDataCollectionBit() {
    if (logger)
        logger->Info("MockAcquisitionDevice cancelling " + std::to_string(data_stream));
    mailbox_fifo.Put(UINT32_MAX);
    mailbox_fifo.Put(current_handle * 128);
    mailbox_fifo.Put(0);
    mailbox_fifo.Put(0);
}

bool MockAcquisitionDevice::HW_SendWorkRequest(uint32_t handle) {
    return true;
}


bool MockAcquisitionDevice::HW_IsIdle() const {
    return true;
}

MockAcquisitionDevice::MockAcquisitionDevice(uint16_t data_stream, size_t in_frame_buffer_size_modules)
: AcquisitionDevice(data_stream) {

    MapBuffersStandard(in_frame_buffer_size_modules,
                       (3 + 3 * 16) * max_modules + frames_int_pkt_gen, -1);
    max_handle = in_frame_buffer_size_modules;
}

void MockAcquisitionDevice::AddModule(uint64_t frame, uint16_t module, const uint16_t *data) {
    if (buffer_h2c.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Need to explictely allocate memory");
    if (current_handle < max_handle) {
        memcpy(buffer_c2h.at(current_handle), data, RAW_MODULE_SIZE * sizeof(uint16_t));

        uint32_t parity = (std::bitset<32>(current_handle).count() + std::bitset<32>(module & 0xFF).count() + 1
                           + std::bitset<64>(frame).count()) % 2;
        mailbox_fifo.Put(current_handle);
        mailbox_fifo.Put((module & 0xFF) // module number
                         + (1 << 8)          // all packets in order
                         + ((parity == 1) ? (1 << 15) : 0)); // parity bit
        mailbox_fifo.Put(frame >> 32);
        mailbox_fifo.Put(frame & UINT32_MAX);

        mailbox_fifo.Put(0);
        mailbox_fifo.Put(0);
        mailbox_fifo.Put(0);
        mailbox_fifo.Put(0);

        current_handle++;
    } else {
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "No buffer handles left");
    }
}

void MockAcquisitionDevice::Terminate() {
    mailbox_fifo.Put(UINT32_MAX);
    mailbox_fifo.Put(current_handle * 128);
    mailbox_fifo.Put(0);
    mailbox_fifo.Put(0);
}

uint32_t MockAcquisitionDevice::HW_GetInternalPacketGeneratorModuleNum() {
    return frames_int_pkt_gen;
}

uint32_t MockAcquisitionDevice::HW_GetMaxModuleNum() {
    return max_modules; // Doesn't matter
}

uint64_t MockAcquisitionDevice::HW_GetMACAddress() const {
    return 0; // Doesn't matter
}