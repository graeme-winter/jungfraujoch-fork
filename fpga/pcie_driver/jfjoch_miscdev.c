// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jfjoch_drv.h"
#include <linux/module.h>

static int device_index = 0;

int jfjoch_register_misc_dev(struct pci_dev *pdev) {
    int err;
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);

    snprintf( drvdata->name, 255, "jfjoch%d", device_index++);

    drvdata->miscdev.mode  = 0660;
    drvdata->miscdev.minor = MISC_DYNAMIC_MINOR;
    drvdata->miscdev.fops = &jfjoch_cdev_fileops;
    drvdata->miscdev.name = drvdata->name;
    err = misc_register(&drvdata->miscdev);
    if (err)
        pr_notice("Misc device registration failed\n");
    return err;
}


void jfjoch_unregister_misc_dev(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    misc_deregister(&drvdata->miscdev);
}

int jfjoch_cdev_open(struct inode *inode, struct file *file) {
    return 0;
}

int jfjoch_cdev_release(struct inode *inode, struct file *file) {
    return 0;
}
