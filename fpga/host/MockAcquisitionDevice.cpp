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

    max_modules = 16;

    MapBuffersStandard(in_frame_buffer_size_modules,
                       (3 + 3 * 16) * max_modules + frames_int_pkt_gen, -1);
    max_handle = in_frame_buffer_size_modules;
}

void MockAcquisitionDevice::AddModule(uint64_t frame, uint16_t module, const uint16_t *data) {

    if (module >= max_modules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Module number exceeding limit");

    if (current_handle < max_handle) {
        memcpy(buffer_device.at(current_handle), data, RAW_MODULE_SIZE * sizeof(uint16_t));

        uint32_t tmp = (module & 0xFF) // module number
                       | (1 << 8)      // all packets in order
                       | (256 << 16);  // 256 packets collected
        uint32_t parity = (std::bitset<32>(current_handle).count()
                           + std::bitset<32>(tmp).count()
                           + std::bitset<64>(frame).count()) % 2;
        mailbox_fifo.Put(current_handle);
        mailbox_fifo.Put( tmp | ((parity == 1) ? (1 << 15) : 0)); // parity bit
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

uint64_t MockAcquisitionDevice::HW_GetMACAddress() const {
    return 0; // Doesn't matter
}

void MockAcquisitionDevice::HW_GetStatus(ActionStatus *status) const {
    memset(status, 0, sizeof(ActionStatus));
    status->max_modules = max_modules;
    status->modules_internal_packet_generator = 1;
}

void MockAcquisitionDevice::CopyInternalPacketGenFrameToDeviceBuffer() {}
void MockAcquisitionDevice::InitializeCalibration(const DiffractionExperiment &experiment, const JFCalibration &calib) {}
