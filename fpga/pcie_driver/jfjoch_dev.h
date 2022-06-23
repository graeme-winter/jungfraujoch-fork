// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCH_DEV_H
#define JUNGFRAUJOCH_JFJOCH_DEV_H

#include <linux/pci.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>

#include "../host/ActionConfig.h"

struct jfjoch_buf {
    dma_addr_t           dma_address;
    void                *kernel_address;
    bool                 owned_by_device;
};

struct jfjoch_drvdata {
    struct mutex          ioctl_mutex;
    struct miscdevice     miscdev;
    struct pci_dev       *pdev;
    unsigned int          nbuf;
    struct jfjoch_buf    *bufs;
    char                 *bar0;
    char                  name[256];
};

#endif //JUNGFRAUJOCH_JFJOCH_DEV_H
