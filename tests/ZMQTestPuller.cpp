// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../writer/JFJochWriter.h"

int main(int argc, char **argv) {
    Logger logger("ZMQTestPuller");

    if ((argc < 2) || (argc > 3)) {
        std::cout << "Usage: ./ZMQImagePusher <output file prefix> {<tcp_port>}" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint16_t zmq_tcp_port = 5678;
    if (argc >= 3)
        zmq_tcp_port = atoi(argv[2]);

    RegisterHDF5Filter();

    ZMQContext context;
    DiffractionExperiment experiment(2, {8}, 8, 36);
    experiment.ImagesPerTrigger(10000000).ImagesPerFile(1000).FilePrefix(argv[1]);

    JFJochWriter writer(experiment, context, "tcp://0.0.0.0:" + std::to_string(zmq_tcp_port), logger);
    writer.WaitTillDone();
}