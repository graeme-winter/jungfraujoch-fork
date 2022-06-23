// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQImagePusher.h"
#include "JFJochException.h"

ZMQImagePusher::ZMQImagePusher(ZMQContext &context)
           : zmq_context(context) {
}

void ZMQImagePusher::EndDataCollection() {
    for (const auto &s: sockets)
        s->Send();
}

void ZMQImagePusher::SendData(void *buffer, size_t image_number, size_t buffer_size) {
    if (sockets.empty())
        return;
    auto metadata = (ImageMetadata *) buffer;
    metadata->version = IMAGE_METADATA_VERSION;
    metadata->image_size = buffer_size;
    metadata->frameid = image_number;

    auto socket_number = experiment.GetFileForImage(image_number) % sockets.size();
    sockets[socket_number]->Send(buffer, buffer_size + sizeof(ImageMetadata));
}

void ZMQImagePusher::Connect(const std::vector<std::string>& in_address, const JFJochProtoBuf::JungfraujochSettings &settings) {
    Disconnect();

    if (!sockets.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Socket must be empty");

    if (in_address.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "No writer ZMQ address provided");

    address = in_address;
    experiment = DiffractionExperiment(settings);

    for (const auto &a : address) {
        auto s = std::make_unique<ZMQSocket>(zmq_context, ZMQSocketType::Push);
        if (send_buffer_size > 0)
            s->SendBufferSize(send_buffer_size);
        if (send_buffer_high_watermark > 0)
            s->SendWaterMark(send_buffer_high_watermark);
        s->Connect(a);
        sockets.push_back(std::move(s));
    }
}

void ZMQImagePusher::Disconnect() {
    for (int i = 0 ; i < sockets.size(); i++) {
        if (!address[i].empty())
            sockets[i]->Disconnect(address[i]);
        address[i] = "";
    }
    sockets.clear();
    address.clear();
}

ZMQImagePusher &ZMQImagePusher::SendBufferSize(int32_t bytes) {
    send_buffer_size = bytes;
    return *this;
}

ZMQImagePusher &ZMQImagePusher::SendBufferHighWatermark(int32_t msgs) {
    send_buffer_high_watermark = msgs;
    return *this;
}
