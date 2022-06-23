// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../grpc/gRPCServer_Template.h"
#include "../common/Logger.h"
#include "JFJochWriterService.h"

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    Logger logger("jfjoch_writer");

    uint16_t tcp_port = 5234;
    if (argc >= 2) tcp_port = atoi(argv[1]);

    uint16_t zmq_tcp_port = 5235;
    if (argc >= 3) zmq_tcp_port = atoi(argv[2]);

    uint16_t zmq_threads = 4;
    if (argc >= 4) zmq_threads = atoi(argv[3]);

    std::string zmq_addr = "tcp://0.0.0.0:" + std::to_string(zmq_tcp_port);
    std::string grpc_addr = "0.0.0.0:" + std::to_string(tcp_port);

    ZMQContext context;
    context.NumThreads(zmq_threads); // Use 4 threads (up to 4 GB/s)

    JFJochWriterService service(context, zmq_addr, logger);
    logger.Info("ZeroMQ pull image socket listening on address " + zmq_addr);
    auto server = gRPCServer(grpc_addr, service);
    logger.Info("gRPC configuration listening on address " + grpc_addr);
    server->Wait();
}