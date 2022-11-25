// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHFRAMESERIALIZER_H
#define JUNGFRAUJOCH_JFJOCHFRAMESERIALIZER_H

#include <vector>
#include <cstdint>
#include "../common/SpotToSave.h"

class JFJochFrameSerializer {
    std::vector<uint8_t> buffer;
    size_t max_buffer_size;
public:
    JFJochFrameSerializer(size_t buffer_size);
    const std::vector<uint8_t> &GetBuffer() const;
    size_t SerializeSequenceStart();
    size_t SerializeSequenceEnd();
    size_t SerializeImage(void *image, size_t image_size,
                          const std::pair<int64_t,int64_t> &image_location_in_file,
                          const std::vector<SpotToSave>& spots);
};


#endif //JUNGFRAUJOCH_JFJOCHFRAMESERIALIZER_H
