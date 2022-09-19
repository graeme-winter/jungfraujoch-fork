// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>
#include <utility>

#include "OpenCAPIDevice.h"
#include "../../common/JFJochException.h"
#include "GenerateOpenCAPIDeviceName.h"

OpenCAPIDevice::OpenCAPIDevice(std::string in_device_name, uint16_t data_stream, size_t in_frame_buffer_size_modules,
                               int16_t numa_node) :
        device_name(std::move(in_device_name)),
        AcquisitionDevice(data_stream) {
    card = snap_card_alloc_dev(device_name.c_str(), SNAP_VENDOR_ID_IBM, SNAP_DEVICE_ID_SNAP);
    if (card == nullptr)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "Failed to attach to the device " + device_name);

    // Attach the action that will be used on the allocated card
    action = snap_attach_action(card, ACTION_TYPE, (snap_action_flag_t) 0, 60);
    if (action == nullptr) {
        snap_card_free(card);
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "Failed to attach action to the device " + device_name);
    }

    CheckVersion();

    max_modules = ReadMMIORegister(MMIORegion::HLS, ADDR_MAX_MODULES_FPGA);

    if (max_modules == 0)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "Max modules cannot be zero");
    MapBuffersStandard(in_frame_buffer_size_modules,
                       (3+3*16) * max_modules + 1,
                       numa_node);

    for (int i = 0; i < max_modules * (3 + 3 * 16) + 1; i++)
        SetCalibrationInputLocation(i, (uint64_t) buffer_device[i]);

    // Set Mailbox FIFOs, so interrupt threshold is 4 messages
    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_SIT, 251);
    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_RIT, 3);
}

OpenCAPIDevice::~OpenCAPIDevice() {
    // ensure action is off
    WriteMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER, (1 << 2u));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    snap_detach_action(action);
    snap_card_free(card);
}

bool OpenCAPIDevice::HW_ReadMailbox(uint32_t values[4]) {
    if (!(ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_STATUS) & MAILBOX_RTA))
        //  The receive FIFO level is less than or equal to the RIT threshold
        return false;

    values[0] = ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_RDDATA);
    values[1] = ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_RDDATA);
    values[2] = ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_RDDATA);
    values[3] = ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_RDDATA);
    return true;
}

void OpenCAPIDevice::HW_SetCancelDataCollectionBit() {
    WriteMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER, (1 << 2u));
}

bool OpenCAPIDevice::HW_SendWorkRequest(uint32_t handle) {
    if (!(ReadMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_STATUS) & MAILBOX_STA))
        // The send FIFO level is greater than the SIT threshold
        return false;

    uint64_t address = (handle == UINT32_MAX) ? 0 : (uint64_t) buffer_device.at(handle);
    uint32_t parity = (std::bitset<32>(handle).count() + std::bitset<64>(address).count()) % 2;

    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_WRDATA, handle);
    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_WRDATA, address >> 32);
    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_WRDATA, address & UINT32_MAX);
    WriteMMIORegister(MMIORegion::MAILBOX, ADDR_MAILBOX_WRDATA, parity);
    return true;
}

OpenCAPIDevice::OpenCAPIDevice(uint16_t pci_slot_number, uint16_t data_stream, size_t in_frame_buffer_size_modules,
                               int16_t numa_node) :
        OpenCAPIDevice(GenerateOpenCAPIDeviceName(pci_slot_number), data_stream, in_frame_buffer_size_modules,
                       numa_node) {

}

uint32_t OpenCAPIDevice::ReadMMIORegister(MMIORegion offset, uint32_t addr) const {
    std::unique_lock<std::mutex> ul(m);
    uint32_t tmp;

    if (snap_action_read32 (card, static_cast<size_t>(offset) + addr, &tmp))
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "Cannot read MMIO register");

    return tmp;
}

void OpenCAPIDevice::WriteMMIORegister(MMIORegion offset, uint32_t addr, uint32_t val) {
    std::unique_lock<std::mutex> ul(m);
    if (snap_action_write32(card, static_cast<size_t>(offset) + addr, val))
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "Cannot write MMIO register");
}

bool OpenCAPIDevice::CheckRegister(MMIORegion offset, uint32_t addr, uint32_t expected_val) {
    return ReadMMIORegister(offset, addr) == expected_val;
}

void OpenCAPIDevice::CheckVersion() {
    if (!CheckRegister(MMIORegion::HLS, ADDR_ACTION_TYPE, ACTION_TYPE))
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Action type mismatch between software and OpenCAPI flash on " + device_name);
    if (!CheckRegister(MMIORegion::HLS, ADDR_RELEASE_LEVEL, RELEASE_LEVEL))
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Version mismatch between software and OpenCAPI flash on " + device_name);
}

void OpenCAPIDevice::HW_StartAction() {
    // Ensure Ethernet is running
    if (!internal_packet_gen) {
        if (!IsOpticalModulePresent())
            throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                                  "Optical module missing for device " + device_name);

        if (!Is100GRxAligned())
            throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                                  "100G ethernet is not ready for device " + device_name);
    }

    // Ensure HBM alignment
#ifndef __PPC__
    // In case of simulation, there is some wait necessary
    size_t counter = 0;

    while ((counter < 600) && !IsHBMAligned()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter++;
    }
#endif

    if (!IsHBMAligned())
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "HBM is not ready for device " + device_name);

    // GO!
    WriteMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER, CTRL_REGISTER_START);
}

bool OpenCAPIDevice::IsHBMAligned() {
    return GetActionCtrlRegister() & CTRL_REGISTER_HBM_SETUP_COMPLETE;
}

bool OpenCAPIDevice::Is100GRxAligned() {
    return GetActionCtrlRegister() & CTRL_REGISTER_100G_ALIGNED; // RX_ALIGNED
}

bool OpenCAPIDevice::HW_IsIdle() const {
    return ReadMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER) & CTRL_REGISTER_IDLE;
}

void OpenCAPIDevice::HW_GetStatus(ActionStatus *status) const {
    auto ptr = (uint32_t *) status;
    for (int i = 0; i < sizeof(ActionStatus) / 4; i ++)
        ptr[i] = ReadMMIORegister(MMIORegion::HLS, i * 4);
}

void OpenCAPIDevice::HW_GetEnvParams(ActionEnvParams *status) const {

    status->mailbox_status_reg   = ReadMMIORegister(MMIORegion::MAILBOX, 0x10);
    status->mailbox_err_reg      = ReadMMIORegister(MMIORegion::MAILBOX, 0x14);

    uint32_t temp = ReadMMIORegister(MMIORegion::AD_BCI, 0xA28);
    status->fpga_temp_C = std::lround((temp >> 16)/16.0 - 273.15);

    uint32_t fpga_status_rail_12V  = ReadMMIORegister(MMIORegion::AD_BCI, 0xA08);
    uint32_t fpga_status_rail_3p3V = ReadMMIORegister(MMIORegion::AD_BCI, 0xA0C);

    status->fpga_pcie_12V_I_mA     = std::lround((fpga_status_rail_12V >> 16) / 4096.0 * 1000.0);
    status->fpga_pcie_3p3V_I_mA    = std::lround((fpga_status_rail_3p3V >> 16) / 4096.0 * 1000.0);

    status->fpga_pcie_12V_V_mV     = std::lround((fpga_status_rail_12V & 0xFFFF) / 4096.0 * 1000.0);
    status->fpga_pcie_3p3V_V_mV    = std::lround((fpga_status_rail_3p3V & 0xFFFF) / 4096.0 * 1000.0);
}

void OpenCAPIDevice::HW_WriteActionRegister(const ActionConfig *job) {
    internal_packet_gen = job->mode & MODE_INTERNAL_PACKET_GEN;
    auto ptr = (uint32_t *) job;
    for (int i = 0; i < sizeof(ActionConfig)/4; i++)
        WriteMMIORegister(MMIORegion::HLS, ADDR_IPV4_ADDR + i * 4, ptr[i]);
}

void OpenCAPIDevice::HW_ReadActionRegister(ActionConfig *job) {
    auto ptr = (uint32_t *) job;
    for (int i = 0; i < sizeof(ActionConfig)/4; i++)
        ptr[i] = ReadMMIORegister(MMIORegion::HLS, ADDR_IPV4_ADDR + i * 4);
}

bool OpenCAPIDevice::IsOpticalModulePresent() {
    return (ReadMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER) & CTRL_REGISTER_QSFP_MOD_PRESENT);
}

void OpenCAPIDevice::SetCalibrationInputLocation(uint32_t location, uint64_t addr) {
    if (location > 512)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Addr out of bounds");
    WriteMMIORegister(MMIORegion::CALIB_BRAM, (location * 2)     * 4, addr & UINT32_MAX);
    WriteMMIORegister(MMIORegion::CALIB_BRAM, (location * 2 + 1) * 4, addr >> 32);
}

uint64_t OpenCAPIDevice::HW_GetMACAddress() const {
    return bit_concat(ReadMMIORegister(MMIORegion::HLS, ADDR_MAC_ADDR_HI),
                      ReadMMIORegister(MMIORegion::HLS, ADDR_MAC_ADDR_LO));
}

void OpenCAPIDevice::SetMACAddress(uint64_t mac_addr_network_order) {
    WriteMMIORegister(MMIORegion::HLS, ADDR_MAC_ADDR_LO, mac_addr_network_order & UINT32_MAX);
    WriteMMIORegister(MMIORegion::HLS, ADDR_MAC_ADDR_HI, mac_addr_network_order >> 32);
}

uint32_t OpenCAPIDevice::GetActionCtrlRegister() {
    return ReadMMIORegister(MMIORegion::HLS, ADDR_CTRL_REGISTER);
}