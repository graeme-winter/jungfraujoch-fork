// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_COMMUNICATIONBUFFER_H
#define JUNGFRAUJOCH_COMMUNICATIONBUFFER_H

#include <cstddef>
#include "DiffractionExperiment.h"
#include "ImageMetadata.h"

class CommunicationBuffer {
protected:
    size_t image_size = 0;
    size_t max_images = 0;
    size_t cap_images = 0;
    size_t buffer_size;
    uint8_t *buffer;
public:
    explicit CommunicationBuffer(size_t buffer_size, size_t cap_images = 0);
    ~CommunicationBuffer();

    void SetMaxImageSize(size_t image_size);

    uint8_t *GetBuffer();
    size_t GetBufferSize() const;
    size_t GetBufferLocationSize() const;
    size_t GetBufferLocationID(const uint8_t *ptr) const;
    size_t GetBufferImageSize() const; // without metadata!

    uint8_t *GetBufferLocation(size_t id); // image metadata + image
    size_t GetNumOfImages() const;
    uint8_t *GetImage(size_t id);

    ImageMetadata *Metadata(size_t id);
};

#endif //JUNGFRAUJOCH_COMMUNICATIONBUFFER_H
