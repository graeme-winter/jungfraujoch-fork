// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCH_IOCTL_H
#define JUNGFRAUJOCH_JFJOCH_IOCTL_H

#include "../host/ActionConfig.h"

#ifdef __KERNEL__
#include <uapi/asm-generic/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define IOCTL_JFJOCH_MAGIC        0xE1

#define IOCTL_JFJOCH_START        _IO (IOCTL_JFJOCH_MAGIC, 0)
#define IOCTL_JFJOCH_STATUS       _IOR(IOCTL_JFJOCH_MAGIC, 1, struct ActionStatus)
#define IOCTL_JFJOCH_READ_CONFIG  _IOR(IOCTL_JFJOCH_MAGIC, 2, struct ActionConfig)
#define IOCTL_JFJOCH_SET_CONFIG   _IOW(IOCTL_JFJOCH_MAGIC, 3, struct ActionConfig)
#define IOCTL_JFJOCH_CANCEL       _IO (IOCTL_JFJOCH_MAGIC, 4)
#define IOCTL_JFJOCH_READ_WC_MBOX _IOR(IOCTL_JFJOCH_MAGIC, 5, uint32_t[4])
#define IOCTL_JFJOCH_SEND_WR      _IOW(IOCTL_JFJOCH_MAGIC, 6, uint32_t)
#define IOCTL_JFJOCH_BUF_COUNT    _IOR(IOCTL_JFJOCH_MAGIC, 7, uint32_t)
#define IOCTL_JFJOCH_SET_MAC      _IOW(IOCTL_JFJOCH_MAGIC, 8, uint64_t)
#define IOCTL_JFJOCH_GET_MAC      _IOR(IOCTL_JFJOCH_MAGIC, 9, uint64_t)
#define IOCTL_JFJOCH_ISIDLE       _IOR(IOCTL_JFJOCH_MAGIC, 10, uint32_t)
#define IOCTL_JFJOCH_GET_ENV_DATA _IOR(IOCTL_JFJOCH_MAGIC, 11, struct ActionEnvParams)
#define IOCTL_JFJOCH_END          _IO (IOCTL_JFJOCH_MAGIC, 12)
#define IOCTL_JFJOCH_RESET        _IO (IOCTL_JFJOCH_MAGIC, 13)

#endif //JUNGFRAUJOCH_JFJOCH_IOCTL_H
