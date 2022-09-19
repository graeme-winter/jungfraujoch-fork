// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "PCIExpressDevice.h"
#include "../../common/JFJochException.h"
#include "../pcie_driver/jfjoch_ioctl.h"
#include <thread>

PCIExpressDevice::PCIExpressDevice(uint16_t data_stream) :
PCIExpressDevice("/dev/jfjoch" + std::to_string(data_stream), data_stream) {}

PCIExpressDevice::PCIExpressDevice(const std::string &device_name, uint16_t data_stream)
: AcquisitionDevice(data_stream) {
    fd = open(device_name.c_str(), O_RDWR);
    if (fd == -1)
        throw JFJochException(JFJochExceptionCategory::PCIeError, "Cannot open device");

    ActionStatus status{};
    GetStatus_Internal(&status);
    max_modules = status.max_modules;
    if (max_modules == 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError, "Max modules cannot be zero");

    uint32_t num_buf = GetNumKernelBuffers();

    if (num_buf < max_modules * (3 + 3 * 16) + 1)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Need to increase number of host-device buffers");

    buffer_device.resize(num_buf, nullptr);
    try {
        for (int i = 0; i < num_buf; i++)
            buffer_device[i] = MapKernelBuffer(i);
    } catch (JFJochException &e) {
        UnmapBuffers();
        throw;
    }
}

PCIExpressDevice::~PCIExpressDevice() {
    close(fd);
}

uint16_t *PCIExpressDevice::MapKernelBuffer(uint32_t n_buf) {
    auto tmp = (uint16_t *) mmap(nullptr, FPGA_BUFFER_LOCATION_SIZE,
                                 PROT_READ | PROT_WRITE, MAP_SHARED,
                                 fd, FPGA_BUFFER_LOCATION_SIZE * n_buf);

    if (tmp == nullptr)
        throw JFJochException(JFJochExceptionCategory::PCIeError, "Mmap of kernel buffer error: "
                                                                  + std::string(strerror(errno)));

    return tmp;
}

bool PCIExpressDevice::HW_ReadMailbox(uint32_t *values) {
    int tmp = ioctl(fd, IOCTL_JFJOCH_READ_WC_MBOX, values);

    if (tmp != 0) {
        if (errno == EAGAIN)
            return false;
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed receiving work completion", errno);
    }

    return true;

}

void PCIExpressDevice::HW_SetCancelDataCollectionBit() {
    if (ioctl(fd, IOCTL_JFJOCH_CANCEL) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed setting cancel bit", errno);
}

bool PCIExpressDevice::HW_SendWorkRequest(uint32_t handle) {
    int tmp = ioctl(fd, IOCTL_JFJOCH_SEND_WR, &handle);
    if (tmp != 0) {
        if (errno == EAGAIN)
            return false;
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed sending work request", errno);
    }
    return true;
}

void PCIExpressDevice::HW_StartAction() {
    if (ioctl(fd, IOCTL_JFJOCH_START) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed starting action", errno);
}

void PCIExpressDevice::HW_EndAction() {
    if (ioctl(fd, IOCTL_JFJOCH_END) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed ending action", errno);
}

bool PCIExpressDevice::HW_IsIdle() const {
    uint32_t tmp;
    if (ioctl(fd, IOCTL_JFJOCH_ISIDLE, &tmp) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed checking if idle", errno);
    return tmp;
}

void PCIExpressDevice::HW_WriteActionRegister(const ActionConfig *config) {
    if (ioctl(fd, IOCTL_JFJOCH_SET_CONFIG, config) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed writing config", errno);
}

void PCIExpressDevice::HW_ReadActionRegister(ActionConfig *config) {
    if (ioctl(fd, IOCTL_JFJOCH_READ_CONFIG, config) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed reading config", errno);
}

uint64_t PCIExpressDevice::HW_GetMACAddress() const {
    uint64_t tmp;
    if (ioctl(fd, IOCTL_JFJOCH_GET_MAC, &tmp) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed getting MAC address", errno);
    return tmp;
}

void PCIExpressDevice::SetMACAddress(uint64_t mac_addr_network_order) {
    if (ioctl(fd, IOCTL_JFJOCH_SET_MAC, &mac_addr_network_order) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed setting MAC address", errno);

}

void PCIExpressDevice::HW_GetStatus(ActionStatus *status) const {
    GetStatus_Internal(status);
}

void PCIExpressDevice::GetStatus_Internal(ActionStatus *status) const {
    if (ioctl(fd, IOCTL_JFJOCH_STATUS, status) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed reading status", errno);
}

void PCIExpressDevice::HW_GetEnvParams(ActionEnvParams *status) const {
    if (ioctl(fd, IOCTL_JFJOCH_GET_ENV_DATA, status) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed reading env. data", errno);
}

uint32_t PCIExpressDevice::GetNumKernelBuffers() const {
    uint32_t tmp;
    if (ioctl(fd, IOCTL_JFJOCH_BUF_COUNT, &tmp) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed reading env. data", errno);
    return tmp;
}

void PCIExpressDevice::Reset() {
    if (ioctl(fd, IOCTL_JFJOCH_RESET) != 0)
        throw JFJochException(JFJochExceptionCategory::PCIeError,
                              "Failed reset", errno);

}