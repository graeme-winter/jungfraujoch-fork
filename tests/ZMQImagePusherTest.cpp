// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/ZMQImagePuller.h"
#include "../common/ZMQImagePusher.h"

TEST_CASE("ZMQImageCommTest_1Writer","[ZeroMQ]") {
    const size_t nframes = 256;

    ZMQContext context;
    Logger logger("test");
    DiffractionExperiment x, x_out;
    x.Mode(DetectorMode::Raw).DataStreamModuleSize(1, {1});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
        .ImagesPerTrigger(nframes);


    REQUIRE(x.GetImageNum() == nframes);

    JungfrauCalibration calib(x);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::string zmq_addr = "inproc://#1";

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    ZMQImagePuller puller(context, zmq_addr);
    ZMQImagePusher pusher(context);

    size_t diff_size = 0, diff_content = 0, diff_size_header = 0;
    size_t nimages = 0;

    std::vector<std::string> zmq_addr_v = {zmq_addr};
    pusher.Connect(zmq_addr_v, x);

    std::thread sender_thread = std::thread([&] {
        std::vector<char> pusher_buff(x.GetMaxCompressedSize() + sizeof(ImageMetadata));
        for (int i = 0; i < nframes; i++) {
            memcpy(pusher_buff.data() + sizeof(ImageMetadata),
                   image1.data() + i * x.GetPixelsNum(),
                   x.GetPixelsNum() * sizeof(uint16_t));
            pusher.SendData(pusher_buff.data(), i, x.GetPixelsNum() * sizeof(uint16_t));
        }
        pusher.EndDataCollection();
    });

    std::vector<uint8_t> rcv_image;
    int64_t msg_size = puller.GetImage(rcv_image);
    while (msg_size > 0) {
        auto image_metadata = (const ImageMetadata *) rcv_image.data();
        if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size++;
        if (image_metadata->image_size != x.GetPixelsNum() * sizeof(uint16_t)) diff_size_header++;
        else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                        image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                        x.GetPixelsNum() * sizeof(uint16_t)) != 0)
            diff_content++;
        nimages++;
        msg_size = puller.GetImage(rcv_image);
    }

    sender_thread.join();

     REQUIRE_NOTHROW(pusher.Disconnect());
    REQUIRE(nimages == nframes);
    REQUIRE(diff_size == 0);
    REQUIRE(diff_content == 0);
    REQUIRE(diff_size_header == 0);
}


TEST_CASE("ZMQImageCommTest_2Writers","[ZeroMQ]") {
    const size_t nframes = 256;

    ZMQContext context;
    Logger logger("test");
    DiffractionExperiment x, x_out;
    x.Mode(DetectorMode::Raw).DataStreamModuleSize(1, {1});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes).ImagesPerFile(16);


    REQUIRE(x.GetImageNum() == nframes);

    JungfrauCalibration calib(x);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::vector<std::string> zmq_addr = {"inproc://#1", "inproc://#2"};

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    ZMQImagePuller puller_0(context, zmq_addr[0]);
    ZMQImagePuller puller_1(context, zmq_addr[1]);
    ZMQImagePusher pusher(context);

    size_t diff_size[2] = {0, 0};
    size_t diff_content[2] = {0, 0};
    size_t diff_split[2] = {0, 0};
    size_t nimages[2] = {0, 0};

    pusher.Connect(zmq_addr, x);

    std::thread sender_thread = std::thread([&] {
        std::vector<char> pusher_buff(x.GetMaxCompressedSize() + sizeof(ImageMetadata));
        for (int i = 0; i < nframes; i++) {
            memcpy(pusher_buff.data() + sizeof(ImageMetadata),
                   image1.data() + i * x.GetPixelsNum(),
                   x.GetPixelsNum() * sizeof(uint16_t));
            pusher.SendData(pusher_buff.data(), i, x.GetPixelsNum() * sizeof(uint16_t));
        }
        pusher.EndDataCollection();
    });

    std::thread puller_0_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_0.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 2 != 0) diff_split[0]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[0]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[0]++;
            msg_size = puller_0.GetImage(rcv_image);
            nimages[0]++;
        }
    });


    std::thread puller_1_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_1.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 2 != 1) diff_split[1]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[1]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[1]++;
            msg_size = puller_1.GetImage(rcv_image);
            nimages[1]++;
        }
    });

    puller_0_thread.join();
    puller_1_thread.join();

    sender_thread.join();

    REQUIRE_NOTHROW(pusher.Disconnect());
    REQUIRE(nimages[0] == nframes / 2);
    REQUIRE(nimages[1] == nframes / 2);
    REQUIRE(diff_size[0] == 0);
    REQUIRE(diff_content[0] == 0);

    REQUIRE(diff_size[1] == 0);
    REQUIRE(diff_content[1] == 0);

    REQUIRE(diff_split[0] == 0);
    REQUIRE(diff_split[1] == 0);
}

TEST_CASE("ZMQImageCommTest_4Writers","[ZeroMQ]") {
    const size_t nframes = 255;

    ZMQContext context;
    Logger logger("test");
    DiffractionExperiment x, x_out;
    x.Mode(DetectorMode::Raw).DataStreamModuleSize(1, {1});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes).ImagesPerFile(16);


    REQUIRE(x.GetImageNum() == nframes);

    JungfrauCalibration calib(x);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::vector<std::string> zmq_addr = {"inproc://#1", "inproc://#2", "inproc://#3", "inproc://#4"};

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    ZMQImagePuller puller_0(context, zmq_addr[0]);
    ZMQImagePuller puller_1(context, zmq_addr[1]);
    ZMQImagePuller puller_2(context, zmq_addr[2]);
    ZMQImagePuller puller_3(context, zmq_addr[3]);
    ZMQImagePusher pusher(context);

    size_t diff_size[4] = {0, 0, 0, 0};
    size_t diff_content[4] = {0, 0, 0, 0};
    size_t diff_split[4] = {0, 0, 0, 0};
    size_t nimages[4] = {0, 0, 0, 0};

    pusher.Connect(zmq_addr, x);

    std::thread sender_thread = std::thread([&] {
        std::vector<char> pusher_buff(x.GetMaxCompressedSize() + sizeof(ImageMetadata));
        for (int i = 0; i < nframes; i++) {
            memcpy(pusher_buff.data() + sizeof(ImageMetadata),
                   image1.data() + i * x.GetPixelsNum(),
                   x.GetPixelsNum() * sizeof(uint16_t));
            pusher.SendData(pusher_buff.data(), i, x.GetPixelsNum() * sizeof(uint16_t));
        }
        pusher.EndDataCollection();
    });

    std::thread puller_0_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_0.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 4 != 0) diff_split[0]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[0]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[0]++;
            msg_size = puller_0.GetImage(rcv_image);
            nimages[0]++;
        }
    });

    std::thread puller_1_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_1.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 4 != 1) diff_split[1]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[1]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[1]++;
            msg_size = puller_1.GetImage(rcv_image);
            nimages[1]++;
        }
    });

    std::thread puller_2_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_2.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 4 != 2) diff_split[2]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[2]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[2]++;
            msg_size = puller_2.GetImage(rcv_image);
            nimages[2]++;
        }
    });

    std::thread puller_3_thread = std::thread([&] {
        std::vector<uint8_t> rcv_image;
        int64_t msg_size = puller_3.GetImage(rcv_image);
        while (msg_size > 0) {
            auto image_metadata = (const ImageMetadata *) rcv_image.data();
            if ((image_metadata->frameid / x.GetImagesPerFile()) % 4 != 3) diff_split[3]++;
            if (msg_size != x.GetPixelsNum() * sizeof(uint16_t) + sizeof(ImageMetadata)) diff_size[3]++;
            else if (memcmp(rcv_image.data() + sizeof(ImageMetadata),
                            image1.data() + image_metadata->frameid * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[3]++;
            msg_size = puller_3.GetImage(rcv_image);
            nimages[3]++;
        }
    });

    puller_0_thread.join();
    puller_1_thread.join();
    puller_2_thread.join();
    puller_3_thread.join();
    sender_thread.join();

    REQUIRE_NOTHROW(pusher.Disconnect());
    REQUIRE(nimages[0] == 64);
    REQUIRE(nimages[1] == 64);
    REQUIRE(nimages[2] == 64);
    REQUIRE(nimages[3] == 63);

    REQUIRE(diff_size[0] == 0);
    REQUIRE(diff_content[0] == 0);

    REQUIRE(diff_size[1] == 0);
    REQUIRE(diff_content[1] == 0);

    REQUIRE(diff_size[2] == 0);
    REQUIRE(diff_content[2] == 0);

    REQUIRE(diff_size[3] == 0);
    REQUIRE(diff_content[3] == 0);

    REQUIRE(diff_split[0] == 0);
    REQUIRE(diff_split[1] == 0);
    REQUIRE(diff_split[2] == 0);
    REQUIRE(diff_split[3] == 0);
}

TEST_CASE("ZMQImagePuller_abort","[ZeroMQ]") {
    const size_t nframes = 256;
    std::string zmq_addr = "inproc://#1";
    DiffractionExperiment x, x_out;
    x.Mode(DetectorMode::Raw).DataStreamModuleSize(1, {1});
    x.PedestalG0Frames(0).NumTriggers(0).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes);

    ZMQContext context;
    ZMQImagePuller puller(context, zmq_addr);

    std::thread puller_thread([&] {
        std::vector<uint8_t> tmp;
        puller.GetImage(tmp);
    });
    puller.Abort();
    puller_thread.join();
}