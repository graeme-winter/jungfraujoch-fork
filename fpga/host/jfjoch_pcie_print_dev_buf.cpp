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

        for (int i = 0; i < test.GetNumKernelBuffers(); i++) {
            auto ptr = test.GetDeviceBuffer(i);
            std::cout << std::hex << i << " ";
            for (int j = 0; j < 32; j++)
                std::cout << ptr[j] << " ";
            std::cout << std::endl;
        }

    } catch (const JFJochException &e) {
        std::cout << e.what() << std::endl;
        std::cout << std::endl;
    }
}