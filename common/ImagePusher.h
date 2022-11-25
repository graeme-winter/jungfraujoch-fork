// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_IMAGEPUSHER_H
#define JUNGFRAUJOCH_IMAGEPUSHER_H

#include <cstdint>
#include <vector>
#include "DiffractionSpot.h"

class ImagePusher {
public:
    virtual void StartDataCollection() = 0;
    virtual void EndDataCollection() = 0;
    virtual void SendData(void *image, const std::pair<int64_t,int64_t> &image_location_in_file,
                  size_t image_size, const std::vector<DiffractionSpot>& spots) = 0;
};


#endif //JUNGFRAUJOCH_IMAGEPUSHER_H
