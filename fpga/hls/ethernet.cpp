// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

void ethernet(AXI_STREAM &eth_in,
              AXI_STREAM &ip_out,
              AXI_STREAM &arp_out,
              ap_uint<48> fpga_mac_addr,
              uint64_t& counter) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis register both port=eth_in
#pragma HLS INTERFACE axis register both port=ip_out
#pragma HLS INTERFACE axis register both port=arp_out
#pragma HLS INTERFACE ap_none register port=fpga_mac_addr
#pragma HLS INTERFACE ap_vld port=counter

#pragma HLS PIPELINE II=1 enable_flush
    enum eth_dest {DEST_IP = 0, DEST_ARP = 1, DEST_IGNORE = 2};
    enum eth_state {INSPECT_HEADER, FORWARD, DISCARD};
    static eth_state state = INSPECT_HEADER;
    static eth_dest dest;
    static uint64_t internal_counter = 0;

#pragma HLS RESET variable=state
#pragma HLS RESET variable=internal_counter

    counter = internal_counter;

    packet_512_t packet_in = eth_in.read();

    if (state == INSPECT_HEADER) {
        dest = DEST_IGNORE;
        if (fpga_mac_addr != 0) {
            ap_uint<48> dest_mac = get_mac_addr(packet_in.data, 0);
            ap_uint<48> src_mac = get_mac_addr(packet_in.data, 48);

            ap_uint<16> ether_type = get_header_field_16(packet_in.data, 12 * 8);

            if ((dest_mac == fpga_mac_addr) && (ether_type == ETHER_IP)) {
                state = FORWARD;
                dest = DEST_IP;
            } else if ((dest_mac == MAC_BROADCAST) && (ether_type == ETHER_ARP) && (packet_in.last)) {
                state = FORWARD;
                dest = DEST_ARP;
            }
        }
    }

    switch (dest) {
        case DEST_IP:
            ip_out.write(packet_in);
            break;
        case DEST_ARP:
            arp_out.write(packet_in);
            break;
        default:
            break;
    }

    if (packet_in.last) {
        state = INSPECT_HEADER;
        internal_counter++;
    }
}