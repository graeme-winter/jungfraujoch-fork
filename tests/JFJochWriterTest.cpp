// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <filesystem>

#include "../writer/JFJochWriterService.h"
#include "../common/ZMQImagePusher.h"
#include "../fpga/receiver/JFJochReceiverService.h"
#include "../fpga/host/HLSSimulatedDevice.h"

TEST_CASE("JFJochWriterTest_ZMQ","[JFJochWriter]") {
    RegisterHDF5Filter();

    Logger logger("test");
    ZMQContext context;
    std::string zmq_addr = "inproc://#1";

    grpc::ServerContext grpc_context;
    JFJochProtoBuf::Empty empty;

    DiffractionExperiment x(1, {2});
    x.FilePrefix("subdir/JFJochWriterTest").NumTriggers(1).ImagesPerTrigger(5)
            .UseInternalPacketGenerator(true).Mode(DetectorMode::Raw).PedestalG0Frames(0);

    JFJochProtoBuf::ReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    receiver_input.add_writer_zmq_address(zmq_addr);

    JFModuleGainCalibration gain;
    std::vector<std::unique_ptr<AcquisitionDevice>>  aq_devices;
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        auto test = new HLSSimulatedDevice(i, 64);
        for (int m = 0; m < x.GetModulesNum(i); m++)
            test->LoadModuleGain(gain, m);
        aq_devices.emplace_back(test);
    }

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQImagePusher pusher (context, {zmq_addr});
    JFJochReceiverService fpga_receiver_service(tmp_devices, logger, pusher);

    JFJochProtoBuf::WriterInput writer_input = x;
    writer_input.set_zmq_receiver_address(zmq_addr);

    JFJochProtoBuf::ReceiverOutput receiver_output;
    JFJochProtoBuf::WriterOutput writer_output;

    std::unique_ptr<JFJochWriter> writer;
    REQUIRE(x.GetImageNum() == 5);
    REQUIRE_NOTHROW(writer = std::make_unique<JFJochWriter>(writer_input, context, logger));

    REQUIRE(fpga_receiver_service.Start(&grpc_context, &receiver_input, &empty).ok());

    REQUIRE(fpga_receiver_service.Stop(&grpc_context, &empty, &receiver_output).ok());
    CHECK(receiver_output.images_sent() == 5);

    REQUIRE_NOTHROW(writer->Stop());

    // HDF5 file can be opened
    std::unique_ptr<HDF5ReadOnlyFile> file;
    REQUIRE_NOTHROW(file = std::make_unique<HDF5ReadOnlyFile>("subdir/JFJochWriterTest_data_000.h5"));
    std::unique_ptr<HDF5DataSet> dataset;
    REQUIRE_NOTHROW(dataset = std::make_unique<HDF5DataSet>(*file, "/entry/data/data"));
    std::unique_ptr<HDF5DataSpace> dataspace;
    REQUIRE_NOTHROW(dataspace = std::make_unique<HDF5DataSpace>(*dataset));

    REQUIRE(dataspace->GetNumOfDimensions() == 3);
    REQUIRE(dataspace->GetDimensions()[0] == 5);
    REQUIRE(dataspace->GetDimensions()[1] == RAW_MODULE_COLS);
    REQUIRE(dataspace->GetDimensions()[2] == 2*RAW_MODULE_LINES);

    REQUIRE(std::filesystem::remove("subdir/JFJochWriterTest_data_000.h5"));
    REQUIRE(std::filesystem::remove("subdir"));
}

TEST_CASE("JFJochWriterServiceTest_ZMQ","[JFJochWriter]") {
    RegisterHDF5Filter();

    Logger logger("test");
    ZMQContext context;
    std::string zmq_addr = "inproc://#1";

    JFJochWriterService writer(context, logger);

    grpc::ServerContext grpc_context;
    JFJochProtoBuf::Empty empty;

    DiffractionExperiment x(1, {2});
    x.FilePrefix("JFJochWriterTest").NumTriggers(1).ImagesPerTrigger(5)
        .UseInternalPacketGenerator(true)
        .Mode(DetectorMode::Raw).PedestalG0Frames(0);


    JFJochProtoBuf::ReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    receiver_input.add_writer_zmq_address(zmq_addr);

    JFModuleGainCalibration empty_gain;

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

    ZMQImagePusher pusher (context, {zmq_addr});
    JFJochReceiverService fpga_receiver_service(tmp_devices, logger, pusher);

    JFJochProtoBuf::WriterInput writer_input = x;
    writer_input.set_zmq_receiver_address(zmq_addr);

    JFJochProtoBuf::ReceiverOutput receiver_output;
    JFJochProtoBuf::WriterOutput writer_output;

    REQUIRE(x.GetImageNum() == 5);
    REQUIRE(writer.Start(&grpc_context, &writer_input, &empty).ok());

    REQUIRE(fpga_receiver_service.Start(&grpc_context, &receiver_input, &empty).ok());
    REQUIRE(fpga_receiver_service.Stop(&grpc_context, &empty, &receiver_output).ok());
    REQUIRE(writer.Stop(&grpc_context, &empty, &writer_output).ok());

    REQUIRE(writer_output.nimages() == 5);
    //TODO: Check contest of HDF5 file
}
