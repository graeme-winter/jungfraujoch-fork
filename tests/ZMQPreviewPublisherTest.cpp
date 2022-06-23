// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/ZMQPreviewPublisher.h"

TEST_CASE("ZMQPreviewPublisher","[ZSTD]") {
    ZMQContext context;
    ZMQPreviewPublisher publisher(context, "inproc://#5");

    ZMQSocket socket(context, ZMQSocketType::Sub);
    socket.Connect("inproc://#5");
    socket.SubscribeAll();

    DiffractionExperiment experiment(1,{1});
    experiment.UpsideDown(false);

    JungfrauCalibration calibration(experiment);
    publisher.Start(experiment, calibration);

    std::vector<int16_t> image(experiment.GetPixelsNum());

    // Predictable random number generator
    std::mt19937 g1(19876);
    std::uniform_int_distribution<int16_t> distribution(-200,25000);
    for (auto &i: image)
        i = distribution(g1);

    publisher.Publish(experiment, image.data(), 546);

    std::string s;

    // Pixel mask
    REQUIRE(socket.Receive(s, false) > 0);
    JFJochProtoBuf::PreviewFrame frame;
    REQUIRE_NOTHROW(frame.ParseFromString(s));
    REQUIRE(frame.pixel_depth() == 4);
    REQUIRE(frame.image_number() == -1);

    // Frame
    REQUIRE(socket.Receive(s, false) > 0);

    REQUIRE_NOTHROW(frame.ParseFromString(s));

    REQUIRE(frame.pixel_depth() == 2);
    REQUIRE(frame.image_number() == 546);
    std::vector<char> image_out = {frame.data().begin(), frame.data().end()};

    REQUIRE(memcmp(image.data(), image_out.data(), experiment.GetPixelsNum() * experiment.GetPixelDepth()) == 0);
    REQUIRE(socket.Receive(s, false) < 0);
}

TEST_CASE("ZMQPreviewPublisher_GetPreviewImage","[ZSTD]") {
    ZMQContext context;
    ZMQPreviewPublisher publisher(context, "inproc://#5");

    DiffractionExperiment experiment(1,{1});
    experiment.UpsideDown(false);

    JungfrauCalibration calibration(experiment);
    publisher.Start(experiment, calibration);

    std::vector<int16_t> image(experiment.GetPixelsNum());

    // Predictable random number generator
    std::mt19937 g1(19876);
    std::uniform_int_distribution<int16_t> distribution(-200,25000);
    for (auto &i: image)
        i = distribution(g1);

    publisher.Publish(experiment, image.data(), 546);

    JFJochProtoBuf::PreviewFrame frame = publisher.GetPreviewImage();

    REQUIRE(frame.pixel_depth() == 2);
    REQUIRE(frame.image_number() == 546);
    std::vector<char> image_out = {frame.data().begin(), frame.data().end()};

    REQUIRE(memcmp(image.data(), image_out.data(), experiment.GetPixelsNum() * experiment.GetPixelDepth()) == 0);
}