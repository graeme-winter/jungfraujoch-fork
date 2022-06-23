// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../writer/JFJochWriterService.h"
#include "../common/ZMQImagePusher.h"
#include "../fpga/host/JFJochReceiverService.h"
#include "../fpga/host/HLSSimulatedDevice.h"


TEST_CASE("JFJochWriterTest_ZMQ","[JFJochWriter]") {
    RegisterHDF5Filter();

    Logger logger("test");
    ZMQContext context;
    std::string zmq_addr = "inproc://#1";

    grpc::ServerContext grpc_context;
    JFJochProtoBuf::Empty empty;

    DiffractionExperiment x;
    x.FilePrefix("JFJochWriterTest").NumTriggers(1).ImagesPerTrigger(5)
            .UseInternalPacketGenerator(true).DataStreamModuleSize(1, {2})
            .Mode(DetectorMode::Raw).PedestalG0Frames(0);

    // Create Receiver Service
    JungfrauCalibration calib(x);

    JFJochProtoBuf::JFJochReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    *receiver_input.mutable_calibration() = calib;
    receiver_input.add_writer_zmq_address(zmq_addr);

    std::vector<double> empty_gain(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        auto test = new HLSSimulatedDevice(i, 64);
        for (int m = 0; m < x.GetModulesNum(i); m++)
            test->LoadModuleGain(empty_gain, m);
        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver_service(tmp_devices, context, logger);

    JFJochProtoBuf::JFJochWriterInput writer_input;
    *writer_input.mutable_jungfraujoch_settings() = x;

    JFJochProtoBuf::JFJochReceiverOutput receiver_output;
    JFJochProtoBuf::JFJochWriterOutput writer_output;

    std::unique_ptr<JFJochWriter> writer;
    REQUIRE(x.GetImageNum() == 5);
    REQUIRE_NOTHROW(writer = std::make_unique<JFJochWriter>(x, context, zmq_addr, logger));

    REQUIRE(fpga_receiver_service.Start(&grpc_context, &receiver_input, &empty).ok());

    REQUIRE(fpga_receiver_service.Stop(&grpc_context, &empty, &receiver_output).ok());
    REQUIRE_NOTHROW(writer->Stop());
}

TEST_CASE("JFJochWriterServiceTest_ZMQ","[JFJochWriter]") {
    RegisterHDF5Filter();

    Logger logger("test");
    ZMQContext context;
    std::string zmq_addr = "inproc://#1";

    JFJochWriterService writer(context, zmq_addr, logger);

    grpc::ServerContext grpc_context;
    JFJochProtoBuf::Empty empty;

    DiffractionExperiment x;
    x.FilePrefix("JFJochWriterTest").NumTriggers(1).ImagesPerTrigger(5)
        .UseInternalPacketGenerator(true).DataStreamModuleSize(1, {2})
        .Mode(DetectorMode::Raw).PedestalG0Frames(0);

    // Create Receiver Service
    JungfrauCalibration calib(x);

    JFJochProtoBuf::JFJochReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    *receiver_input.mutable_calibration() = calib;
    receiver_input.add_writer_zmq_address(zmq_addr);

    std::vector<double> empty_gain(RAW_MODULE_SIZE);

    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        auto test = new HLSSimulatedDevice(i, 64);
        for (int m = 0; m < x.GetModulesNum(i); m++)
            test->LoadModuleGain(empty_gain, m);
        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    JFJochReceiverService fpga_receiver_service(tmp_devices, context, logger);

    JFJochProtoBuf::JFJochWriterInput writer_input;
    *writer_input.mutable_jungfraujoch_settings() = x;

    JFJochProtoBuf::JFJochReceiverOutput receiver_output;
    JFJochProtoBuf::JFJochWriterOutput writer_output;

    REQUIRE(x.GetImageNum() == 5);
    REQUIRE(writer.Start(&grpc_context, &writer_input, &empty).ok());

    REQUIRE(fpga_receiver_service.Start(&grpc_context, &receiver_input, &empty).ok());

    REQUIRE(fpga_receiver_service.Stop(&grpc_context, &empty, &receiver_output).ok());
    REQUIRE(writer.Stop(&grpc_context, &empty, &empty).ok());

    //TODO: Check contest of HDF5 file
}
