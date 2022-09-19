// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_MOCKACQUISITIONDEVICE_H
#define JUNGFRAUJOCH_MOCKACQUISITIONDEVICE_H

#include "AcquisitionDevice.h"
#include "../../common/ThreadSafeFIFO.h"

class MockAcquisitionDevice : public AcquisitionDevice {
    ThreadSafeFIFO<uint32_t> mailbox_fifo;
    uint32_t current_handle = 0;
    uint32_t max_handle = 0;
    ActionConfig cfg;

    constexpr static const uint32_t frames_int_pkt_gen = 1;
    void HW_ReadActionRegister(ActionConfig *job) override;
    void HW_WriteActionRegister(const ActionConfig *job) override;
    void HW_StartAction() override;
    bool HW_ReadMailbox(uint32_t values[4]) override;
    void HW_SetCancelDataCollectionBit() override;
    bool HW_SendWorkRequest(uint32_t handle) override;
    bool HW_IsIdle() const override;
    uint64_t HW_GetMACAddress() const override;
    void HW_GetStatus(ActionStatus *status) const override;
    void CopyInternalPacketGenFrameToDeviceBuffer() override;
public:
    MockAcquisitionDevice(uint16_t data_stream, size_t in_frame_buffer_size_modules);
    void AddModule(uint64_t frame, uint16_t module, const uint16_t *data);
    void Terminate();
    void InitializeCalibration(const DiffractionExperiment &experiment, const JFCalibration &calib) override;
};

#endif //JUNGFRAUJOCH_MOCKACQUISITIONDEVICE_H
