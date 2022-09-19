// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jfjoch_drv.h"
#include "jfjoch_ioctl.h"

long jfjoch_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct jfjoch_drvdata *drvdata = container_of(file->private_data, struct jfjoch_drvdata, miscdev);
    struct ActionStatus status;
    struct ActionConfig config;
    struct ActionEnvParams env_params;
    u32 exchange[4];
    int err;

    switch (cmd) {
        case IOCTL_JFJOCH_START:
            jfjoch_start(drvdata);
            return 0;
        case IOCTL_JFJOCH_END:
            jfjoch_end(drvdata);
            return 0;
        case IOCTL_JFJOCH_CANCEL:
            jfjoch_cancel(drvdata);
            return 0;
        case IOCTL_JFJOCH_STATUS:
            jfjoch_get_status(drvdata, &status);
            if (copy_to_user((char *) arg, &status, sizeof(struct ActionStatus)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_READ_CONFIG:
            jfjoch_get_config(drvdata, &config);
            if (copy_to_user((char *) arg, &config, sizeof(struct ActionConfig)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_SET_CONFIG:
            if (copy_from_user(&config, (char *) arg, sizeof(struct ActionConfig)) != 0)
                return -EFAULT;
            jfjoch_set_config(drvdata, &config);
            return 0;
        case IOCTL_JFJOCH_GET_ENV_DATA:
            jfjoch_get_env_data(drvdata, &env_params);
            if (copy_to_user((char *) arg, &env_params, sizeof(struct ActionEnvParams)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_SEND_WR:
            if (copy_from_user(exchange, (char *) arg, sizeof(u32)) != 0)
                return -EFAULT;
            return jfjoch_send_wr(drvdata, exchange[0]);
        case IOCTL_JFJOCH_READ_WC_MBOX:
            err = jfjoch_read_wc(drvdata,exchange);
            if (err)
                return err;
            if (copy_to_user((char *) arg, exchange, 4 * sizeof(u32)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_BUF_COUNT:
            if (copy_to_user((char *) arg, &nbuffer, sizeof(int32_t)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_SET_MAC:
            if (copy_from_user(exchange, (char *) arg, sizeof(u64)) != 0)
                return -EFAULT;
            jfjoch_set_mac_addr(drvdata, (u64 *) exchange);
            return 0;
        case IOCTL_JFJOCH_GET_MAC:
            jfjoch_get_mac_addr(drvdata, (u64 *) exchange);
            if (copy_to_user((char *) arg, exchange, sizeof(u64)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_ISIDLE:
            jfjoch_is_idle(drvdata, exchange);
            if (copy_to_user((char *) arg, exchange, sizeof(int32_t)) != 0)
                return -EFAULT;
            return 0;
        case IOCTL_JFJOCH_RESET:
            jfjoch_reset(drvdata);
            return 0;
        default:
            return -ENOTTY;
    }
}