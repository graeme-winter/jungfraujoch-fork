// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCH_DRV_H
#define JUNGFRAUJOCH_JFJOCH_DRV_H

#include <linux/pci.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>

#include "../host/ActionConfig.h"

// From Xilinx XDMA
/* obtain the 32 most significant (high) bits of a 32-bit or 64-bit address */
#define PCI_DMA_H(addr) ((addr >> 16) >> 16)
/* obtain the 32 least significant (low) bits of a 32-bit or 64-bit address */
#define PCI_DMA_L(addr) (addr & 0xffffffffUL)

/* bits of the SG DMA control register */
#define XDMA_CTRL_RUN_STOP			    (1UL << 0)
#define XDMA_CTRL_IE_DESC_STOPPED		(1UL << 1)
#define XDMA_CTRL_IE_DESC_COMPLETED		(1UL << 2)
#define XDMA_CTRL_IE_DESC_ALIGN_MISMATCH	(1UL << 3)
#define XDMA_CTRL_IE_MAGIC_STOPPED		(1UL << 4)
#define XDMA_CTRL_IE_IDLE_STOPPED		(1UL << 6)
#define XDMA_CTRL_IE_READ_ERROR			(0x1FUL << 9)
#define XDMA_CTRL_IE_WRITE_ERROR	    (0x1FUL << 14)
#define XDMA_CTRL_IE_DESC_ERROR			(0x1FUL << 19)
#define XDMA_CTRL_NON_INCR_ADDR			(1UL << 25)
#define XDMA_CTRL_POLL_MODE_WB			(1UL << 26)
#define XDMA_CTRL_STM_MODE_WB			(1UL << 27)


// Offset for BAR #0 for action configuration
#define ACTION_CONFIG_OFFSET     (0x10000)
#define MAILBOX_OFFSET           (0x30000)
#define CALIB_BRAM_OFFSET        (0x60000)
#define CMS_OFFSET               (0xC0000)
#define PCIE_OFFSET              (0x90000)

#define ADDR_CMS_MB_RESETN_REG        0x020000
#define ADDR_CMS_FPGA_TEMP_INS_REG    0x028100 // in C
#define ADDR_CMS_3V3PEX_I_IN_INS_REG  0x028280 // in mA
#define ADDR_CMS_12VPEX_I_IN_INS_REG  0x0280D0 // in mA
#define ADDR_CMS_12V_PEX_INS_REG      0x028028 // in mV
#define ADDR_CMS_3V3_PEX_INS_REG      0x028034 // in mV

struct jfjoch_buf {
    dma_addr_t           dma_address;
    void                *kernel_address;
};

struct jfjoch_drvdata {
    struct miscdevice     miscdev;
    struct pci_dev       *pdev;
    unsigned int          nbuf;
    struct jfjoch_buf    *bufs;
    char                 *bar0;
    char                  name[256];
    u32                   max_modules;
    u32                   git_sha1;
};

int jfjoch_register_misc_dev(struct pci_dev *pdev);
void jfjoch_unregister_misc_dev(struct pci_dev *pdev);

int jfjoch_alloc_phys_continous_buf(struct pci_dev *pdev);
void jfjoch_free_phys_continous_buf(struct pci_dev *pdev);

int jfjoch_setup_pcie_for_dma(struct pci_dev *pdev);
int jfjoch_map_cfg_bar(struct pci_dev *pdev);
int jfjoch_check_version(struct pci_dev *pdev);

long jfjoch_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int jfjoch_cdev_mmap(struct file *file, struct vm_area_struct *vma);
int jfjoch_cdev_release(struct inode *inode, struct file *file);
int jfjoch_cdev_open(struct inode *inode, struct file *file);

int jfjoch_setup_calibration(struct pci_dev *pdev);
void jfjoch_setup_cms(struct jfjoch_drvdata *drvdata);

void jfjoch_start(struct jfjoch_drvdata *drvdata);
void jfjoch_cancel(struct jfjoch_drvdata *drvdata);
void jfjoch_end(struct jfjoch_drvdata *drvdata);

int jfjoch_send_wr(struct jfjoch_drvdata *drvdata, u32 handle);
int jfjoch_read_wc(struct jfjoch_drvdata *drvdata, u32 *output);
void jfjoch_set_config(struct jfjoch_drvdata *drvdata, const struct ActionConfig *config);
void jfjoch_get_config(struct jfjoch_drvdata *drvdata, struct ActionConfig *config);
void jfjoch_get_status(struct jfjoch_drvdata *drvdata, struct ActionStatus *status);
void jfjoch_set_mac_addr(struct jfjoch_drvdata *drvdata, u64 *mac_addr);
void jfjoch_get_mac_addr(struct jfjoch_drvdata *drvdata, u64 *mac_addr);
void jfjoch_is_idle(struct jfjoch_drvdata *drvdata, uint32_t *output);

void jfjoch_get_env_data(struct jfjoch_drvdata *drvdata, struct ActionEnvParams *env_params);

void jfjoch_reset(struct jfjoch_drvdata *drvdata);

static const struct file_operations jfjoch_cdev_fileops = {
        .mmap           = jfjoch_cdev_mmap,
        .unlocked_ioctl = jfjoch_cdev_ioctl,
        .open           = jfjoch_cdev_open,
        .release        = jfjoch_cdev_release,
        .llseek         = no_llseek
};

extern int nbuffer;

#endif //JUNGFRAUJOCH_JFJOCH_DRV_H
