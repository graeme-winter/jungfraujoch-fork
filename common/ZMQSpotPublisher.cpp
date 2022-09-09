// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQSpotPublisher.h"

ZMQSpotPublisher::ZMQSpotPublisher(ZMQContext &context, const std::string &addr)
   : socket(context, ZMQSocketType::Pub) {
    socket.SendWaterMark(100).NoLinger(); // allow to close the socket immediately + reduce watermark
    socket.Bind(addr);
}

void ZMQSpotPublisher::Publish(const DiffractionExperiment &experiment, const std::vector<DiffractionSpot> &vec,
                               int64_t image_number) {
    std::vector<Coord> recip;
    for (const auto &i: vec)
        recip.push_back(i.ReciprocalCoord(experiment));
    PublishReciprocal(experiment, recip, image_number);
}

void ZMQSpotPublisher::PublishReciprocal(const DiffractionExperiment& experiment, const std::vector<Coord> &vec,
                                         int64_t image_number) {
    JFJochProtoBuf::SpotFinderImageOutput list;
    list.set_image_number(image_number);
    *list.mutable_coord() = {vec.begin(), vec.end()};
    socket.Send(list.SerializeAsString());
}