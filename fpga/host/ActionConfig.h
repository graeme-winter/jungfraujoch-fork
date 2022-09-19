// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACTIONCONFIG_H
#define JUNGFRAUJOCH_ACTIONCONFIG_H

#ifdef __KERNEL__
typedef __u32 uint32_t;
typedef __u64 uint64_t;
#else
#include <cstdint>
#endif

#pragma pack(push)
#pragma pack(4)

struct ActionConfig {
    uint32_t fpga_ipv4_addr;
    uint32_t nmodules;
    uint32_t mode;
    uint32_t one_over_energy;
    uint32_t frames_internal_packet_gen;
    uint32_t nstorage_cells;
};

struct ActionStatus {
    uint32_t ctrl_reg;
    uint32_t reserved_0;
    uint32_t set_led;
    uint32_t git_sha1;
    uint32_t action_type;
    uint32_t release_level;
    uint32_t hbm_temperature;
    uint32_t hbm_max_temperature;
    uint32_t max_modules;
    uint32_t modules_internal_packet_generator;
    uint64_t pipeline_stalls_host;
    uint64_t pipeline_stalls_hbm;
    uint32_t fifo_status;
    uint32_t reserved_1;
    uint64_t packets_processed;
    uint64_t packets_eth;
    uint64_t packets_icmp;
    uint64_t packets_udp;
};

struct ActionEnvParams {
    uint32_t mailbox_status_reg;
    uint32_t mailbox_err_reg;
    uint32_t fpga_temp_C;

    uint32_t fpga_pcie_12V_I_mA;
    uint32_t fpga_pcie_3p3V_I_mA;
    uint32_t fpga_pcie_12V_V_mV;
    uint32_t fpga_pcie_3p3V_V_mV;

    uint32_t pcie_h2c_descriptors;
    uint32_t pcie_c2h_descriptors;
    uint32_t pcie_h2c_beats;
    uint32_t pcie_c2h_beats;

    uint32_t pcie_h2c_status;
    uint32_t pcie_c2h_status;
};
#pragma pack(pop)

#endif //JUNGFRAUJOCH_ACTIONCONFIG_H
