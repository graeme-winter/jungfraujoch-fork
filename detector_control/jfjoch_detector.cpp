// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fstream>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>
#include <nlohmann/json.hpp>

#include "JFJochDetector.h"
#include "../common/Logger.h"
#include "../grpc/gRPCServer_Template.h"

int main(int argc, char **argv) {
    Logger logger("jfjoch_detector");

    nlohmann::json input;

    std::ifstream file(argv[1]);
    try {
        input = nlohmann::json::parse(file);
    } catch (const nlohmann::json::exception &e) {
        logger.Error("JSON Parsing exception: " + std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    uint16_t tcp_port = 5236;
    if (argc == 3) tcp_port = atoi(argv[2]);

    JFJochDetector service;

    auto server = gRPCServer("0.0.0.0:" + std::to_string(tcp_port), service);
    logger.Info("gRPC configuration listening on port " + std::to_string(tcp_port));
    server->Wait();
}
