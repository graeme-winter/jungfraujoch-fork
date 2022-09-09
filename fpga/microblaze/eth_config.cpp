// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "xparameters.h"
#include "xil_io.h"

int main() {
    // Enable RS_FECf
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x107C, 0x03);

    // Restart TX and RX cores + deassert resets
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0004, (1 << 31) + (1 << 30));
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0004, 0x0);

    // TX Send RFI
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x10);

    // Enable RX
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0014, 0x01);

    enum state {ETH_RX_DOWN, ETH_RX_UP};
    state curr_state = ETH_RX_DOWN;

    while (1) {
        int eth_aligned = Xil_In32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0204) & 0x2;
        switch (curr_state) {
            case ETH_RX_DOWN:
                if (eth_aligned) {
                    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x1);
                    curr_state = ETH_RX_UP;
                }
                break;
            case ETH_RX_UP:
                if (!eth_aligned) {
                    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x10);
                    curr_state = ETH_RX_DOWN;
                }
                break;
        }
    }
    return 0;
}
