// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <sys/wait.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../grpc/gRPCServer_Template.h"
#include "../common/Logger.h"
#include "JFJochWriterService.h"

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    Logger logger("jfjoch_writer");

    if (argc == 1)
        logger.Error("Usage ./jfjoch_writer <json config file>");

    std::vector<std::string> grpc_addr;
    std::string base_directory;
    {
        nlohmann::json input;
        std::ifstream file(argv[1]);
        try {
            input = nlohmann::json::parse(file);
        } catch (const nlohmann::json::exception &e) {
            logger.Error("JSON Parsing exception: " + std::string(e.what()));
            exit(EXIT_FAILURE);
        }
        if (input.contains("base_directory"))
            base_directory = input["base_directory"];

        if (!input.contains("grpc_addr"))
            logger.Error("Input file needs writer services gRPC addresses");
        if (!input["grpc_addr"].is_array())
            logger.Error("grpc_addr must be array");

        for (const auto &j: input["grpc_addr"]) {
            if (j.is_number()) {
                int64_t tcp_port = j.get<int64_t>();
                if ((tcp_port <= 0) || (tcp_port >= UINT16_MAX))
                    logger.Error("tcp port {} invalid", tcp_port);
                grpc_addr.push_back("0.0.0.0:" + std::to_string(tcp_port));
            } else if (j.is_string()) {
                grpc_addr.push_back(j.get<std::string>());
            } else
                logger.Error("grpc_addr array element must be string");

        }
    }

    std::vector<pid_t> child_pids;

    for (const auto &addr: grpc_addr) {
        Logger logger_local(addr);

        pid_t pid = fork();
        if (pid == 0) {
            ZMQContext context;
            JFJochWriterService service(context, logger_local);
            if (!base_directory.empty())
                service.BaseDirectory(base_directory);

            auto server = gRPCServer(addr, service);
            logger_local.Info("gRPC configuration listening on address " + addr);
            server->Wait();
        } else
            child_pids.push_back(pid);
    }
    int status;
    for (const auto &p: child_pids)
        wait(&status);
}