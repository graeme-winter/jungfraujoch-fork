// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../writer/ZMQImagePuller.h"
#include "../common/ZMQImagePusher.h"

void test_puller(ZMQImagePuller *puller,
                 const DiffractionExperiment& x,
                 const std::vector<uint16_t> &image1,
                 int64_t nwriter,
                 int64_t writer_id,
                 std::vector<size_t> &diff_split,
                 std::vector<size_t> &diff_size,
                 std::vector<size_t> &diff_content,
                 std::vector<size_t> &nimages) {

    puller->WaitForImage();
    if (puller->GetFrameType() != JFJochFrameDeserializer::Type::START) {
        diff_content[writer_id]++;
        return;
    }
    puller->WaitForImage();
    while (puller->GetFrameType() != JFJochFrameDeserializer::Type::END) {
        if (puller->GetFrameType() == JFJochFrameDeserializer::Type::IMAGE) {

            if ((nwriter > 1) && (puller->GetImageNumber() % nwriter != writer_id))
                diff_split[writer_id]++;
            if (puller->GetImageSize() != x.GetPixelsNum() * sizeof(uint16_t))
                diff_size[writer_id]++;
            else if (memcmp(puller->GetImage(),image1.data() + puller->GetImageNumber() * x.GetPixelsNum(),
                            x.GetPixelsNum() * sizeof(uint16_t)) != 0)
                diff_content[writer_id]++;
            nimages[writer_id]++;
        }
        puller->WaitForImage();
    }
}

TEST_CASE("ZMQImageCommTest_1Writer","[ZeroMQ]") {
    const size_t nframes = 256;

    ZMQContext context;
    Logger logger("test");
    DiffractionExperiment x(1,{1});
    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
        .ImagesPerTrigger(nframes);

    std::vector<DiffractionSpot> empty_spot_vector;

    REQUIRE(x.GetImageNum() == nframes);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::string zmq_addr = "inproc://#1";

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    ZMQImagePuller puller(context);
    ZMQImagePusher pusher(context, {zmq_addr});

    std::vector<size_t> diff_size(1), diff_content(1), diff_split(1), nimages(1);

    puller.Connect(zmq_addr);

    std::thread sender_thread = std::thread([&] {
        pusher.StartDataCollection(16);
        for (int i = 0; i < nframes; i++)
            pusher.SendData(image1.data() + i * x.GetPixelsNum(), x.GetPixelsNum() * sizeof(uint16_t),
                            empty_spot_vector, i);
        pusher.EndDataCollection();
    });

    std::thread puller_thread(test_puller, &puller, std::cref(x), std::cref(image1), 1, 0,
                              std::ref(diff_split), std::ref(diff_size), std::ref(diff_content),
                              std::ref(nimages));

    sender_thread.join();
    puller_thread.join();

    puller.Disconnect();

    REQUIRE(nimages[0] == nframes);
    REQUIRE(diff_size[0] == 0);
    REQUIRE(diff_content[0] == 0);
}


TEST_CASE("ZMQImageCommTest_2Writers","[ZeroMQ]") {
    const size_t nframes = 256;

    ZMQContext context;
    Logger logger("test");
    DiffractionExperiment x(1, {1});
    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes);

    REQUIRE(x.GetImageNum() == nframes);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::vector<DiffractionSpot> empty_spot_vector;

    std::vector<std::string> zmq_addr;

    int64_t npullers = 2;

    for (int i = 0; i < npullers; i++)
        zmq_addr.push_back("inproc://#" + std::to_string(i));

    ZMQImagePusher pusher(context, zmq_addr);

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    std::vector<std::unique_ptr<ZMQImagePuller> > puller;
    for (int i = 0; i < npullers; i++) {
        puller.push_back(std::make_unique<ZMQImagePuller>(context));
        puller[i]->Connect(zmq_addr[i]);
    }

    std::vector<size_t> diff_size(npullers), diff_content(npullers), diff_split(npullers), nimages(npullers);

    std::thread sender_thread = std::thread([&] {
        pusher.StartDataCollection(16);
        for (int i = 0; i < nframes; i++)
            pusher.SendData(image1.data() + i * x.GetPixelsNum(), x.GetPixelsNum() * sizeof(uint16_t),
                            empty_spot_vector, i);
        pusher.EndDataCollection();
    });

    std::vector<std::thread> puller_threads;
    for (int i = 0; i < npullers; i++)
        puller_threads.emplace_back(test_puller, puller[i].get(), std::cref(x),
                                    std::cref(image1), npullers, i,
                                    std::ref(diff_split), std::ref(diff_size), std::ref(diff_content), std::ref(nimages));

    for (int i = 0; i < npullers; i++)
        puller_threads[i].join();

    sender_thread.join();

    REQUIRE_NOTHROW(puller[0]->Disconnect());
    REQUIRE_NOTHROW(puller[1]->Disconnect());

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
    DiffractionExperiment x(1, {1});
    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes);

    REQUIRE(x.GetImageNum() == nframes);

    std::mt19937 g1(1387);
    std::uniform_int_distribution<uint16_t> dist;

    std::vector<uint16_t> image1(x.GetPixelsNum()*nframes);
    for (auto &i: image1) i = dist(g1);

    std::vector<DiffractionSpot> empty_spot_vector;

    std::vector<std::string> zmq_addr;

    int64_t npullers = 4;


    for (int i = 0; i < npullers; i++)
        zmq_addr.push_back("inproc://#" + std::to_string(i));

    ZMQImagePusher pusher(context, zmq_addr);

    // Puller needs to be declared first, but both objects need to exist till communication finished
    // TODO: ImageSender should not allow if there are still completions to be done
    std::vector<std::unique_ptr<ZMQImagePuller> > puller;
    for (int i = 0; i < npullers; i++) {
        puller.push_back(std::make_unique<ZMQImagePuller>(context));
        puller[i]->Connect(zmq_addr[i]);
    }


    std::vector<size_t> diff_size(npullers), diff_content(npullers), diff_split(npullers), nimages(npullers);

    std::thread sender_thread = std::thread([&] {
        pusher.StartDataCollection(16);
        for (int i = 0; i < nframes; i++)
            pusher.SendData(image1.data() + i * x.GetPixelsNum(), x.GetPixelsNum() * sizeof(uint16_t),
                            empty_spot_vector, i);
        pusher.EndDataCollection();
    });

    std::vector<std::thread> puller_threads;
    for (int i = 0; i < npullers; i++)
        puller_threads.emplace_back(test_puller, puller[i].get(), std::cref(x),
                                    std::cref(image1), npullers, i,
                                    std::ref(diff_split), std::ref(diff_size), std::ref(diff_content), std::ref(nimages));

    for (int i = 0; i < npullers; i++)
        puller_threads[i].join();

    sender_thread.join();

    REQUIRE_NOTHROW(puller[0]->Disconnect());
    REQUIRE_NOTHROW(puller[1]->Disconnect());
    REQUIRE_NOTHROW(puller[2]->Disconnect());
    REQUIRE_NOTHROW(puller[3]->Disconnect());

    REQUIRE(nimages[0] == 64);
    REQUIRE(nimages[1] == 64);
    REQUIRE(nimages[2] == 64);
    REQUIRE(nimages[3] == 63);

    for (int i = 0; i < npullers; i++) {
        REQUIRE(diff_size[i] == 0);
        REQUIRE(diff_content[i] == 0);
        REQUIRE(diff_split[i] == 0);
    }
}

TEST_CASE("ZMQImagePuller_abort","[ZeroMQ]") {
    const size_t nframes = 256;
    std::string zmq_addr = "inproc://#1";
    DiffractionExperiment x, x_out(1, {1});
    x.Mode(DetectorMode::Raw);
    x.PedestalG0Frames(0).NumTriggers(1).UseInternalPacketGenerator(false).PhotonEnergy_keV(12.4)
            .ImagesPerTrigger(nframes);

    ZMQContext context;
    ZMQImagePuller puller(context);

    std::vector<size_t> diff_size(1), diff_content(1), diff_split(1), nimages(1);
    std::vector<uint16_t> image1(x.GetPixelsNum());

    std::thread puller_thread(test_puller, &puller, std::cref(x), std::cref(image1), 1, 0,
                              std::ref(diff_split), std::ref(diff_size), std::ref(diff_content),
                              std::ref(nimages));
    puller.Abort();
    puller_thread.join();
    REQUIRE(nimages[0] == 0);
}