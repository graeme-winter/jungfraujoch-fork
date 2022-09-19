// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later


#include <iostream>
#include <bitset>

#include "../common/JFJochException.h"
#include "PCIExpressDevice.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Usage: ./jfjoch_pcie_status <device name>" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Device " << argv[1] << std::endl;
    std::cout << std::endl;
    try {
        PCIExpressDevice test(argv[1], 0);

        auto ptr = test.GetDeviceBuffer(9);

        for (int i = 0; i < RAW_MODULE_SIZE; i++)
            ptr[i] = i % 16;

    } catch (const JFJochException &e) {
        std::cout << e.what() << std::endl;
        std::cout << std::endl;
    }
}