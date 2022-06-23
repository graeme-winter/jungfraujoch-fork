// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

static const uint64_t arp_refresh_s = 60;
static const uint64_t low_freq_Hz = 200L*1000L*1000L;

static const ap_uint<16> ARP_REQUEST = 0x0100;
static const ap_uint<16> ARP_REPLY   = 0x0200;
static const ap_uint<16> ARP_HW_ETH  = 0x0100;
static const ap_uint<16> ARP_PR_IP   = 0x0008;

static const ap_uint<32> arp_sha_pos = eth_payload_pos + 8 * 8;
static const ap_uint<32> arp_spa_pos = arp_sha_pos + 6 * 8;
static const ap_uint<32> arp_tha_pos = arp_spa_pos + 4 * 8;
static const ap_uint<32> arp_tpa_pos = arp_tha_pos + 6 * 8;

inline void compose_arp_packet(packet_512_t  &packet_out,
                               ap_uint<48> fpga_mac_addr,
                               ap_uint<32> fpga_ipv4_addr,
                               ap_uint<48> arp_src_mac_addr,
                               ap_uint<32> arp_src_ipv4_addr,
                               ap_uint<16> arp_operation) {
#pragma HLS INLINE
    packet_out.data = 0;
    packet_out.data(47, 0) = arp_src_mac_addr;
    packet_out.data(48 + 47, 48) = fpga_mac_addr;

    packet_out.data(96 + 16, 96) = 0x0608; // 0x0806
    packet_out.data(eth_payload_pos + 15, eth_payload_pos) = ARP_HW_ETH; // ETH = 0x0001
    packet_out.data(eth_payload_pos + 31, eth_payload_pos + 16) = ARP_PR_IP; // IPv4 = 0x0800
    packet_out.data(eth_payload_pos + 39, eth_payload_pos + 32) = 0x6;
    packet_out.data(eth_payload_pos + 47, eth_payload_pos + 40) = 0x4;
    packet_out.data(eth_payload_pos + 63, eth_payload_pos + 48) = arp_operation; // 2 = reply

    packet_out.data(arp_sha_pos + 47, arp_sha_pos) = fpga_mac_addr;
    packet_out.data(arp_spa_pos + 31, arp_spa_pos) = fpga_ipv4_addr;
    packet_out.data(arp_tha_pos + 47, arp_tha_pos) = arp_src_mac_addr;
    packet_out.data(arp_tpa_pos + 31, arp_tpa_pos) = arp_src_ipv4_addr;

    packet_out.dest = 0;
    packet_out.last = 1;
    packet_out.keep = UINT64_MAX; // 64 bytes
    packet_out.user = 0;

}

void arp(AXI_STREAM &arp_in,
         AXI_STREAM &eth_out,
         ap_uint<48> fpga_mac_addr,
         ap_uint<32> fpga_ipv4_addr,
         ap_uint<1> enable,
         ap_uint<1> arp_start) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1 enable_flush
#pragma HLS INTERFACE ap_none register port=fpga_mac_addr
#pragma HLS INTERFACE ap_none register port=fpga_ipv4_addr
#pragma HLS INTERFACE ap_none register port=enable
#pragma HLS INTERFACE ap_none register port=arp_start

#pragma HLS INTERFACE axis register both port=arp_in
#pragma HLS INTERFACE axis register both port=eth_out

    packet_512_t packet_in, packet_out;
    static ap_uint<1> prev_start = 0;
    static ap_uint<36> counter = 0; // 2^36 cycles @ 200 MHz = 343 seconds

    if (enable && (counter == 0) && (fpga_mac_addr != 0) && (fpga_ipv4_addr != 0)) {
        compose_arp_packet(packet_out, fpga_mac_addr, fpga_ipv4_addr,
                           MAC_BROADCAST, fpga_ipv4_addr,
                           ARP_REQUEST);
        eth_out.write(packet_out);
    } else if (!arp_in.empty()) {
        arp_in.read(packet_in);

        if ((fpga_mac_addr != 0) && (fpga_ipv4_addr != 0)) {

            ap_uint<16> arp_hw_type = packet_in.data(eth_payload_pos + 15, eth_payload_pos);
            ap_uint<16> arp_protocol_type = packet_in.data(eth_payload_pos + 31, eth_payload_pos + 16);
            ap_uint<16> arp_operation = packet_in.data(eth_payload_pos + 63, eth_payload_pos + 48);
            ap_uint<48> arp_src_hw_addr = packet_in.data(arp_sha_pos + 47, arp_sha_pos);
            ap_uint<32> arp_src_ip_addr = packet_in.data(arp_spa_pos + 31, arp_spa_pos);
            ap_uint<32> arp_dst_ip_addr = packet_in.data(arp_tpa_pos + 31, arp_tpa_pos);

            if ((arp_hw_type == ARP_HW_ETH)
                && (arp_operation == ARP_REQUEST)
                && (arp_protocol_type == ARP_PR_IP)
                && (arp_src_ip_addr != fpga_ipv4_addr) // Don't reply on own gratuitous ARP
                && (arp_dst_ip_addr == fpga_ipv4_addr)) {

                compose_arp_packet(packet_out, fpga_mac_addr, fpga_ipv4_addr,
                                   arp_src_hw_addr, arp_src_ip_addr,
                                   ARP_REPLY);
                eth_out.write(packet_out);
            }
        }
    }

    if (!prev_start && arp_start)
        counter = 0;
    else if (enable && (counter < low_freq_Hz * arp_refresh_s) && (fpga_mac_addr != 0) && (fpga_ipv4_addr != 0))
        counter++;
    else
        counter = 0;

    prev_start = arp_start;
}

