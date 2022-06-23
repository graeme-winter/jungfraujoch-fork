// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../grpc/gRPCServer_Template.h"
#include "../broker/JFJochBrokerService.h"
#include "../writer/JFJochWriterService.h"
#include "../fpga/host/JFJochReceiverService.h"
#include "FPGAUnitTest.h"
#include "../fpga/host/MockAcquisitionDevice.h"
#include "../indexing/JFJochIndexerService.h"

TEST_CASE("JFJochIntegrationTest_ZMQ", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ");
    ZMQContext zmq_context;

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices, zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(broker.Initialize(nullptr, nullptr, nullptr));
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(0.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++) {
            for (int image_num = 1; image_num <= nimages; image_num++)
                aq_devices[i]->AddModule(image_num, m, image.data());
            aq_devices[i]->AddModule(PEDESTAL_FRAME_ID, m, image.data());
        }
        aq_devices[i]->Terminate();
    }

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverStatus progress;
    REQUIRE(fpga_receiver.GetStatus(nullptr, nullptr, &progress).ok());
    REQUIRE(progress.progress() == Approx(100.0));

    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(100.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_IDLE);
    REQUIRE(status.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(status.last_measurement_images_collected() == 5);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_RAW", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ");
    ZMQContext zmq_context;

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).Mode(DetectorMode::Conversion);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices, zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(broker.Initialize(nullptr, nullptr, nullptr));
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("integration_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_collection_mode("raw");

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(0.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++) {
            for (int image_num = 1; image_num <= nimages; image_num++)
                aq_devices[i]->AddModule(image_num, m, image.data());
            aq_devices[i]->AddModule(PEDESTAL_FRAME_ID, m, image.data());
        }
        aq_devices[i]->Terminate();
    }

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverStatus progress;
    REQUIRE(fpga_receiver.GetStatus(nullptr, nullptr, &progress).ok());
    REQUIRE(progress.progress() == Approx(100.0));

    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(100.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_IDLE);
    REQUIRE(status.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(status.last_measurement_images_collected() == 5);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_3Writers", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_3Writers");
    ZMQContext zmq_context;

    RegisterHDF5Filter();

    int64_t nimages = 37;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).ImagesPerFile(8);
    broker.Services()
            .Writer("unix:writer_test_0", "inproc://#0")
            .Writer("unix:writer_test_1", "inproc://#1")
            .Writer("unix:writer_test_2", "inproc://#2")
            .Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices, zmq_context, logger);

    JFJochWriterService writer_0(zmq_context, "inproc://#0", logger);
    JFJochWriterService writer_1(zmq_context, "inproc://#1", logger);
    JFJochWriterService writer_2(zmq_context, "inproc://#2", logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server_0 = gRPCServer("unix:writer_test_0",  writer_0);
    auto writer_server_1 = gRPCServer("unix:writer_test_1",  writer_1);
    auto writer_server_2 = gRPCServer("unix:writer_test_2",  writer_2);

    REQUIRE_NOTHROW(broker.Initialize(nullptr, nullptr, nullptr));
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("integration_test_3writers");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(0.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int image_num = 1; image_num <= nimages; image_num++) {
            for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++)
                aq_devices[i]->AddModule(image_num, m, image.data());
        }
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++)
            aq_devices[i]->AddModule(PEDESTAL_FRAME_ID, m, image.data());

        aq_devices[i]->Terminate();
    }

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverStatus progress;
    REQUIRE(fpga_receiver.GetStatus(nullptr, nullptr, &progress).ok());
    REQUIRE(progress.progress() == Approx(100.0));

    broker.GetStatus(nullptr, nullptr, &status);
    REQUIRE(status.receiver_status().progress() == Approx(100.0));
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus_State_IDLE);
    REQUIRE(status.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(status.last_measurement_images_collected() == nimages);

    // REQUIRE(imagePuller_0.GetImageCount() == 16);
    // REQUIRE(imagePuller_1.GetImageCount() == 13);
    // REQUIRE(imagePuller_2.GetImageCount() == 8);
    fpga_receiver_server->Shutdown();
    writer_server_0->Shutdown();
    writer_server_1->Shutdown();
    writer_server_2->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_Cancel", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_Cancel");

    ZMQContext zmq_context;

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        test->EnableLogging(&logger);
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);
            test->AddModule(1, m, image.data());
            test->AddModule(2, m, image.data());
            test->AddModule(3, m, image.data());
            test->AddModule(4, m, image.data());
            test->AddModule(7, m, image.data());
            test->AddModule(PEDESTAL_FRAME_ID, m, image.data());
        }
        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices,zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("integration_test");
    setup.set_images_per_trigger(2 * nimages);
    setup.set_photon_energy_kev(12.4);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    logger.Info("Cancelling");
    REQUIRE(broker.Cancel(nullptr, nullptr, nullptr).ok());
    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::BrokerStatus statistics;
    REQUIRE(broker.GetStatus(nullptr, nullptr, &statistics).ok());

    REQUIRE(statistics.last_measurement_collection_efficiency() == 0.5);
    REQUIRE(statistics.last_measurement_images_collected() == 5);

    JFJochProtoBuf::JFJochReceiverOutput output;
    REQUIRE(broker.GetDetailedReceiverOutput(nullptr, nullptr, &output).ok());

    REQUIRE(output.max_image_number_sent() == 6);
    REQUIRE(output.images_sent() == 5);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_with_preview", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_with_preview");

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);
            for (int image_num = 1; image_num <= nimages; image_num++)
                test->AddModule(image_num, m, image.data());
            test->AddModule(PEDESTAL_FRAME_ID, m, image.data());
        }
        test->Terminate();
        aq_devices.emplace_back(test);
    }
    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices,zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    ZMQPreviewPublisher preview(zmq_context, "inproc://#2");
    fpga_receiver.PreviewPublisher(&preview);

    ZMQSocket rcv_preview_socket(zmq_context, ZMQSocketType::Sub);
    REQUIRE_NOTHROW(rcv_preview_socket.Connect("inproc://#2"));
    rcv_preview_socket.SubscribeAll();

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("integration_test_preview");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_center_x_pxl(123.0);
    setup.set_preview_rate_ms(5);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    std::string s;

    // Pixel mask
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);
    JFJochProtoBuf::PreviewFrame frame;
    REQUIRE_NOTHROW(frame.ParseFromString(s));

    // Check header
    REQUIRE(frame.image_number() == -1);
    REQUIRE(frame.width() == broker.Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == broker.Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 4);

    // First frame
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);

    REQUIRE_NOTHROW(frame.ParseFromString(s));

    // Check header
    REQUIRE(frame.image_number() == 0);
    REQUIRE(frame.beam_center_x() == Approx(setup.beam_center_x_pxl()));
    REQUIRE(frame.beam_center_y() == Approx(setup.beam_center_y_pxl()));
    REQUIRE(frame.width() == broker.Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == broker.Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 2);

    // Check compressed image
    size_t npixel = broker.Experiment().GetPixelsNum();

    std::vector<char> rcv_image(frame.data().size());
    rcv_image = {frame.data().begin(), frame.data().end()};
    REQUIRE(rcv_image.size() == broker.Experiment().GetPixelsNum() * sizeof(int16_t));

    // Check no more frames waiting
    REQUIRE(rcv_preview_socket.Receive(s, false) == -1);

    JFJochProtoBuf::BrokerStatus statistics;
    broker.GetStatus(nullptr, nullptr, &statistics);

    REQUIRE(statistics.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(statistics.last_measurement_images_collected() == 5);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}


TEST_CASE("JFJochIntegrationTest_ZMQ_with_preview_different_rate", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_with_preview");

    RegisterHDF5Filter();

    int64_t nimages = 10;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        for (int m = 0; m < broker.Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);
            for (int image_num = 1; image_num <= nimages; image_num++)
                test->AddModule(image_num, m, image.data());
            test->AddModule(PEDESTAL_FRAME_ID, m, image.data());
        }
        test->Terminate();
        aq_devices.emplace_back(test);
    }
    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver(tmp_devices,zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    ZMQPreviewPublisher preview(zmq_context, "inproc://#2");
    fpga_receiver.PreviewPublisher(&preview);

    ZMQSocket rcv_preview_socket(zmq_context, ZMQSocketType::Sub);
    REQUIRE_NOTHROW(rcv_preview_socket.Connect("inproc://#2"));
    rcv_preview_socket.SubscribeAll();

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(100);
    setup.set_image_time_us(5000);
    setup.set_name_pattern("integration_test");
    setup.set_images_per_trigger(2);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_center_x_pxl(123.0);
    setup.set_preview_rate_ms(50);
    setup.set_frame_summation(false);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    std::string s;

    // Pixel mask
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);
    JFJochProtoBuf::PreviewFrame frame;
    REQUIRE_NOTHROW(frame.ParseFromString(s));

    // Check header
    REQUIRE(frame.image_number() == -1);
    REQUIRE(frame.width() == broker.Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == broker.Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 4);

    // First frame
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);
    frame.ParseFromString(s);

    // Check header
    REQUIRE(frame.image_number() == 0);
    REQUIRE(frame.beam_center_x() == Approx(setup.beam_center_x_pxl()));
    REQUIRE(frame.beam_center_y() == Approx(setup.beam_center_y_pxl()));
    REQUIRE(frame.width() == broker.Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == broker.Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 2);

    std::vector<char> rcv_image(frame.data().size());
    rcv_image = {frame.data().begin(), frame.data().end()};
    REQUIRE(rcv_image.size() == broker.Experiment().GetPixelsNum() * sizeof(int16_t));

    // Check no more frames waiting
    REQUIRE(rcv_preview_socket.Receive(s, false) == -1);

    logger.Info("Preview image received");


    JFJochProtoBuf::PreviewFrame frame_via_broker;
    broker.GetPreviewFrame(nullptr, nullptr, &frame_via_broker);
    REQUIRE(frame_via_broker.image_number() == 0);
    REQUIRE(frame_via_broker.beam_center_x() == Approx(setup.beam_center_x_pxl()));
    REQUIRE(frame_via_broker.beam_center_y() == Approx(setup.beam_center_y_pxl()));
    REQUIRE(frame_via_broker.width() == broker.Experiment().GetXPixelsNum());
    REQUIRE(frame_via_broker.height() == broker.Experiment().GetYPixelsNum());
    REQUIRE(frame_via_broker.pixel_depth() == 2);

    JFJochProtoBuf::BrokerStatus statistics;
    broker.GetStatus(nullptr, nullptr, &statistics);

    REQUIRE(statistics.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(statistics.last_measurement_images_collected() == 10);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();

}

TEST_CASE("JFJochIntegrationTest_ZMQ_lysozyme_spot", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index");

    RegisterHDF5Filter();

    size_t nimages = 1;
    int64_t ndatastream = 1;
    std::vector<int64_t> nmodules = {8};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    HDF5File data("../../tests/test_data/compression_benchmark.h5", false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    std::vector<hsize_t> file_size = {nimages, file_space.GetDimensions()[1], file_space.GetDimensions()[2]};
    dataset.ReadVector(image_conv, start, file_size);

    std::vector<int16_t> image_raw_geom(nimages * broker.Experiment().GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++)
        ConvertedToRawGeometry(broker.Experiment(),
                               image_raw_geom.data() + i * RAW_MODULE_SIZE * broker.Experiment().GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);


    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 256);

    for (int m = 0; m < broker.Experiment().GetModulesNum(0); m++) {
        test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);
        test->AddModule(1, m, (uint16_t *) (image_raw_geom.data() + m * RAW_MODULE_SIZE));
    }
    test->Terminate();

    aq_devices.emplace_back(test);

    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQSpotPublisher spots(zmq_context, "inproc://#2");

    ZMQSocket rcv_spots_socket(zmq_context, ZMQSocketType::Sub);
    REQUIRE_NOTHROW(rcv_spots_socket.Connect("inproc://#2"));
    rcv_spots_socket.SubscribeAll();

    JFJochReceiverService fpga_receiver(tmp_devices,zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(75);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("spot_finding_test");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_center_x_pxl(1090);
    setup.set_beam_center_y_pxl(1136);
    *setup.mutable_unit_cell() = UnitCell(36.9, 78.95, 78.95, 90, 90, 90);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_enable_3d_spot_finding(false);
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);
    settings.set_max_spots(30);

    broker.SetDataProcessingSettings(nullptr, &settings, nullptr);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverStatus progress;
    REQUIRE(fpga_receiver.GetStatus(nullptr, nullptr, &progress).ok());
    REQUIRE(progress.progress() == Approx(100.0));


    JFJochProtoBuf::BrokerStatus statistics;
    broker.GetStatus(nullptr, nullptr, &statistics);

    REQUIRE(statistics.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(statistics.last_measurement_images_collected() == 1);

    JFJochProtoBuf::SpotFinderImageOutput spot_finder_out;
    std::string s;
    REQUIRE(rcv_spots_socket.Receive(s, false) > 0);
    REQUIRE_NOTHROW(spot_finder_out.ParseFromString(s));
    REQUIRE(spot_finder_out.image_number() == 0);

    double ret = 0.0;
    for (const auto &iter: spot_finder_out.coord()) {
        ret += cos(2*M_PI*(Coord(iter) * Coord(39.431274, 25.273994, 63.584850))); //a_star
        ret += cos(2*M_PI*(Coord(iter) * Coord(28.513466,60.642326,-41.786633)));  //b_star
        ret += cos(2*M_PI*(Coord(iter) * Coord(-29.096050, 20.499268, 9.895372))); //c_star
    }
    REQUIRE(spot_finder_out.coord_size() <= 30);
    REQUIRE(ret > 3.0 * 0.75 * spot_finder_out.coord_size()); // expect score to be perfect for 75% spots
    REQUIRE(rcv_spots_socket.Receive(s, false) == -1);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index");

    RegisterHDF5Filter();

    size_t nimages = 24;
    int64_t ndatastream = 1;
    std::vector<int64_t> nmodules = {8};

    JFJochBrokerService broker(logger);
    broker.Experiment().DataStreamModuleSize(2, nmodules, 8, 36)
            .PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    broker.Services().Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test")
            .Indexer("unix:indexer_test","inproc://#2");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    HDF5File data("../../tests/test_data/compression_benchmark.h5", false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    std::vector<hsize_t> file_size = {nimages, file_space.GetDimensions()[1], file_space.GetDimensions()[2]};
    dataset.ReadVector(image_conv, start, file_size);

    std::vector<int16_t> image_raw_geom(nimages * broker.Experiment().GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++)
        ConvertedToRawGeometry(broker.Experiment(),
                               image_raw_geom.data() + i * RAW_MODULE_SIZE * broker.Experiment().GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);


    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 512);

    std::vector<uint16_t> pedestal(RAW_MODULE_SIZE, 3500);

    for (int m = 0; m < broker.Experiment().GetModulesNum(0); m++) {
        test->LoadModuleGain("../../tests/test_data/gainMaps_M049.bin", m);
        for (int image_num = 1; image_num <= nimages; image_num++)
            test->AddModule(image_num, m, (uint16_t *) (image_raw_geom.data()
                                                        + (m + (image_num-1) * broker.Experiment().GetModulesNum(0))
                                                        * RAW_MODULE_SIZE));
        test->AddModule(PEDESTAL_FRAME_ID, m, pedestal.data());
    }
    test->Terminate();

    aq_devices.emplace_back(test);

    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQSpotPublisher spots(zmq_context, "inproc://#2");

    JFJochIndexerService indexer(zmq_context, logger);
    JFJochReceiverService fpga_receiver(tmp_devices,zmq_context, logger);
    JFJochWriterService writer(zmq_context, "inproc://#1", logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);
    auto indexer_server = gRPCServer("unix:indexer_test", indexer);

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    logger.Info("Initialized");

    JFJochProtoBuf::BrokerSetup setup;
    setup.set_ntrigger(0);
    setup.set_detector_distance_mm(75);
    setup.set_image_time_us(1000);
    setup.set_name_pattern("spot_finding_test");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_center_x_pxl(1090);
    setup.set_beam_center_y_pxl(1136);
    setup.set_spot_finding_rate_ms(10);
    *setup.mutable_unit_cell() = UnitCell(36.9, 78.95, 78.95, 90, 90, 90);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_enable_3d_spot_finding(false);
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);

    broker.SetDataProcessingSettings(nullptr, &settings, nullptr);

    REQUIRE(broker.Start(nullptr, &setup, nullptr).ok());
    logger.Info("Started measurement");

    REQUIRE(broker.Stop(nullptr, nullptr, nullptr).ok());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverStatus progress;
    REQUIRE(fpga_receiver.GetStatus(nullptr, nullptr, &progress).ok());
    REQUIRE(progress.progress() == Approx(100.0));

    JFJochProtoBuf::BrokerStatus statistics;
    broker.GetStatus(nullptr, nullptr, &statistics);

    CHECK(statistics.indexer_status().images_analyzed() == 3);
    CHECK(statistics.indexer_status().images_indexed() == 3);
    REQUIRE(statistics.last_measurement_collection_efficiency() == 1.0);
    REQUIRE(statistics.last_measurement_images_collected() == nimages);

    JFJochProtoBuf::SpotFinderImageOutput spot_finder_out;
    std::string s;

    indexer_server->Shutdown();
    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}
//#endif