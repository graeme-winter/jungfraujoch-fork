// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jfjoch_drv.h"
#include "../../common/Definitions.h"

int jfjoch_setup_pcie_for_dma(struct pci_dev *pdev) {
    struct device *const dev = &pdev->dev;

    // Set DMA masks
    int err = pci_set_dma_mask(pdev, DMA_BIT_MASK(64));
    if (err) {
        dev_err(dev, "Failed to set 64-bit DMA mask (%d)\n", err);
        return err;
    }

    err = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64));
    if (err) {
        dev_err(dev, "Failed to set 64-bit DMA consistent mask (%d)\n", err);
        return err;
    }

    // Enable bus master for the device
    pci_set_master(pdev);
    return 0;
}

int jfjoch_map_cfg_bar(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    struct device *const dev = &pdev->dev;

    // Request BAR 0 to map
    int err = pci_request_region(pdev, 0, "jfjoch");
    if (err) {
        dev_err(dev, "Unable to request PCI memory I/O addresses\n");
        return err;
    }

    // Map BAR 0 for configuration
    drvdata->bar0 = pci_iomap(pdev, 0, 0);
    if (drvdata->bar0 == NULL) {
        dev_err(dev, "Unable to map PCI memory I/O addresses\n");
        pci_release_region(pdev, 0); // Release requested region
        return -ENOMEM;
    }
    return 0;
}

int jfjoch_check_version(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    struct device *const dev = &pdev->dev;
    unsigned int action_type, release_level;

    // Check if FPGA action is matching the driver
    action_type   = ioread32(drvdata->bar0 + ACTION_CONFIG_OFFSET + ADDR_ACTION_TYPE);
    release_level = ioread32(drvdata->bar0 + ACTION_CONFIG_OFFSET + ADDR_RELEASE_LEVEL);

    if (action_type != ACTION_TYPE) {
        dev_err(dev, "Mismatch in JFJoch action type (%x)\n", action_type);
        return -EINVAL;
    }

    if (release_level != RELEASE_LEVEL) {
        dev_err(dev, "Mismatch in JFJoch release level (%x)\n", release_level);
        return -EINVAL;
    }
    return 0;
}