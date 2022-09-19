// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jfjoch_drv.h"
#include "../../common/Definitions.h"

#include <linux/module.h>

int nbuffer = 1024;
module_param(nbuffer, int, 0600);

int jfjoch_alloc_phys_continous_buf(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    struct device *const dev = &pdev->dev;
    int i;

    drvdata->nbuf = 0;
    drvdata->bufs = kzalloc(sizeof(struct jfjoch_buf) * nbuffer, GFP_KERNEL);

    if (drvdata->bufs == NULL) {
        dev_err(dev, "Failed to allocate local structure\n");
        return -ENOMEM;
    }

    for (i = 0; i < nbuffer; i++) {
        dma_addr_t bus_addr;
        void *tmp = pci_zalloc_consistent(pdev,FPGA_BUFFER_LOCATION_SIZE,&bus_addr);
        if (tmp == NULL) {
            dev_err(dev, "Failed to allocate %d PCI consistent buffer\n", i);
            jfjoch_free_phys_continous_buf(pdev);
            return -ENOMEM;
        }
        drvdata->bufs[i].kernel_address = tmp;
        drvdata->bufs[i].dma_address = bus_addr;

        drvdata->nbuf++;
    }

    return 0;
}

void jfjoch_free_phys_continous_buf(struct pci_dev *pdev) {
    int i;
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    for (i = 0; i < drvdata->nbuf; i++) {
        if (drvdata->bufs[i].kernel_address != NULL) {
            pci_free_consistent(pdev,
                                FPGA_BUFFER_LOCATION_SIZE,
                                drvdata->bufs[i].kernel_address,
                                drvdata->bufs[i].dma_address);
        }
    }
    kfree(drvdata->bufs);
}

int jfjoch_setup_calibration(struct pci_dev *pdev) {
    struct jfjoch_drvdata *drvdata = pci_get_drvdata(pdev);
    u32 i;
    u32 cell_count = drvdata->max_modules * (3 + 3 * 16) + 1;

    if ((cell_count > nbuffer) || (cell_count > 512)) {
        dev_err(&pdev->dev, "Not enough buffers to support this card\n");
        return -EINVAL;
    }

    for (i = 0; i < cell_count; i++) {
        u64 addr = drvdata->bufs[i].dma_address;
        iowrite32(PCI_DMA_L(addr), drvdata->bar0 + CALIB_BRAM_OFFSET + i * 2 * 4);
        iowrite32(PCI_DMA_H(addr), drvdata->bar0 + CALIB_BRAM_OFFSET + (i * 2 + 1) * 4);
    }

    return 0;
}

int jfjoch_cdev_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long offset, buffer_number, len;

    struct jfjoch_drvdata *drvdata = container_of(file->private_data, struct jfjoch_drvdata, miscdev);

    if (vma->vm_flags & VM_EXEC) {
        return -EINVAL;
    }

    vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP | VM_DONTCOPY | VM_LOCKED;
    // Don't allow region to be expanded (i.e. via mremap() in user mode), to be copied on fork, or dumped, or swapped

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
