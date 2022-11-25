// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fstream>
#include <nlohmann/json.hpp>

#include "../grpc/gRPCServer_Template.h"
#include "../common/Logger.h"
#include "JFJochWriterService.h"

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    Logger logger("jfjoch_writer");

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

    uint16_t tcp_port = 5234;
    if (argc >= 3) tcp_port = atoi(argv[2]);

    std::string grpc_addr = "0.0.0.0:" + std::to_string(tcp_port);
    ZMQContext context;

    JFJochWriterService service(context, logger);

    if (input.contains("base_directory"))
        service.BaseDirectory(input["base_directory"]);

    auto server = gRPCServer(grpc_addr, service);
    logger.Info("gRPC configuration listening on address " + grpc_addr);
    server->Wait();
}