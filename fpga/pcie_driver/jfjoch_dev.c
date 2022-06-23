// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/io.h>

#include "../../common/Definitions.h"
#include "jfjoch_dev.h"
#include "ioctl.h"

MODULE_AUTHOR("Filip Leonarski; Paul Scherrer Institute");
MODULE_DESCRIPTION("Jungfraujoch device module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define PCI_VENDOR_ID_ALPHA_DATA (0x4144)
#define PCI_DEVICE_ID_9H3        (0x0906)

static ssize_t hbm_temp_show(struct device *dev,
                             struct device_attribute *attr,
                             char *buf) {
    //ssize_t n = snprint(buf, 25, "x");
    return 0;
}


static ssize_t fpga_temp_show(struct device *dev,
                             struct device_attribute *attr,
                             char *buf) {
    //ssize_t n = snprint(buf, 25, "x");
    return 0;
}

#define SYSFS_FILE1 hbm_temp
#define SYSFS_FILE2 fpga_temp

static DEVICE_ATTR_RO(SYSFS_FILE1);
static DEVICE_ATTR_RO(SYSFS_FILE2);


static int nbuffer = 1024;
module_param(nbuffer, int, 0600);

static int device_index = 0;

static const struct pci_device_id jfjoch_pci_tbl[] = {
        { PCI_DEVICE(PCI_VENDOR_ID_ALPHA_DATA, PCI_DEVICE_ID_9H3) },
        { 0, },
};

static struct pci_driver jfjoch_pci_driver;
static const struct file_operations jfjoch_cdev_fileops;

static int jfjoch_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    unsigned int action_type, release_level;
    int err, i;
    struct device *const dev = &pdev->dev;
    struct jfjoch_drvdata *drvdata;

    if ((nbuffer < 64) || (nbuffer > 65536)) {
        dev_err(dev, "nbuffer parameter must be in range 64-65536\n");
        err = EINVAL;
        goto ret_with_err;
    }

    // Setup private data structure
    drvdata = kzalloc(sizeof(struct jfjoch_drvdata), GFP_KERNEL);
    if (drvdata == NULL) {
        dev_err(dev, "Failed to allocate driver data\n");
        err = -ENOMEM;
        goto ret_with_err;
    }
    pci_set_drvdata(pdev, drvdata);

    // Enable device
    err = pci_enable_device(pdev);
    if (err) {
        dev_err(dev, "Failed to enable PCI device (%d)\n", err);
        goto dealloc_private_data;
    }

    // Request BAR 0 to map
    err = pci_request_region(pdev, 0, "jfjoch");
    if (err) {
        dev_err(dev, "Unable to request PCI memory I/O addresses\n");
        goto disable_device;
    }

    // Map BAR 0 for configuration
    drvdata->bar0 = pci_iomap(pdev, 0, 0);
    if (drvdata->bar0 == NULL) {
        dev_err(dev, "Unable to map PCI memory I/O addresses\n");
        err = -ENOMEM;
        goto release_region;
    }

    // Check if FPGA action is matching the driver
    action_type   = ioread32(drvdata->bar0 + ADDR_ACTION_TYPE);
    release_level = ioread32(drvdata->bar0 + ADDR_RELEASE_LEVEL);

    if (action_type != ACTION_TYPE) {
        dev_err(dev, "Mismatch in JFJoch action type (%x)\n", action_type);
        goto unmap_bar;
    }

    if (release_level != RELEASE_LEVEL) {
        dev_err(dev, "Mismatch in JFJoch release level (%x)\n", release_level);
        goto unmap_bar;
    }

    snprintf( drvdata->name, 255, "jfjoch%d", device_index++);

    drvdata->miscdev.mode  = 0660;
    drvdata->miscdev.minor = MISC_DYNAMIC_MINOR;
    drvdata->miscdev.fops = &jfjoch_cdev_fileops;
    drvdata->miscdev.name = drvdata->name;

    err = misc_register(&drvdata->miscdev);
    if (err != 0) {
        pr_notice("Misc device registration failed\n");
        goto unmap_bar;
    }

    // Set DMA masks
    err = pci_set_dma_mask(pdev, DMA_BIT_MASK(64));
    if (err) {
        dev_err(dev, "Failed to set 64-bit DMA mask (%d)\n", err);
        goto deregister_misc;
    }

    err = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64));
    if (err) {
        dev_err(dev, "Failed to set 64-bit DMA consistent mask (%d)\n", err);
        goto deregister_misc;
    }

    pci_set_master(pdev);

    // Allocate memory
    drvdata->nbuf = 0;
    drvdata->bufs = kzalloc(sizeof(struct jfjoch_buf) * nbuffer, GFP_KERNEL);

    if (drvdata->bufs == NULL) {
        dev_err(dev, "Failed to allocate local structure\n");
        err = ENOMEM;
        goto deregister_misc;
    }

    for (i = 0; i < nbuffer; i++) {
        dma_addr_t bus_addr;
        void *tmp = pci_alloc_consistent(pdev,FPGA_BUFFER_LOCATION_SIZE,&bus_addr);
        if (tmp == NULL) {
            dev_err(dev, "Failed to allocate %d PCI consistent buffer\n", i);
            goto dealloc_partial_buffers;
        }

        memset(tmp, 0, FPGA_BUFFER_LOCATION_SIZE);
        drvdata->bufs[i].owned_by_device = false;
        drvdata->bufs[i].kernel_address = tmp;
        drvdata->bufs[i].dma_address = bus_addr;

        drvdata->nbuf++;
    }

    dev_info(dev, "Jungfraujoch FPGA - all good!");
    return 0;

    dealloc_partial_buffers:
    for (i = 0; i < drvdata->nbuf; i++)
        pci_free_consistent(pdev,
                            FPGA_BUFFER_LOCATION_SIZE,
                            drvdata->bufs[i].kernel_address,
                            drvdata->bufs[i].dma_address);
    kfree(drvdata->bufs);

    deregister_misc:
    misc_deregister(&drvdata->miscdev);

    unmap_bar:
    pci_iounmap(pdev, drvdata->bar0);

    release_region:
    pci_release_region(pdev, 0);

    disable_device:
    pci_disable_device(pdev);

    dealloc_private_data:
    kfree(drvdata);

    ret_with_err:
    return err;
}

static void jfjoch_pci_remove(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    int i;

    if (!drvdata)
        return;

    for (i = 0; i < drvdata->nbuf; i++) {
        if (drvdata->bufs[i].kernel_address != NULL)
            pci_free_consistent(pdev,
                                FPGA_BUFFER_LOCATION_SIZE,
                            drvdata->bufs[i].kernel_address,
                            drvdata->bufs[i].dma_address);
    }

    pci_iounmap(pdev, drvdata->bar0);
    pci_release_region(pdev, 0);
    pci_disable_device(pdev);
    kfree(drvdata);
}


int jfjoch_cdev_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long offset, buffer_number, len;

    struct jfjoch_drvdata *drvdata = container_of(file->private_data, struct jfjoch_drvdata, miscdev);

    if (vma->vm_flags & VM_EXEC) {
        return -EINVAL;
    }

    vma->vm_flags |= VM_DONTEXPAND; /* Don't allow region to be expanded (i.e. via mremap() in user mode) */

    // pgoff determines which buffer to use
    offset = vma->vm_pgoff << PAGE_SHIFT;

    // Reset offset to zero
    vma->vm_pgoff = 0;

    // Offset must be multiple of buffer size
    if (offset % FPGA_BUFFER_LOCATION_SIZE)
        return -EINVAL;

    // Buffer number must be within allocated range
    buffer_number = offset / FPGA_BUFFER_LOCATION_SIZE;
    if (buffer_number >= drvdata->nbuf)
        return -EINVAL;

    // Size of mapped region must be exactly one buffer size
    len = vma->vm_end - vma->vm_start;
    if (len != FPGA_BUFFER_LOCATION_SIZE)
        return -EINVAL;

    return dma_mmap_coherent(&drvdata->pdev->dev,
                             vma,
                             drvdata->bufs[buffer_number].kernel_address,
                             drvdata->bufs[buffer_number].dma_address,
                             FPGA_BUFFER_LOCATION_SIZE);
}

long jfjoch_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct jfjoch_drvdata *drvdata = container_of(file->private_data, struct jfjoch_drvdata, miscdev);
    struct ActionStatus *status;
    struct ActionConfig *config;
    switch (cmd) {
        case IOCTL_JFJOCH_STATUS:
            status = kzalloc(sizeof(struct ActionStatus), GFP_KERNEL);
            if (status == NULL) {
                dev_err(&drvdata->pdev->dev, "Cannot allocate memory for action status\n");
                return -ENOMEM;
            }
            memcpy_fromio(status, drvdata->bar0, sizeof(struct ActionStatus));
            if (copy_to_user((char *) arg, status, sizeof(struct ActionStatus)) == 0) {
                kfree(status);
                return 0;
            } else {
                kfree(status);
                return -EFAULT;
            }
        case IOCTL_JFJOCH_CONFIG:
            config = kzalloc(sizeof(struct ActionConfig), GFP_KERNEL);
            if (config == NULL) {
                dev_err(&drvdata->pdev->dev, "Cannot allocate memory for action status\n");
                return -ENOMEM;
            }
            memcpy_fromio(config, (drvdata->bar0) + ADDR_MAC_ADDR_HI, sizeof(struct ActionConfig));
            if (copy_to_user((char *) arg, status, sizeof(struct ActionConfig)) == 0) {
                kfree(status);
                return 0;
            } else {
                kfree(status);
                return -EFAULT;
            }
        default:
            return -ENOTTY;
    }

    return 0;
}

static struct pci_driver jfjoch_pci_driver = {
        .name		= "jfjoch",
        .id_table	= jfjoch_pci_tbl,
        .probe		= jfjoch_pci_probe,
        .remove	    = jfjoch_pci_remove
};

static const struct file_operations jfjoch_cdev_fileops = {
        .mmap           = jfjoch_cdev_mmap,
        .unlocked_ioctl = jfjoch_cdev_ioctl,
        .llseek         = no_llseek
};

MODULE_DEVICE_TABLE(pci, jfjoch_pci_tbl);
module_pci_driver(jfjoch_pci_driver);
