// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQIMAGEPUSHER_H
#define JUNGFRAUJOCH_ZMQIMAGEPUSHER_H

#include <jfjoch.pb.h>

#include "ImagePusher.h"
#include "ThreadSafeFIFO.h"
#include "ZMQWrappers.h"
#include "DiffractionSpot.h"
#include "../frame_serialize/JFJochFrameSerializer.h"

class ZMQImagePusher : public ImagePusher {
    std::mutex m;
    std::vector<std::unique_ptr<ZMQContext>> contexts;
    std::vector<std::unique_ptr<ZMQSocket>> sockets;
    JFJochFrameSerializer serializer{16*1024*1024};
    int64_t file_count = 1;
public:
    ZMQImagePusher(ZMQContext &context, const std::vector<std::string>& addr,
                   int32_t send_buffer_high_watermark = -1, int32_t send_buffer_size = -1);
    // High performance implementation, where each socket has dedicated ZMQ context
    explicit ZMQImagePusher(const std::vector<std::string>& addr,
                   int32_t send_buffer_high_watermark = -1, int32_t send_buffer_size = -1);
    void StartDataCollection(int64_t file_count) override;
    void EndDataCollection() override;
    void SendData(void *image, size_t image_size, const std::vector<DiffractionSpot>& spots,
                  int64_t image_number) override;
};

#endif //JUNGFRAUJOCH_ZMQIMAGEPUSHER_H
