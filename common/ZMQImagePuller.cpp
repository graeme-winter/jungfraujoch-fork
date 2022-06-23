// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include "ZMQImagePuller.h"
#include "ImageMetadata.h"

ZMQImagePuller::ZMQImagePuller(ZMQContext &context, const std::string &in_address) :
        socket (context, ZMQSocketType::Pull) {
    socket.ReceiveWaterMark(ReceiverWaterMark);
    socket.ReceiveTimeout(ReceiveTimeout);
    socket.Bind(in_address);
}

void ZMQImagePuller::Start() {
    abort = 0;
}

void ZMQImagePuller::Abort() {
    abort = 1;
}

int64_t ZMQImagePuller::GetImage(std::vector<uint8_t> &image) {
    int64_t msg_size = socket.Receive(image, true, true);
    while ((msg_size < 0) && (!abort)) {
        auto now = std::chrono::system_clock::now();
        msg_size = socket.Receive(image, true, true);
    }
    if (msg_size > sizeof(ImageMetadata)) {
        // Start counting from the time the first message received
        if (processed_size == 0)
            start_time = std::chrono::system_clock::now();
        processed_images++;
        processed_size += msg_size - sizeof(ImageMetadata);
    }
    return msg_size;
}

ZMQImagePullerStatistics ZMQImagePuller::GetStatistics() {
    ZMQImagePullerStatistics stats;
    stats.nimages = processed_images;
    if (processed_images > 0) {
        auto now = std::chrono::system_clock::now();
        auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time);
        // MByte/s ==> Byte/us
        stats.throughput_MBs = std::lround(static_cast<double>(processed_size) / static_cast<double>(time_us.count()));
        stats.throughput_Hz = std::lround(static_cast<double>(processed_images) * 1e6 / static_cast<double>(time_us.count()));
    } else {
        stats.throughput_Hz = 0;
        stats.throughput_MBs = 0;
    }
    return stats;
}
