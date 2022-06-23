// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_IOCTL_H
#define JUNGFRAUJOCH_IOCTL_H

#include "../host_app/ActionConfig.h"

#ifdef __KERNEL__
#include <uapi/asm-generic/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define IOCTL_JFJOCH_MAGIC        0xE1

#define IOCTL_JFJOCH_START        _IOW(IOCTL_JFJOCH_MAGIC, 0, struct ActionConfig)
#define IOCTL_JFJOCH_STATUS       _IOR(IOCTL_JFJOCH_MAGIC, 1, struct ActionStatus)
#define IOCTL_JFJOCH_CONFIG       _IOR(IOCTL_JFJOCH_MAGIC, 2, struct ActionConfig)
#define IOCTL_JFJOCH_START_ETH    _IO( IOCTL_JFJOCH_MAGIC, 3)
#define IOCTL_JFJOCH_CANCEL       _IO( IOCTL_JFJOCH_MAGIC, 4)
#define IOCTL_JFJOCH_READ_WC_MBOX _IOR(IOCTL_JFJOCH_MAGIC, 5, uint32_t[4])
#define IOCTL_JFJOCH_SEND_WR      _IOW(IOCTL_JFJOCH_MAGIC, 6, uint32_t)

#endif //JUNGFRAUJOCH_IOCTL_H
