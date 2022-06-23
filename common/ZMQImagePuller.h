// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQIMAGEPULLER_H
#define JUNGFRAUJOCH_ZMQIMAGEPULLER_H

#include <future>
#include "DiffractionExperiment.h"
#include "jfjoch.pb.h"
#include "ZMQWrappers.h"
#include "Logger.h"

struct ZMQImagePullerStatistics {
    size_t nimages;
    int64_t throughput_MBs;
    int64_t throughput_Hz;
};

class ZMQImagePuller {
    constexpr const static uint32_t ReceiverWaterMark = 1000;
    // ZeroMQ receive timeout allows to check for abort value from time to time
    constexpr const static auto ReceiveTimeout = std::chrono::milliseconds(100);
    size_t processed_size = 0;
    size_t processed_images = 0;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    ZMQSocket socket;
    volatile int abort = 0;
public:
    ZMQImagePuller(ZMQContext &context, const std::string &in_address);
    void Start();
    int64_t GetImage(std::vector<uint8_t> &image);
    void Abort();
    ZMQImagePullerStatistics GetStatistics();
};

#endif //JUNGFRAUJOCH_ZMQIMAGEPULLER_H
