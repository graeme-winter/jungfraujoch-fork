// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/Definitions.h"
#include "../fpga/host/ActionConfig.h"

TEST_CASE("ActionStatus") {
    ActionStatus status{};
    status.packets_icmp = (1uL<<56) + (1uL<<9);
    status.packets_eth = 21321321321212L;
    status.packets_processed = 5454545455454545454L;
    status.packets_udp = 123216721123676789L;

    status.modules_internal_packet_generator = 3456;
    status.pipeline_stalls_hbm = (1uL<<33) + (1uL<<32) + (1uL<<4);
    status.pipeline_stalls_host = (1uL<<35) + (1uL<<30) + (1uL<<3);
    status.max_modules = 7;

    auto status32 = (uint32_t *) &status;
    CHECK(status32[ADDR_PACKETS_ICMP_LO/4] == (1uL<<9));
    CHECK(status32[ADDR_PACKETS_ICMP_HI/4] == (1uL<<(56-32)));
    CHECK(status32[ADDR_PACKETS_ETH_LO/4] == (status.packets_eth & UINT32_MAX));
    CHECK(status32[ADDR_PACKETS_ETH_HI/4] == (status.packets_eth >> 32));
    CHECK(status32[ADDR_PACKETS_UDP_LO/4] == (status.packets_udp & UINT32_MAX));
    CHECK(status32[ADDR_PACKETS_UDP_HI/4] == (status.packets_udp >> 32));
    CHECK(status32[ADDR_PACKETS_PROC_LO/4] == (status.packets_processed & UINT32_MAX));
    CHECK(status32[ADDR_PACKETS_PROC_HI/4] == (status.packets_processed >> 32));

    CHECK(status32[ADDR_MODS_INT_PKT_GEN/4] == 3456);
    CHECK(status32[ADDR_MAX_MODULES_FPGA/4] == 7);

    CHECK(status32[ADDR_STALLS_HBM_HI/4] == 1 + 2);
    CHECK(status32[ADDR_STALLS_HBM_LO/4] == 1 << 4);

    CHECK(status32[ADDR_STALLS_HOST_HI/4] == (1 << (35-32)));
    CHECK(status32[ADDR_STALLS_HOST_LO/4] == (1<<30) + (1 << 3));
}


TEST_CASE("ActionConfigSize") {
    REQUIRE(sizeof(ActionConfig) == 5 * sizeof(uint32_t));
}

TEST_CASE("ActionConfig") {
    ActionConfig config{};
    auto config32 = (uint32_t *) &config;

    config.fpga_ipv4_addr = 32;
    CHECK(config32[(ADDR_IPV4_ADDR - ADDR_IPV4_ADDR)/4] == 32);

    config.nmodules = 456;
    CHECK(config32[(ADDR_NMODULES - ADDR_IPV4_ADDR)/4] == 456);

    config.one_over_energy = 1<<17;
    CHECK(config32[(ADDR_ONE_OVER_ENERGY - ADDR_IPV4_ADDR)/4] == 1<<17);

    config.mode = 1<<5;
    CHECK(config32[(ADDR_DATA_COL_MODE - ADDR_IPV4_ADDR)/4] == 1<<5);

    config.frames_per_trigger = 1<<18;
    CHECK(config32[(ADDR_FRAMES_PER_TRIG - ADDR_IPV4_ADDR)/4] == 1 << 18  );
}