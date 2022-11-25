// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jfjoch_drv.h"
#include "../../common/Definitions.h"
#include <linux/bitops.h>
#include <linux/types.h>

DEFINE_MUTEX(set_config_mutex);
DEFINE_MUTEX(set_mac_mutex);

DEFINE_MUTEX(send_wr_mutex);
DEFINE_MUTEX(read_wc_mutex);

u32 parity(uint32_t values[4]) {
    return (hweight32(values[0]) + hweight32(values[1]) + hweight32(values[2])) % 2;
}

void jfjoch_start(struct jfjoch_drvdata *drvdata) {
    u32 run_val = XDMA_CTRL_RUN_STOP | XDMA_CTRL_IE_DESC_ALIGN_MISMATCH | XDMA_CTRL_IE_DESC_ERROR | XDMA_CTRL_IE_READ_ERROR
                  | XDMA_CTRL_IE_WRITE_ERROR | XDMA_CTRL_IE_DESC_COMPLETED
                  | XDMA_CTRL_STM_MODE_WB; // Disable stream writeback

    // Set PCIe beats counters
    iowrite32((1 << 1), drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0xC0);
    iowrite32((1 << 2), drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0xC0);

    iowrite32((1 << 1), drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0xC0);
    iowrite32((1 << 2), drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0xC0);

    // Start DMA
    // RUN + enable logging of certain error conditions ==> H2C channel 0 control register
    iowrite32(run_val, drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0x04);
    // RUN ==> C2H channel 0 control register
    iowrite32(run_val, drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0x04);

    // Set Mailbox FIFOs, so interrupt threshold is 4 messages
    iowrite32(251, drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_SIT);
    iowrite32(3,   drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_RIT);

    // Write Start value to action config register
    iowrite32(0x1, drvdata->bar0 + ACTION_CONFIG_OFFSET);
}

void jfjoch_end(struct jfjoch_drvdata *drvdata) {
    // Write cancel register
    iowrite32(0x4, drvdata->bar0 + ACTION_CONFIG_OFFSET);

    // RUN ==> H2C channel 0 control register
    iowrite32(0, drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0x04);
    // RUN ==> C2H channel 0 control register
    iowrite32(0, drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0x04);
}


void jfjoch_cancel(struct jfjoch_drvdata *drvdata) {
    iowrite32(0x4, drvdata->bar0 + ACTION_CONFIG_OFFSET);
}

int jfjoch_send_wr(struct jfjoch_drvdata *drvdata, u32 handle) {
    u32 sta;
    dma_addr_t addr = 0;
    u32 parity = 0;

    if (handle != 0xFFFFFFFFLU) {
        if (handle >= nbuffer)
            return -EFAULT;
        addr = drvdata->bufs[handle].dma_address;
    }

    parity = (hweight32(handle) + hweight64(addr)) % 2;

    mutex_lock(&send_wr_mutex);

    sta = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_STATUS) & MAILBOX_STA;
    if (!sta) {
        mutex_unlock(&send_wr_mutex);
        return -EAGAIN;
    }
    iowrite32(handle, drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_WRDATA);
    iowrite32(PCI_DMA_H(addr), drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_WRDATA);
    iowrite32(PCI_DMA_L(addr), drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_WRDATA);
    iowrite32(parity, drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_WRDATA);

    mutex_unlock(&send_wr_mutex);

    return 0;
}

int jfjoch_read_wc(struct jfjoch_drvdata *drvdata, u32 *output) {
    u32 rta;

    mutex_lock(&read_wc_mutex);

    rta = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_STATUS) & MAILBOX_RTA;
    if (!rta) {
        mutex_unlock(&read_wc_mutex);
        return -EAGAIN;
    }

    output[0] = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_RDDATA);
    output[1] = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_RDDATA);
    output[2] = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_RDDATA);
    output[3] = ioread32(drvdata->bar0 + MAILBOX_OFFSET + ADDR_MAILBOX_RDDATA);

    mutex_unlock(&read_wc_mutex);

    return 0;
}

void jfjoch_set_config(struct jfjoch_drvdata *drvdata, const struct ActionConfig *config) {
    mutex_lock(&set_config_mutex);
    memcpy_toio((drvdata->bar0) + ACTION_CONFIG_OFFSET + ADDR_IPV4_ADDR, config, sizeof(struct ActionConfig));
    mutex_unlock(&set_config_mutex);
}

void jfjoch_get_config(struct jfjoch_drvdata *drvdata, struct ActionConfig *config) {
    memcpy_fromio(config, (drvdata->bar0) + ACTION_CONFIG_OFFSET + ADDR_IPV4_ADDR, sizeof(struct ActionConfig));
}

void jfjoch_get_status(struct jfjoch_drvdata *drvdata, struct ActionStatus *status) {
    memcpy_fromio(status, drvdata->bar0 + ACTION_CONFIG_OFFSET, sizeof(struct ActionStatus));
}

void jfjoch_set_mac_addr(struct jfjoch_drvdata *drvdata, u64 *mac_addr) {
    mutex_lock(&set_mac_mutex);
    memcpy_toio((drvdata->bar0) + ACTION_CONFIG_OFFSET + ADDR_MAC_ADDR_LO, mac_addr, sizeof(uint64_t));
    mutex_unlock(&set_mac_mutex);
}

void jfjoch_get_mac_addr(struct jfjoch_drvdata *drvdata, u64 *mac_addr) {
    memcpy_fromio(mac_addr, (drvdata->bar0) + ACTION_CONFIG_OFFSET + ADDR_MAC_ADDR_LO, sizeof(uint64_t));
}

void jfjoch_is_idle(struct jfjoch_drvdata *drvdata, uint32_t *output) {
    if (ioread32(drvdata->bar0 + ACTION_CONFIG_OFFSET + ADDR_CTRL_REGISTER) & CTRL_REGISTER_IDLE)
        *output = 1;
    else
        *output = 0;
}

void jfjoch_setup_cms(struct jfjoch_drvdata *drvdata) {
    iowrite32(0x1, drvdata->bar0 + CMS_OFFSET + ADDR_CMS_MB_RESETN_REG);
}

void jfjoch_get_env_data(struct jfjoch_drvdata *drvdata, struct ActionEnvParams *env_params) {
    env_params->mailbox_status_reg   = ioread32(drvdata->bar0 + MAILBOX_OFFSET + 0x10);
    env_params->mailbox_err_reg      = ioread32(drvdata->bar0 + MAILBOX_OFFSET + 0x14);

    env_params->fpga_temp_C          = ioread32(drvdata->bar0 + CMS_OFFSET + ADDR_CMS_FPGA_TEMP_INS_REG);

    env_params->fpga_pcie_3p3V_I_mA  = ioread32(drvdata->bar0 + CMS_OFFSET + ADDR_CMS_3V3PEX_I_IN_INS_REG);
    env_params->fpga_pcie_12V_I_mA   = ioread32(drvdata->bar0 + CMS_OFFSET + ADDR_CMS_12VPEX_I_IN_INS_REG);
    env_params->fpga_pcie_3p3V_V_mV  = ioread32(drvdata->bar0 + CMS_OFFSET + ADDR_CMS_3V3_PEX_INS_REG);
    env_params->fpga_pcie_12V_V_mV   = ioread32(drvdata->bar0 + CMS_OFFSET + ADDR_CMS_12V_PEX_INS_REG);

    env_params->pcie_h2c_descriptors = ioread32(drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0x48);
    env_params->pcie_h2c_beats       = ioread32(drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0xCC);
    env_params->pcie_h2c_status      = ioread32(drvdata->bar0 + PCIE_OFFSET + (0<<12) + 0x40);

    env_params->pcie_c2h_descriptors = ioread32(drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0x48);
    env_params->pcie_c2h_beats       = ioread32(drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0xCC);
    env_params->pcie_c2h_status      = ioread32(drvdata->bar0 + PCIE_OFFSET + (1<<12) + 0x40);
}
