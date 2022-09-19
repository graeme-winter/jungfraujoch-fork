// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_PCIEXPRESSDEVICE_H
#define JUNGFRAUJOCH_PCIEXPRESSDEVICE_H

#include "AcquisitionDevice.h"

class PCIExpressDevice : public AcquisitionDevice {
    int fd;

    bool HW_ReadMailbox(uint32_t values[4]) override;
    void HW_SetCancelDataCollectionBit() override;
    bool HW_SendWorkRequest(uint32_t handle) override;
    void HW_StartAction() override;
    bool HW_IsIdle() const final;
    void HW_WriteActionRegister(const ActionConfig *job) override;
    void HW_ReadActionRegister(ActionConfig *job) override;
    uint64_t HW_GetMACAddress() const override;
    void HW_EndAction() override;

    void Reset();
    void GetStatus_Internal(ActionStatus *status) const;

    uint16_t *MapKernelBuffer(uint32_t n_buf);
public:
    PCIExpressDevice(uint16_t data_stream);
    PCIExpressDevice(const std::string &device_name, uint16_t data_stream);
    ~PCIExpressDevice();

    void SetMACAddress(uint64_t mac_addr_network_order);
    void HW_GetStatus(ActionStatus *status) const override;
    void HW_GetEnvParams(ActionEnvParams *status) const override;
    uint32_t GetNumKernelBuffers() const;
};


#endif //JUNGFRAUJOCH_PCIEXPRESSDEVICE_H
