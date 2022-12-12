// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../grpc/gRPCServer_Template.h"
#include "../broker/JFJochStateMachine.h"
#include "../writer/JFJochWriterService.h"
#include "../fpga/receiver/JFJochReceiverService.h"
#include "FPGAUnitTest.h"
#include "../fpga/host/MockAcquisitionDevice.h"
#include "../indexing/JFJochIndexerService.h"
#include "../common/ZMQImagePusher.h"
#include "../common/jsonToGrpc.h"

using namespace std::literals::chrono_literals;

TEST_CASE("JFJochIntegrationTest_ZMQ", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ");
    ZMQContext zmq_context;

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    REQUIRE(!state_machine.GetMeasurementStatistics().has_value());


    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain(gain_from_file, m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);

    JFJochWriterService writer(zmq_context, logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_images_per_trigger(5);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(2);
    setup.set_summation(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    status = state_machine.GetStatus();
    REQUIRE(status.progress() == Approx(0.0));
    REQUIRE(status.broker_state() == JFJochProtoBuf::DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++) {
            for (int image_num = 1; image_num <= nimages; image_num++)
                aq_devices[i]->AddModule(image_num, m, image.data());
        }
        aq_devices[i]->Terminate();
    }

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    status = state_machine.GetStatus();
    REQUIRE(status.broker_state() == JFJochProtoBuf::IDLE);

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 5);
    REQUIRE(statistics.images_written() == 5);
    REQUIRE(statistics.max_image_number_sent() == 4);
    REQUIRE(!statistics.cancelled());
    REQUIRE(statistics.file_prefix() == "integration_test");
    REQUIRE(statistics.detector_width() == 2068);
    REQUIRE(statistics.detector_height() == 2164);
    REQUIRE(statistics.detector_pixel_depth() == 2);

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

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);

    REQUIRE(!state_machine.GetMeasurementStatistics().has_value());

    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().Mode(DetectorMode::Conversion);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain(gain_from_file, m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    JFJochProtoBuf::DetectorSettings detector_settings;
    detector_settings.set_frame_time_us(500);
    detector_settings.set_collect_raw_data(true);
    REQUIRE_NOTHROW(state_machine.SetDetectorSettings(detector_settings));
    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_raw_test");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    status = state_machine.GetStatus();
    REQUIRE(status.progress() == Approx(0.0));
    REQUIRE(status.broker_state() == JFJochProtoBuf::DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++) {
            for (int image_num = 1; image_num <= nimages; image_num++)
                aq_devices[i]->AddModule(image_num, m, image.data());
        }
        aq_devices[i]->Terminate();
    }

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    status = state_machine.GetStatus();
    REQUIRE(status.broker_state() == JFJochProtoBuf::IDLE);

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 5);
    REQUIRE(statistics.images_written() == 5);
    REQUIRE(statistics.file_prefix() == "integration_raw_test");
    REQUIRE(statistics.detector_width() == 1024);
    REQUIRE(statistics.detector_height() == 8*512);
    REQUIRE(statistics.detector_pixel_depth() == 2);

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

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    services
            .Writer("unix:writer_test_0", "inproc://#0")
            .Writer("unix:writer_test_1", "inproc://#1")
            .Writer("unix:writer_test_2", "inproc://#2")
            .Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);

        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++)
            test->LoadModuleGain(gain_from_file, m);

        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#0", "inproc://#1", "inproc://#2"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);

    JFJochWriterService writer_0(zmq_context, logger);
    JFJochWriterService writer_1(zmq_context, logger);
    JFJochWriterService writer_2(zmq_context, logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server_0 = gRPCServer("unix:writer_test_0",  writer_0);
    auto writer_server_1 = gRPCServer("unix:writer_test_1",  writer_1);
    auto writer_server_2 = gRPCServer("unix:writer_test_2",  writer_2);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test_3writers");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_data_file_count(5);
    setup.set_summation(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    JFJochProtoBuf::BrokerStatus status;
    status = state_machine.GetStatus();
    REQUIRE(status.progress() == Approx(0.0));
    REQUIRE(status.broker_state() == JFJochProtoBuf::DATA_COLLECTION);

    for (int i = 0; i < ndatastream; i++) {
        for (int image_num = 1; image_num <= nimages; image_num++) {
            for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++)
                aq_devices[i]->AddModule(image_num, m, image.data());
        }
        aq_devices[i]->Terminate();
    }

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    status = state_machine.GetStatus();
    REQUIRE(status.broker_state() == JFJochProtoBuf::IDLE);

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == nimages);
    REQUIRE(statistics.images_written() == nimages);

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

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<MockAcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        test->EnableLogging(&logger);
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain(gain_from_file, m);
            for (int frame = 1; frame <= nimages; frame ++)
                test->AddModule(frame, m, image.data());
        }
        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test");
    setup.set_images_per_trigger(2 * nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_summation(1);
    setup.set_data_file_count(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    logger.Info("Cancelling");
    REQUIRE_NOTHROW(state_machine.Cancel());
    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 0.5);
    REQUIRE(statistics.images_collected() == 5);
    REQUIRE(statistics.has_images_written());
    REQUIRE(statistics.images_written() == 5);
    REQUIRE(statistics.max_image_number_sent() == 4);
    REQUIRE(statistics.cancelled());
    REQUIRE(!statistics.has_indexing_rate());

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_with_preview", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_with_preview");

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).PreviewPeriod(5ms);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain(gain_from_file, m);
            for (int image_num = 1; image_num <= nimages; image_num++)
                test->AddModule(image_num, m, image.data());
        }
        test->Terminate();
        aq_devices.emplace_back(test);
    }
    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    ZMQPreviewPublisher preview(zmq_context, "inproc://#2");
    fpga_receiver.PreviewPublisher(&preview);

    ZMQSocket rcv_preview_socket(zmq_context, ZMQSocketType::Sub);
    REQUIRE_NOTHROW(rcv_preview_socket.Connect("inproc://#2"));
    rcv_preview_socket.SubscribeAll();

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_file_prefix("integration_test_preview");
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(123.0);
    setup.set_summation(1);
    setup.set_data_file_count(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    std::string s;

    // Pixel mask
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);
    JFJochProtoBuf::PreviewFrame frame;
    REQUIRE_NOTHROW(frame = jsonToGrpc<JFJochProtoBuf::PreviewFrame>(s));

    // Check header
    REQUIRE(frame.image_number() == -1);
    REQUIRE(frame.width() == state_machine.NotThreadSafe_Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == state_machine.NotThreadSafe_Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 4);

    // First frame
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);

    REQUIRE_NOTHROW(frame = jsonToGrpc<JFJochProtoBuf::PreviewFrame>(s));

    // Check header
    REQUIRE(frame.image_number() == 0);
    REQUIRE(frame.beam_x_pxl() == Approx(setup.beam_x_pxl()));
    REQUIRE(frame.beam_y_pxl() == Approx(setup.beam_y_pxl()));
    REQUIRE(frame.width() == state_machine.NotThreadSafe_Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == state_machine.NotThreadSafe_Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 2);

    // Check compressed image
    size_t npixel = state_machine.NotThreadSafe_Experiment().GetPixelsNum();

    std::vector<char> rcv_image(frame.data().size());
    rcv_image = {frame.data().begin(), frame.data().end()};
    REQUIRE(rcv_image.size() == state_machine.NotThreadSafe_Experiment().GetPixelsNum() * sizeof(int16_t));

    // Check no more frames waiting
    REQUIRE(rcv_preview_socket.Receive(s, false) == -1);

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 5);
    REQUIRE(statistics.images_written() == 5);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}


TEST_CASE("JFJochIntegrationTest_ZMQ_with_preview_no_writer", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_with_preview_no_writer");

    RegisterHDF5Filter();

    int64_t nimages = 5;
    int64_t ndatastream = 2;
    std::vector<int64_t> nmodules = {4, 4};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).PreviewPeriod(5ms);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < ndatastream; i++) {
        auto *test = new MockAcquisitionDevice(i , 256);
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(i); m++) {
            test->LoadModuleGain(gain_from_file, m);
            for (int image_num = 1; image_num <= nimages; image_num++)
                test->AddModule(image_num, m, image.data());
        }
        test->Terminate();
        aq_devices.emplace_back(test);
    }
    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    ZMQPreviewPublisher preview(zmq_context, "inproc://#2");
    fpga_receiver.PreviewPublisher(&preview);

    ZMQSocket rcv_preview_socket(zmq_context, ZMQSocketType::Sub);
    REQUIRE_NOTHROW(rcv_preview_socket.Connect("inproc://#2"));
    rcv_preview_socket.SubscribeAll();

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(100);
    setup.set_images_per_trigger(5);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(123.0);
    setup.set_beam_y_pxl(878.0);
    setup.set_summation(1);
    setup.set_data_file_count(1);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    std::string s;

    // Pixel mask
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);
    JFJochProtoBuf::PreviewFrame frame;
    REQUIRE_NOTHROW(frame = jsonToGrpc<JFJochProtoBuf::PreviewFrame>(s));

    // Check header
    REQUIRE(frame.image_number() == -1);
    REQUIRE(frame.width() == state_machine.NotThreadSafe_Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == state_machine.NotThreadSafe_Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 4);

    // First frame
    REQUIRE(rcv_preview_socket.Receive(s, false) > 0);

    REQUIRE_NOTHROW(frame = jsonToGrpc<JFJochProtoBuf::PreviewFrame>(s));

    // Check header
    REQUIRE(frame.image_number() == 0);
    REQUIRE(frame.beam_x_pxl() == Approx(setup.beam_x_pxl()));
    REQUIRE(frame.beam_y_pxl() == Approx(setup.beam_y_pxl()));
    REQUIRE(frame.width() == state_machine.NotThreadSafe_Experiment().GetXPixelsNum());
    REQUIRE(frame.height() == state_machine.NotThreadSafe_Experiment().GetYPixelsNum());
    REQUIRE(frame.pixel_depth() == 2);

    // Check compressed image
    size_t npixel = state_machine.NotThreadSafe_Experiment().GetPixelsNum();

    std::vector<char> rcv_image(frame.data().size());
    rcv_image = {frame.data().begin(), frame.data().end()};
    REQUIRE(rcv_image.size() == state_machine.NotThreadSafe_Experiment().GetPixelsNum() * sizeof(int16_t));

    // Check no more frames waiting
    REQUIRE(rcv_preview_socket.Receive(s, false) == -1);

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 5);
    REQUIRE(statistics.images_written() == 0);
    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_background_estimation", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_background_estimation");

    RegisterHDF5Filter();

    size_t nimages = 10;
    int64_t ndatastream = 1;
    std::vector<int64_t> nmodules = {8};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0)
            .MaskChipEdges(true).MaskModuleEdges(true).BackgroundEstimationPeriod(1ms).FrameTime(1ms);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image_raw_geom(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 256);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(); m++)
        test->LoadModuleGain(gain_from_file, m);

    for (int i = 0; i < nimages; i++) {
        for (auto & j: image_raw_geom)
            j = 2 * i + 5;
        for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(); m++)
            test->AddModule(i + 1, m, image_raw_geom.data());
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

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(75);
    setup.set_summation(1);
    setup.set_file_prefix("bkg_estimate_test");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(1090);
    setup.set_beam_y_pxl(1136);
    setup.mutable_unit_cell()->set_a(36.9);
    setup.mutable_unit_cell()->set_b(78.95);
    setup.mutable_unit_cell()->set_c(78.95);
    setup.mutable_unit_cell()->set_alpha(90.0);
    setup.mutable_unit_cell()->set_beta(90.0);
    setup.mutable_unit_cell()->set_gamma(90.0);
    setup.set_data_file_count(1);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);

    settings.set_bkg_estimate_low_q(2 * M_PI / 4.0);
    settings.set_bkg_estimate_high_q(2 * M_PI / 3.0);

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    JFJochProtoBuf::ReceiverDataProcessingPlots plots;
    REQUIRE(fpga_receiver.GetDataProcessingPlots(nullptr, nullptr, &plots).ok());

    REQUIRE(plots.bkg_estimate().x_size() == 1);
    REQUIRE(plots.bkg_estimate().y_size() == 1);
    REQUIRE(plots.bkg_estimate().x(0) == Approx(4.5));
    REQUIRE(plots.bkg_estimate().y(0) == Approx((5+23) / 2.0));

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 10);

    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}

TEST_CASE("JFJochIntegrationTest_ZMQ_lysozyme_spot", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index");

    RegisterHDF5Filter();

    size_t nimages = 1;
    int64_t ndatastream = 1;
    std::vector<int64_t> nmodules = {8};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test");

    logger.Verbose(true);

    std::vector<uint16_t> image(RAW_MODULE_SIZE);

    HDF5File data("../../tests/test_data/compression_benchmark.h5", false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    std::vector<hsize_t> file_size = {nimages, file_space.GetDimensions()[1], file_space.GetDimensions()[2]};
    dataset.ReadVector(image_conv, start, file_size);

    std::vector<int16_t> image_raw_geom(nimages * state_machine.NotThreadSafe_Experiment().GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++)
        ConvertedToRawGeometry(state_machine.NotThreadSafe_Experiment(),
                               image_raw_geom.data() + i * RAW_MODULE_SIZE * state_machine.NotThreadSafe_Experiment().GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);


    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 256);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(0); m++) {
        test->LoadModuleGain(gain_from_file, m);
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

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(75);
    setup.set_summation(1);
    setup.set_file_prefix("spot_finding_test");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(1090);
    setup.set_beam_y_pxl(1136);
    setup.mutable_unit_cell()->set_a(36.9);
    setup.mutable_unit_cell()->set_b(78.95);
    setup.mutable_unit_cell()->set_c(78.95);
    setup.mutable_unit_cell()->set_alpha(90.0);
    setup.mutable_unit_cell()->set_beta(90.0);
    setup.mutable_unit_cell()->set_gamma(90.0);
    setup.set_data_file_count(1);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);

    REQUIRE_NOTHROW(state_machine.SetDataProcessingSettings(settings));

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == 1);

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

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).SpotFindingPeriod(10ms);
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test")
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

    std::vector<int16_t> image_raw_geom(nimages * state_machine.NotThreadSafe_Experiment().GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++)
        ConvertedToRawGeometry(state_machine.NotThreadSafe_Experiment(),
                               image_raw_geom.data() + i * RAW_MODULE_SIZE * state_machine.NotThreadSafe_Experiment().GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);


    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 512);

    std::vector<uint16_t> pedestal(RAW_MODULE_SIZE, 3500);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(0); m++) {
        test->LoadModuleGain(gain_from_file, m);
        for (int image_num = 1; image_num <= nimages; image_num++)
            test->AddModule(image_num, m, (uint16_t *) (image_raw_geom.data()
                                                        + (m + (image_num-1) * state_machine.NotThreadSafe_Experiment().GetModulesNum(0))
                                                          * RAW_MODULE_SIZE));
    }
    test->Terminate();

    aq_devices.emplace_back(test);

    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQSpotPublisher spots(zmq_context, "inproc://#2");

    JFJochIndexerService indexer(zmq_context, logger);

    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);
    auto indexer_server = gRPCServer("unix:indexer_test", indexer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(75);
    setup.set_summation(1);
    setup.set_file_prefix("spot_finding_test");
    setup.set_images_per_trigger(nimages);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(1090);
    setup.set_beam_y_pxl(1136);
    setup.mutable_unit_cell()->set_a(36.9);
    setup.mutable_unit_cell()->set_b(78.95);
    setup.mutable_unit_cell()->set_c(78.95);
    setup.mutable_unit_cell()->set_alpha(90.0);
    setup.mutable_unit_cell()->set_beta(90.0);
    setup.mutable_unit_cell()->set_gamma(90.0);
    setup.set_data_file_count(1);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);

    REQUIRE_NOTHROW(state_machine.SetDataProcessingSettings(settings));

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.has_indexing_rate());
    REQUIRE(statistics.indexing_rate() == 1.0);
    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == nimages);

    JFJochProtoBuf::SpotFinderImageOutput spot_finder_out;
    std::string s;

    indexer_server->Shutdown();
    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}


TEST_CASE("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index_sum", "[JFJochReceiver]") {
    Logger logger("JFJochIntegrationTest_ZMQ_lysozyme_spot_and_index_sum");

    RegisterHDF5Filter();

    size_t nimages = 24;
    int64_t ndatastream = 1;
    std::vector<int64_t> nmodules = {8};

    JFJochServices services(logger);
    JFJochStateMachine state_machine(services);
    state_machine.NotThreadSafe_Experiment() = DiffractionExperiment(2, nmodules, 8, 36);
    state_machine.NotThreadSafe_Experiment().PedestalG0Frames(0).PedestalG1Frames(0).PedestalG2Frames(0).SpotFindingPeriod(std::chrono::milliseconds(10));
    services.Writer("unix:writer_test", "inproc://#1").Receiver("unix:fpga_receiver_test")
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

    std::vector<int16_t> image_raw_geom(nimages * state_machine.NotThreadSafe_Experiment().GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++)
        ConvertedToRawGeometry(state_machine.NotThreadSafe_Experiment(),
                               image_raw_geom.data() + i * RAW_MODULE_SIZE * state_machine.NotThreadSafe_Experiment().GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);


    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;

    auto *test = new MockAcquisitionDevice(0 , 512);

    std::vector<uint16_t> pedestal(RAW_MODULE_SIZE, 3500);

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int m = 0; m < state_machine.NotThreadSafe_Experiment().GetModulesNum(0); m++) {
        test->LoadModuleGain(gain_from_file, m);
        for (int image_num = 1; image_num <= nimages; image_num++)
            test->AddModule(image_num, m, (uint16_t *) (image_raw_geom.data()
                                                        + (m + (image_num-1) * state_machine.NotThreadSafe_Experiment().GetModulesNum(0))
                                                          * RAW_MODULE_SIZE));
    }
    test->Terminate();

    aq_devices.emplace_back(test);

    ZMQContext zmq_context;

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQSpotPublisher spots(zmq_context, "inproc://#2");

    JFJochIndexerService indexer(zmq_context, logger);
    ZMQImagePusher pusher(zmq_context, {"inproc://#1"});
    JFJochReceiverService fpga_receiver(tmp_devices, logger, pusher);
    JFJochWriterService writer(zmq_context, logger);

    fpga_receiver.SpotPublisher(&spots);

    auto fpga_receiver_server = gRPCServer("unix:fpga_receiver_test", fpga_receiver);
    auto writer_server = gRPCServer("unix:writer_test",  writer);
    auto indexer_server = gRPCServer("unix:indexer_test", indexer);

    REQUIRE_NOTHROW(state_machine.Initialize());
    logger.Info("Initialized");

    JFJochProtoBuf::DatasetSettings setup;
    setup.set_ntrigger(1);
    setup.set_detector_distance_mm(75);
    setup.set_summation(3);
    setup.set_file_prefix("spot_finding_test");
    setup.set_images_per_trigger(nimages/3);
    setup.set_photon_energy_kev(12.4);
    setup.set_beam_x_pxl(1090);
    setup.set_beam_y_pxl(1136);
    setup.mutable_unit_cell()->set_a(36.9);
    setup.mutable_unit_cell()->set_b(78.95);
    setup.mutable_unit_cell()->set_c(78.95);
    setup.mutable_unit_cell()->set_alpha(90.0);
    setup.mutable_unit_cell()->set_beta(90.0);
    setup.mutable_unit_cell()->set_gamma(90.0);
    setup.set_data_file_count(1);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_signal_to_noise_threshold(4);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.5);
    settings.set_local_bkg_size(5);

    REQUIRE_NOTHROW(state_machine.SetDataProcessingSettings(settings));

    REQUIRE_NOTHROW(state_machine.Start(setup));
    logger.Info("Started measurement");

    REQUIRE_NOTHROW(state_machine.Stop());
    logger.Info("Stopped measurement");

    auto tmp = state_machine.GetMeasurementStatistics();
    REQUIRE(tmp.has_value());
    auto statistics = tmp.value();

    REQUIRE(statistics.collection_efficiency() == 1.0);
    REQUIRE(statistics.images_collected() == nimages/3);
    REQUIRE(statistics.has_indexing_rate());
    REQUIRE(statistics.indexing_rate() == 1.0);

    JFJochProtoBuf::SpotFinderImageOutput spot_finder_out;
    std::string s;

    indexer_server->Shutdown();
    fpga_receiver_server->Shutdown();
    writer_server->Shutdown();
}
//#endif