// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQIMAGEPULLER_H
#define JUNGFRAUJOCH_ZMQIMAGEPULLER_H

#include <future>

#include <jfjoch.pb.h>

#include "../common/ZMQWrappers.h"
#include "../common/Logger.h"
#include "../common/SpotToSave.h"
#include "../frame_serialize/JFJochFrameDeserializer.h"

class ZMQImagePuller {
    std::vector<uint8_t> zmq_recv_buffer;

    JFJochFrameDeserializer deserializer;

    constexpr const static uint32_t ReceiverWaterMark = 1000;
    // ZeroMQ receive timeout allows to check for abort value from time to time
    constexpr const static auto ReceiveTimeout = std::chrono::milliseconds(100);
    size_t processed_size = 0;
    size_t processed_images = 0;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    ZMQSocket socket;
    std::string addr;
    volatile int abort = 0;
public:
    ZMQImagePuller(ZMQContext &context);
    void Connect(const std::string &in_address);
    void Disconnect();
    void Abort();

    void WaitForImage();
    const uint8_t *GetImage() const;
    size_t GetImageSize() const;
    uint64_t GetImageNumber() const;
    uint64_t GetFileNumber() const;
    const std::vector<SpotToSave>& GetSpots() const;
    JFJochFrameDeserializer::Type GetFrameType() const;

    JFJochProtoBuf::WriterOutput GetStatistics();
};

#endif //JUNGFRAUJOCH_ZMQIMAGEPULLER_H
