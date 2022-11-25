// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/io.h>

#include "../../common/Definitions.h"
#include "jfjoch_drv.h"

MODULE_AUTHOR("Filip Leonarski; Paul Scherrer Institute");
MODULE_DESCRIPTION("Jungfraujoch device module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define XDMA_GEN4_x8        (0x9048)
#define XDMA_GEN3_x16       (0x903F)

static const struct pci_device_id jfjoch_pci_tbl[] = {
        { PCI_DEVICE(PCI_VENDOR_ID_XILINX, XDMA_GEN4_x8) },
        { PCI_DEVICE(PCI_VENDOR_ID_XILINX, XDMA_GEN3_x16) },
        { 0, },
};

static struct pci_driver jfjoch_pci_driver;
static const struct file_operations jfjoch_cdev_fileops;

static int jfjoch_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    int err;
    struct device *const dev = &pdev->dev;
    struct jfjoch_drvdata *drvdata;
    struct ActionStatus status;

    if ((nbuffer < 64) || (nbuffer > 65536)) {
        dev_err(dev, "nbuffer parameter must be in range 64-65536\n");
        err = -EINVAL;
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
    drvdata->pdev = pdev;

    // Enable device
    err = pci_enable_device(pdev);
    if (err) {
        dev_err(dev, "Failed to enable PCI device (%d)\n", err);
        goto dealloc_private_data;
    }

    err = jfjoch_map_cfg_bar(pdev);
    if (err)
        goto disable_device;

    err = jfjoch_check_version(pdev);
    if (err)
        goto unmap_bar;

    err = jfjoch_register_misc_dev(pdev);
    if (err)
        goto unmap_bar;

    // Setup DMA on PCIe side
    err = jfjoch_setup_pcie_for_dma(pdev);
    if (err)
        goto deregister_misc;

    // Allocate memory
    err = jfjoch_alloc_phys_continous_buf(pdev);
    if (err)
        goto deregister_misc;

    jfjoch_get_status(drvdata, &status);
    drvdata->git_sha1            = status.git_sha1;
    drvdata->max_modules         = status.max_modules;

    err = jfjoch_setup_calibration(pdev);
    if (err)
        goto deregister_misc;

    jfjoch_setup_cms(drvdata);

    dev_info(drvdata->miscdev.this_device, "Jungfraujoch FPGA loaded with FW build: %x", drvdata->git_sha1);
    return 0;

    deregister_misc:
    misc_deregister(&drvdata->miscdev);

    unmap_bar:
    pci_iounmap(pdev, drvdata->bar0);
    pci_release_region(pdev, 0);

    disable_device:
    pci_disable_device(pdev);

    dealloc_private_data:
    kfree(drvdata);

    ret_with_err:
    return err;
}


void jfjoch_reset(struct jfjoch_drvdata *drvdata) {
    pci_reset_function(drvdata->pdev);

    jfjoch_setup_cms(drvdata);
    jfjoch_setup_calibration(drvdata->pdev);
    dev_info(drvdata->miscdev.this_device, "Jungfraujoch FPGA restarted");
}

static void jfjoch_pci_remove(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);

    if (!drvdata)
        return;

    pci_clear_master(pdev);

    jfjoch_unregister_misc_dev(pdev);
    jfjoch_free_phys_continous_buf(pdev);
    pci_iounmap(pdev, drvdata->bar0);
    pci_release_region(pdev, 0);
    pci_disable_device(pdev);
    kfree(drvdata);
}

static struct pci_driver jfjoch_pci_driver = {
        .name		= "jfjoch",
        .id_table	= jfjoch_pci_tbl,
        .probe		= jfjoch_pci_probe,
        .remove	    = jfjoch_pci_remove
};


MODULE_DEVICE_TABLE(pci, jfjoch_pci_tbl);
module_pci_driver(jfjoch_pci_driver);
