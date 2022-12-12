// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQImagePusher.h"
#include "JFJochException.h"

ZMQImagePusher::ZMQImagePusher(ZMQContext &zmq_context, const std::vector<std::string> &addr,
                               int32_t send_buffer_high_watermark, int32_t send_buffer_size) {
    if (addr.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "No writer ZMQ address provided");

    for (const auto &a : addr) {
        auto s = std::make_unique<ZMQSocket>(zmq_context, ZMQSocketType::Push);
        if (send_buffer_size > 0)
            s->SendBufferSize(send_buffer_size);
        if (send_buffer_high_watermark > 0)
            s->SendWaterMark(send_buffer_high_watermark);
        s->Bind(a);
        sockets.push_back(std::move(s));
    }
}

ZMQImagePusher::ZMQImagePusher(const std::vector<std::string> &addr,
                               int32_t send_buffer_high_watermark, int32_t send_buffer_size) {
    if (addr.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "No writer ZMQ address provided");

    for (const auto &a : addr) {
        auto c = std::make_unique<ZMQContext>();
        auto s = std::make_unique<ZMQSocket>(*c, ZMQSocketType::Push);
        if (send_buffer_size > 0)
            s->SendBufferSize(send_buffer_size);
        if (send_buffer_high_watermark > 0)
            s->SendWaterMark(send_buffer_high_watermark);
        s->Bind(a);
        contexts.push_back(std::move(c));
        sockets.push_back(std::move(s));
    }
}

void ZMQImagePusher::SendData(void *image, size_t image_size, const std::vector<DiffractionSpot>& spots,
                              int64_t image_number) {
    std::unique_lock<std::mutex> ul(m);
    if (sockets.empty())
        return;

    std::vector<SpotToSave> spots_to_save;
    for (const auto & spot : spots)
        spots_to_save.push_back(spot);

    size_t msg_size = serializer.SerializeImage(image, image_size, image_number, spots_to_save);

    auto socket_number = (image_number % file_count) % sockets.size();
    sockets[socket_number]->Send(serializer.GetBuffer().data(), msg_size);
}

void ZMQImagePusher::StartDataCollection(int64_t in_file_count) {
    std::unique_lock<std::mutex> ul(m);

    if (in_file_count < 1)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "File count cannot be zero or negative");
    file_count = in_file_count;

    size_t msg_size = serializer.SerializeSequenceStart();
    for (const auto &s: sockets)
        s->Send(serializer.GetBuffer().data(), msg_size, true);
}


void ZMQImagePusher::EndDataCollection() {
    std::unique_lock<std::mutex> ul(m);
    size_t msg_size = serializer.SerializeSequenceEnd();
    for (const auto &s: sockets)
        s->Send(serializer.GetBuffer().data(), msg_size, true);
}