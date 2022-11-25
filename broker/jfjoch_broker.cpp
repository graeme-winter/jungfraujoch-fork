// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

#include "../common/Logger.h"
#include "../grpc/gRPCServer_Template.h"

#include "JFJochBrokerService.h"

int main (int argc, char **argv) {
    if (argc > 4) {
        std::cout << "Usage ./jfjoch_broker {<JSON config> {<TCP gRPC port> {<log file>}}}" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint16_t tcp_port = 5232;
    if (argc >= 3) tcp_port = atoi(argv[2]);

    std::string log_file;
    if (argc >= 4)
        log_file = argv[3];

    Logger logger("jfjoch_broker", log_file);

    nlohmann::json input;
    if (argc > 1) {
        std::ifstream file(argv[1]);
        try {
            input = nlohmann::json::parse(file);
        } catch (const nlohmann::json::exception &e) {
            logger.Error("JSON Parsing exception: " + std::string(e.what()));
            exit(EXIT_FAILURE);
        }
    }

    DiffractionExperiment experiment(2, {4,4}, 8, 36);
    experiment.PedestalG0Frames(2000).PedestalG1Frames(300).PedestalG2Frames(300);
    experiment.MaskChipEdges(true).MaskModuleEdges(true);

    std::random_device generator;

    uint8_t base_ipv4_addr_net = UINT8_MAX;
    if (input.contains("ipv4_subnet"))
        base_ipv4_addr_net = input["ipv4_subnet"];

    std::uniform_int_distribution<uint8_t> ipv4_subnet_distribution(0,UINT8_MAX-1);
    if (base_ipv4_addr_net == UINT8_MAX)
        base_ipv4_addr_net = ipv4_subnet_distribution(generator);
    experiment.BaseIPv4Address("10.1." + std::to_string(base_ipv4_addr_net) + ".1");
    logger.Info("Base IPv4 address for FPGA and detector modules: "
                + DiffractionExperiment::IPv4AddressToStr(experiment.GetDestIPv4Address(0)));

    JFJochBrokerService broker(logger);
    broker.Experiment() = experiment;

    if (input.contains("receiver_addr"))
        broker.Services().Receiver(input["receiver_addr"]);
    if (input.contains("writer")) {
        if (input["writer"].is_array()) {
            for (const auto &j: input["writer"])
                broker.Services().Writer(j["addr_grpc"], j["addr_zmq"]);
        } else {
            broker.Services().Writer(input["writer"]["addr_grpc"], input["writer"]["addr_zmq"]);
        }
    }
    if (input.contains("detector_addr"))
        broker.Services().Detector(input["detector_addr"]);
     if (input.contains("indexer"))
        broker.Services().Indexer(input["indexer"]["addr_grpc"], input["indexer"]["addr_zmq"]);

    JFJochProtoBuf::FacilityMetadata facility_metadata;
    facility_metadata.set_source_name(SOURCE_NAME);
    facility_metadata.set_source_name_short(SOURCE_NAME_SHORT);
    facility_metadata.set_instrument_name(INSTRUMENT_NAME);
    facility_metadata.set_instrument_name_short(INSTRUMENT_NAME_SHORT);
    broker.Services().FacilityMetadata(facility_metadata);

    auto server = gRPCServer("0.0.0.0:" + std::to_string(tcp_port), broker);
    logger.Info("Detector module number: {}", broker.Experiment().GetModulesNum());
    logger.Info("gRPC configuration listening on port {}", tcp_port);
    logger.Info("Started");
    server->Wait();
}