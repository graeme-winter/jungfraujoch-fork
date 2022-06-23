// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQIMAGEPUSHER_H
#define JUNGFRAUJOCH_ZMQIMAGEPUSHER_H

#include "CommunicationBuffer.h"
#include "JungfrauCalibration.h"
#include "ThreadSafeFIFO.h"
#include "jfjoch.pb.h"
#include "ZMQWrappers.h"

class ZMQImagePusher {
    ZMQContext& zmq_context;
    DiffractionExperiment experiment;
    std::vector<std::unique_ptr<ZMQSocket>> sockets;
    std::vector<std::string> address;
    int32_t send_buffer_size = -1;
    int32_t send_buffer_high_watermark = -1;
public:
    ZMQImagePusher(ZMQContext &context);

    void Connect(const std::vector<std::string>& addr, const JFJochProtoBuf::JungfraujochSettings &settings);
    void Disconnect();

    void EndDataCollection();
    void SendData(void *buffer, size_t image_number, size_t image_size);
    ZMQImagePusher& SendBufferHighWatermark(int32_t msgs);
    ZMQImagePusher& SendBufferSize(int32_t bytes);
};


#endif //JUNGFRAUJOCH_ZMQIMAGEPUSHER_H
