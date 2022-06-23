// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENCAPIWRAPPER_H
#define OPENCAPIWRAPPER_H

#include <unistd.h>
#include <libosnap.h>
#include <osnap_hls_if.h>

#include "../../common/Definitions.h"
#include "../../common/DiffractionExperiment.h"
#include "../../common/Logger.h"
#include "AcquisitionDevice.h"
#include <bitset>

enum class MMIORegion : size_t {HLS = 0x00000, CMAC = 0x40000, MAILBOX = 0x30000, AD_BCI = 0x10000, CALIB_BRAM = 0x60000};

class OpenCAPIDevice  : public AcquisitionDevice {
// OpenCAPI card
    mutable std::mutex m;
    std::string device_name;
    snap_card *card = nullptr;
    snap_action *action = nullptr;

    uint32_t max_modules = 0;
    uint32_t max_modules_internal_packet_generator = 0;

    bool internal_packet_gen = false;

    void SetCalibrationInputLocation(uint32_t location, uint64_t addr);
    uint32_t GetActionCtrlRegister();
    bool IsHBMAligned();
    bool Is100GRxAligned();
    bool IsOpticalModulePresent();

    void CheckVersion();
    void WriteMMIORegister(MMIORegion offset, uint32_t addr, uint32_t val);
    uint32_t ReadMMIORegister(MMIORegion offset, uint32_t addr) const;
    bool CheckRegister(MMIORegion offset, uint32_t addr, uint32_t expected_val);

    uint32_t HW_GetInternalPacketGeneratorModuleNum() override;
    uint32_t HW_GetMaxModuleNum() override;
    bool HW_ReadMailbox(uint32_t values[4]) override;
    void HW_SetCancelDataCollectionBit() override;
    bool HW_SendWorkRequest(uint32_t handle) override;
    void HW_StartAction() override;
    bool HW_IsIdle() const final;
    void HW_WriteActionRegister(const ActionConfig *job) override;
    void HW_ReadActionRegister(ActionConfig *job) override;
    uint64_t HW_GetMACAddress() const override;
public:
    void SetMACAddress(uint64_t mac_addr_network_order);
    void HW_GetStatus(ActionStatus *status) const override;
    void HW_GetEnvParams(ActionEnvParams *status) const override;
    OpenCAPIDevice(std::string device_name, uint16_t data_stream, size_t in_frame_buffer_size_modules,
                   int16_t numa_node = -1);
    OpenCAPIDevice(uint16_t pci_slot_number, uint16_t data_stream, size_t in_frame_buffer_size_modules,
                   int16_t numa_node = -1);
    ~OpenCAPIDevice();
};

#endif //JUNGFRAUJOCH_OPENCAPIWRAPPER_H
