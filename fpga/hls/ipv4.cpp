// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"
#include "ip_header_checksum.h"

void ipv4(AXI_STREAM &eth_in,
        AXI_STREAM &udp_out,
        AXI_STREAM &icmp_out,
        ap_uint<32> fpga_ipv4_addr) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis register both port=eth_in
#pragma HLS INTERFACE axis register both port=udp_out
#pragma HLS INTERFACE axis register both port=icmp_out
#pragma HLS INTERFACE ap_none register port=fpga_ipv4_addr

#pragma HLS PIPELINE II=1 enable_flush
    enum ipv4_dest {DEST_UDP = 0, DEST_ICMP = 1, DEST_IGNORE=2};
    enum ipv4_state {INSPECT_HEADER, FORWARD, DISCARD};
    static ipv4_state state = INSPECT_HEADER;
    static ipv4_dest dest;
    static uint64_t internal_counter = 0;

    packet_512_t packet_in = eth_in.read();

    if (state == INSPECT_HEADER) {
        dest = DEST_IGNORE;
            ap_uint<4> ip_version = packet_in.data(eth_payload_pos + 8 - 1, eth_payload_pos + 4);
            ap_uint<8> ipv4_protocol = packet_in.data(eth_payload_pos + 80 - 1, eth_payload_pos + 72);
            ap_uint<32> ipv4_source_ip = get_header_field_32_network_order(packet_in.data,
                                                                           eth_payload_pos + 96);
            ap_uint<32> ipv4_dest_ip = get_header_field_32_network_order(packet_in.data, eth_payload_pos + 128);
            ap_uint<16> ipv4_header_checksum_check = computeCheckSum20B(packet_in.data(eth_payload_pos + 159, eth_payload_pos));

            if ((ip_version == 4) && (ipv4_dest_ip == fpga_ipv4_addr) && (ipv4_header_checksum_check == 0)) {
                // IP is version 4 + IP address is correct + checksum is OK
                // Possibly the conditions can be relaxed for broadcast ICMP
                if (ipv4_protocol == PROTOCOL_UDP) {
                    state = FORWARD;
                    dest = DEST_UDP;
                } else if (ipv4_protocol == PROTOCOL_ICMP) {
                    state = FORWARD;
                    dest = DEST_ICMP;
                }
            }

    }

    switch (dest) {
        case DEST_UDP:
            udp_out.write(packet_in);
            break;
        case DEST_ICMP:
            icmp_out.write(packet_in);
            break;
        default:
            break;
    }

    if (packet_in.last) {
        state = INSPECT_HEADER;
        internal_counter++;
    }
}



