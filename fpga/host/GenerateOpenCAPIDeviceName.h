// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_GENERATEOPENCAPIDEVICENAME_H
#define JUNGFRAUJOCH_GENERATEOPENCAPIDEVICENAME_H

#include <cstdio>
#include <string>

inline std::string GenerateOpenCAPIDeviceName(uint16_t slot_number) {
    char device[255];
    snprintf(device, sizeof(device)-1, "/dev/ocxl/IBM,oc-snap.%04d:00:00.1.0", slot_number);
    return {device};
}

#endif //JUNGFRAUJOCH_GENERATEOPENCAPIDEVICENAME_H
