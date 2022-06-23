// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define WVL_1A_IN_KEV           12.39854

#define DELAY_FRAMES_STOP_AND_QUIT 5
#define RAW_MODULE_LINES        (512L)
#define RAW_MODULE_COLS         (1024L)
#define RAW_MODULE_SIZE         (RAW_MODULE_LINES * RAW_MODULE_COLS)

#define CONVERTED_MODULE_LINES  (514L)
#define CONVERTED_MODULE_COLS   (1030L)
#define CONVERTED_MODULE_SIZE   (CONVERTED_MODULE_LINES * CONVERTED_MODULE_COLS)

#define FPGA_BUFFER_LOCATION_SIZE (RAW_MODULE_SIZE * sizeof(short))

#define PEDESTAL_FRAME_ID    (UINT64_MAX -1)
#define MIN_COUNT_TIME_IN_US 10
#define MIN_FRAME_TIME_HALF_SPEED_IN_US 1000
#define MIN_FRAME_TIME_FULL_SPEED_IN_US 500

#define READOUT_TIME_IN_US 20

#define GRPC_MAX_MESSAGE_SIZE (1000L*1000L*1000L)
#define MIN_ENERGY 0.1
#define MAX_ENERGY 25.0

#define PEDESTAL_WINDOW_SIZE 128

#define FRAME_TIME_PEDE_G1G2_IN_US (10*1000)

#define SENSOR_THICKNESS_IN_UM 320.0
#define PIXEL_SIZE_IN_UM        75.0
#define PIXEL_SIZE_IN_MM       (PIXEL_SIZE_IN_UM/1000.0)
#define DETECTOR_NAME          "JF4M"
#define SOURCE_NAME_SHORT      "SLS"
#define SOURCE_NAME            "Swiss Light Source"
#define INSTRUMENT_NAME        "X06SA VESPA"
#define INSTRUMENT_NAME_SHORT  "VESPA"
#define SENSOR_MATERIAL        "Si"

#define GAIN_G0_MULTIPLIER 32
#define GAIN_G1_MULTIPLIER (-1)
#define GAIN_G2_MULTIPLIER (-1)

// For FPGA
/* This number is unique and is declared in ~snap/ActionTypes.md */
#define ACTION_TYPE   0x52324158
#define RELEASE_LEVEL 0x0020

#define MODE_RAW                    0
#define MODE_CONV                   1

#define MODE_INTERNAL_PACKET_GEN 0x0100L
#define MODE_BITSHUFFLE_FPGA     0x0200L

#define TASK_NO_DATA_STREAM UINT16_MAX

// Compression
#define LZ4_BLOCK_SIZE  4096
#define ZSTD_BLOCK_SIZE 4096
#define ZSTD_USE_JFJOCH_RLE 0xFFFF

#define PIXEL_OUT_SATURATION    (INT16_MAX)
#define PIXEL_OUT_LOST          (INT16_MIN)
#define PIXEL_OUT_0xFFFF        (INT16_MIN)
#define PIXEL_OUT_G1_SATURATION (INT16_MIN)
#define PIXEL_OUT_GAINBIT_2     (INT16_MIN)

#define IMAGE_METADATA_VERSION 0x52320001

// FPGA register map

#define ADDR_CTRL_REGISTER       0x0000
#define ADDR_GIT_SHA1            0x000C
#define ADDR_ACTION_TYPE         0x0010
#define ADDR_RELEASE_LEVEL       0x0014
#define ADDR_HBM_TEMP            0x0018
#define ADDR_HBM_MAX_TEMP        0x001C
#define ADDR_MAX_MODULES_FPGA    0x0020
#define ADDR_MODS_INT_PKT_GEN    0x0024
#define ADDR_STALLS_HOST_LO      0x0028
#define ADDR_STALLS_HOST_HI      0x002C
#define ADDR_STALLS_HBM_LO       0x0030
#define ADDR_STALLS_HBM_HI       0x0034
#define ADDR_FIFO_STATUS         0x0038

#define ADDR_PACKETS_PROC_LO     0x0040
#define ADDR_PACKETS_PROC_HI     0x0044
#define ADDR_PACKETS_ETH_LO      0x0048
#define ADDR_PACKETS_ETH_HI      0x004C
#define ADDR_PACKETS_ICMP_LO     0x0050
#define ADDR_PACKETS_ICMP_HI     0x0054
#define ADDR_PACKETS_UDP_LO      0x0058
#define ADDR_PACKETS_UDP_HI      0x005C

#define ADDR_MAC_ADDR_LO         0x0060
#define ADDR_MAC_ADDR_HI         0x0064
#define ADDR_IPV4_ADDR           0x0068
#define ADDR_NMODULES            0x006C
#define ADDR_DATA_COL_MODE       0x0070
#define ADDR_ONE_OVER_ENERGY     0x0074
#define ADDR_FRAMES_PER_TRIG     0x0078


#define ADDR_MAILBOX_WRDATA      0x00
#define ADDR_MAILBOX_RDDATA      0x08
#define ADDR_MAILBOX_STATUS      0x10
#define ADDR_MAILBOX_SIT         0x18
#define ADDR_MAILBOX_RIT         0x1C

#define MAILBOX_EMPTY            (1 << 0)
#define MAILBOX_FULL             (1 << 1)
#define MAILBOX_STA              (1 << 2)
#define MAILBOX_RTA              (1 << 3)

#define CTRL_REGISTER_IDLE               (1<<1u)
#define CTRL_REGISTER_100G_ALIGNED       (1<<8u)
#define CTRL_REGISTER_100G_STATUS        (1<<9u)
#define CTRL_REGISTER_100G_BAD_FCS       (1<<16u)
#define CTRL_REGISTER_HBM_SETUP_COMPLETE (1<<14u)
#define CTRL_REGISTER_QSFP_MOD_PRESENT   (1<<18u)
#define CTRL_REGISTER_START              (1<<0u)

#endif //DEFINITIONS_H
