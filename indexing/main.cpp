// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../common/Logger.h"
#include "../grpc/gRPCServer_Template.h"
#include "../indexing/JFJochIndexerService.h"

int main(int argc, char **argv) {
    Logger logger("jfjoch_xgandalf");

    nlohmann::json runtime_params_json;
    if ((argc != 2) && (argc != 3)) {
        std::cout << "Usage: ./jfjoch_xgandalf <config.json> <gRPC port>" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::ifstream input(argv[1]);
    input >> runtime_params_json;

    uint16_t tcp_port = 5405;
    if (argc >= 3)
        tcp_port = atoi(argv[2]);

    ZMQContext context;

    JFJochIndexerService service(context, logger);

    if (runtime_params_json.contains("nthreads"))
        service.NumThreads(runtime_params_json["nthreads"]);

    auto server = gRPCServer("0.0.0.0:" + std::to_string(tcp_port), service);
    logger.Info("gRPC configuration listening on port " + std::to_string(tcp_port));
    logger.Info("Started");
    server->Wait();
}
