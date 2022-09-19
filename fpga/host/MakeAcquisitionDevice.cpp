// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MakeAcquisitionDevice.h"
#include "HLSSimulatedDevice.h"
#include "PCIExpressDevice.h"
#include "MockAcquisitionDevice.h"
#include "../../common/JFJochException.h"

#ifdef JFJOCH_USE_OPENCAPI
#include "OpenCAPIDevice.h"
#endif

std::unique_ptr<AcquisitionDevice> MakeAcquisitionDevice(AcquisitionDeviceType type, uint16_t data_stream,
                                                         size_t in_frame_buffer_size_modules,
                                                         uint16_t pci_slot_number,
                                                         int16_t numa_node) {
    switch (type) {
        case AcquisitionDeviceType::OpenCAPI:
#ifdef JFJOCH_USE_OPENCAPI
        return std::make_unique<OpenCAPIDevice>(pci_slot_number, data_stream, in_frame_buffer_size_modules, numa_node);
#else
            throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                                  "OpenCAPI support not compiled");
#endif
        case AcquisitionDeviceType::HLS:
            if (data_stream != 0)
                throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                                      "HLS simulation possible with a single data stream only");
            return std::make_unique<HLSSimulatedDevice>(data_stream, in_frame_buffer_size_modules, numa_node);
        case AcquisitionDeviceType::PCIe:
            return std::make_unique<PCIExpressDevice>(data_stream);
        case AcquisitionDeviceType::Mock:
        default: {
            auto tmp = std::make_unique<MockAcquisitionDevice>(data_stream, in_frame_buffer_size_modules);
            tmp->Terminate();
            return tmp;
        }

    }
}