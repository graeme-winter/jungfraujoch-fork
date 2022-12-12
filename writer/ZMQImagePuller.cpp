// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include "ZMQImagePuller.h"

ZMQImagePuller::ZMQImagePuller(ZMQContext &context) :
        socket (context, ZMQSocketType::Pull) {
    socket.ReceiveWaterMark(ReceiverWaterMark);
    socket.ReceiveTimeout(ReceiveTimeout);
    zmq_recv_buffer.reserve(2*1024*1024); // Reasonable size 2 MiB
}

void ZMQImagePuller::Connect(const std::string &in_address) {
    Disconnect();

    abort = 0;
    addr = in_address;
    socket.Connect(in_address);
}

void ZMQImagePuller::Disconnect() {
    if (!addr.empty())
        socket.Disconnect(addr);
    addr = "";
}

void ZMQImagePuller::Abort() {
    abort = 1;
}

void ZMQImagePuller::WaitForImage() {
    int64_t msg_size = -1;

    while ((msg_size < 0) && (!abort))
        msg_size = socket.Receive(zmq_recv_buffer, true, true);

    if (!abort) {
        deserializer.Process(zmq_recv_buffer);
        if (deserializer.GetType() == JFJochFrameDeserializer::Type::START)
            start_time = std::chrono::system_clock::now();
        else if (deserializer.GetType() == JFJochFrameDeserializer::Type::IMAGE) {
            processed_images++;
            processed_size += deserializer.GetImageSize();
        }
    }
}

const uint8_t *ZMQImagePuller::GetImage() const {
    return deserializer.GetImageData();
}

size_t ZMQImagePuller::GetImageSize() const {
    return deserializer.GetImageSize();
}

int64_t ZMQImagePuller::GetImageNumber() const {
    return deserializer.GetImageNumber();
}

const std::vector<SpotToSave>& ZMQImagePuller::GetSpots() const {
    return deserializer.GetSpots();
}

JFJochFrameDeserializer::Type ZMQImagePuller::GetFrameType() const {
    if (abort)
        return JFJochFrameDeserializer::Type::END;
    else
        return deserializer.GetType();
}

JFJochProtoBuf::WriterOutput ZMQImagePuller::GetStatistics() {
    JFJochProtoBuf::WriterOutput stats;
    stats.set_nimages(processed_images);
    if (processed_images > 0) {
        auto now = std::chrono::system_clock::now();
        auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time);
        // MByte/s ==> Byte/us
        stats.set_performance_mbs(static_cast<float>(processed_size) / static_cast<float>(time_us.count()));
        stats.set_performance_hz(static_cast<float>(processed_images) * 1e6 / static_cast<float>(time_us.count()));
    } else {
        stats.set_performance_mbs(0);
        stats.set_performance_hz(0);
    }
    return stats;
}
