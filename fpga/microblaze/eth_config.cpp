#include "xparameters.h"
#include "xintc.h"
#include "xil_io.h"

static XIntc InterruptController; /* Instance of the Interrupt Controller */
static void RXAlignedInterruptHandler() __attribute__ ((fast_interrupt));
static void RXNotAlignedInterruptHandler() __attribute__ ((fast_interrupt));
static volatile int eth_aligned = 0;

void RXAlignedInterruptHandler() {
    eth_aligned = 1;
}


void RXNotAlignedInterruptHandler() {
    eth_aligned = 0;
}

int main() {
    XIntc_Initialize(&InterruptController,
                     XPAR_MAC_100G_ETH_CTRL_AXI_INTC_0_DEVICE_ID);

    XIntc_Start(&InterruptController, XIN_REAL_MODE);

    XIntc_ConnectFastHandler(&InterruptController,
                             XPAR_MAC_100G_ETH_CTRL_AXI_INTC_0_MAC_100G_CMAC_USPLUS_0_STAT_RX_ALIGNED_INTR,
                             (XFastInterruptHandler) RXAlignedInterruptHandler);


    XIntc_ConnectFastHandler(&InterruptController,
                             XPAR_MAC_100G_ETH_CTRL_AXI_INTC_0_MAC_100G_CMAC_USPLUS_0_STAT_RX_ALIGNED_LOW_PRIORITY_INTR,
                             (XFastInterruptHandler) RXNotAlignedInterruptHandler);

    // Enable RS_FEC
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x107C, 0x03);

    // Restart TX and RX cores + deassert resets
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0004, (1 << 31) + (1 << 30));
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0004, 0x0);

    // TX Send RFI
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x10);

    // Enable RX
    Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x0014, 0x01);

    int curr_state = 0;

    while (1) {
        if ((eth_aligned == 0) && (curr_state == 1))
            Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x10);
        else if ((eth_aligned == 1) && (curr_state == 0))
            Xil_Out32(XPAR_MAC_100G_CMAC_USPLUS_0_BASEADDR + 0x000C, 0x1);
        curr_state = eth_aligned;
    }
    return 0;
}
