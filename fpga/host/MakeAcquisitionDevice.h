// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_MAKEACQUISITIONDEVICE_H
#define JUNGFRAUJOCH_MAKEACQUISITIONDEVICE_H

#include "AcquisitionDevice.h"

enum class AcquisitionDeviceType {OpenCAPI, PCIe, HLS, Mock};

std::unique_ptr<AcquisitionDevice> MakeAcquisitionDevice(AcquisitionDeviceType type, uint16_t data_stream,
                                                         size_t in_frame_buffer_size_modules = 1024,
                                                         uint16_t pci_slot_number = 0,
                                                         int16_t numa_node = -1);
#endif //JUNGFRAUJOCH_MAKEACQUISITIONDEVICE_H
