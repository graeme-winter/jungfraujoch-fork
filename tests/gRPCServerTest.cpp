// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../grpc/gRPCServer_Template.h"

#include "../common/Logger.h"
#include "../fpga/receiver/JFJochReceiverService.h"
#include "../grpc/JFJochReceiverClient.h"
#include "../fpga/host/HLSSimulatedDevice.h"


#include <grpcpp/grpcpp.h>

TEST_CASE("JFJochReceiver_gRPC_server", "[gRPC]") {
    DiffractionExperiment x;
    x.DataStreamModuleSize(2, {4});

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    AcquisitionDevice *test = new HLSSimulatedDevice(0, 64);
    aq_devices.emplace_back(test);

    std::vector<AcquisitionDevice *>  tmp_devices;
    for (const auto &i: aq_devices)
        tmp_devices.emplace_back(i.get());

    ZMQContext zmq_context;
    Logger logger("receiver");
    JFJochReceiverService service(tmp_devices, zmq_context,logger);

    auto server = gRPCServer("unix:receiver_test", service);
    {
        JFJochReceiverClient client;
        REQUIRE_NOTHROW(client.Connect("unix:receiver_test"));
    }

    server->Shutdown();
}