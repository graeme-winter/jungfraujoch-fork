// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef JFJOCH_USE_NUMA
#include <numa.h>
#endif

#include <sys/mman.h>
#include "CommunicationBuffer.h"
#include "JFJochException.h"

CommunicationBuffer::CommunicationBuffer(size_t in_buffer_size, size_t in_cap_images) {
    // MMAP will return page aligned memory, which could be beneficial for raw data (8 kB is exactly two pages)
    buffer_size = in_buffer_size;
    cap_images = in_cap_images;
#ifdef JFJOCH_USE_NUMA
    buffer = (uint8_t *) numa_alloc_interleaved(buffer_size);
#else
    buffer = (uint8_t *) mmap (nullptr, buffer_size, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
#endif
    if (buffer == nullptr)
        throw JFJochException(JFJochExceptionCategory::MemAllocFailed, "Failed to allocate communication buffer");

    memset(buffer, 0, in_buffer_size);
}

CommunicationBuffer::~CommunicationBuffer() {
#ifdef JFJOCH_USE_NUMA
    numa_free(buffer, buffer_size);
#else
    munmap(buffer, buffer_size);
#endif
}

uint8_t *CommunicationBuffer::GetBuffer() {
    return buffer;
}

size_t CommunicationBuffer::GetBufferSize() const {
    return buffer_size;
}

size_t CommunicationBuffer::GetBufferImageSize() const {
    return image_size - sizeof(ImageMetadata);
}

size_t CommunicationBuffer::GetBufferLocationSize() const {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    return image_size;
}

void CommunicationBuffer::SetMaxImageSize(size_t in_image_size) {
    image_size = in_image_size + sizeof(ImageMetadata);
    max_images = buffer_size / image_size;
    if (cap_images > 0)
        max_images = std::min(max_images, cap_images);
}

uint8_t* CommunicationBuffer::GetBufferLocation(size_t id) {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    if (id >= max_images)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Image entry out of buffer bounds");

    return buffer + image_size * id;
}

uint8_t* CommunicationBuffer::GetImage(size_t id) {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    if (id >= max_images)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Image entry out of buffer bounds");

    return buffer + image_size * id + sizeof(ImageMetadata);
}

size_t CommunicationBuffer::GetNumOfImages() const {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    return max_images;
}

size_t CommunicationBuffer::GetBufferLocationID(const uint8_t *ptr) const {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    if (ptr < buffer)
            throw JFJochException(JFJochExceptionCategory::WrongNumber, "Pointer outside of buffer location");

    if (ptr >= buffer + buffer_size)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Pointer outside of buffer location");
    return (ptr - buffer) / image_size;
}

ImageMetadata *CommunicationBuffer::Metadata(size_t id) {
    if (image_size == 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Buffer not initialized");
    return (ImageMetadata *) GetBufferLocation(id);
}
