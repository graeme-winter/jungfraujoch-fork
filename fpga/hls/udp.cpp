// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

void udp(AXI_STREAM &eth_in,
         AXI_STREAM &udp_payload_out,
         hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > &udp_metadata_out,
         uint64_t& counter) {
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE axis register both port=eth_in
#pragma HLS INTERFACE axis register both port=udp_payload_out
#pragma HLS INTERFACE axis register both port=udp_metadata_out
#pragma HLS INTERFACE ap_vld port=counter

#pragma HLS pipeline II=1 enable_flush

    enum   udp_state {INSPECT_HEADER, FORWARD, DISCARD};
    static udp_state state = INSPECT_HEADER;
    static packet_512_t packet_in, packet_out;
    static uint16_t eth_packets_expected;
    static uint16_t eth_packets_analyzed;
    static ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata;

    static uint64_t internal_counter = 0;
    counter = internal_counter;

    ap_uint<16> udp_dest_port, udp_len;

    eth_in >> packet_in;

    packet_out.dest = 0;
    packet_out.user = 0;
    packet_out.id = 0;
    packet_out.last = packet_in.last;

    switch (state) {
        case INSPECT_HEADER:
            udp_len = get_header_field_16(packet_in.data, ipv4_payload_pos + 32);
            udp_dest_port = get_header_field_16(packet_in.data, ipv4_payload_pos + 16);

            udp_metadata_len(udp_metadata) = udp_len;
            udp_metadata_dest_port(udp_metadata) = udp_dest_port;

            eth_packets_expected = (udp_payload_pos/8 + udp_len) / 64 + ((udp_payload_pos/8 + udp_len) % 64 ? 1 : 0);
            eth_packets_analyzed = 1;

            packet_out.data(511 - udp_payload_pos, 0) = packet_in.data(511, udp_payload_pos);

            if (packet_in.last)
                udp_payload_out << packet_out;
            else
                state = FORWARD;
            break;
        case FORWARD:
            packet_out.data(511, 512 - udp_payload_pos) = packet_in.data(udp_payload_pos - 1, 0);

            if (eth_packets_analyzed > eth_packets_expected) {
                packet_out.last = 1;
                state = DISCARD;
            }

            udp_payload_out << packet_out;
            packet_out.data(511 - udp_payload_pos, 0) = packet_in.data(511, udp_payload_pos);
            eth_packets_analyzed++;
            break;
        case DISCARD:
            break;
    }

    if (packet_in.last) {
        udp_metadata_eth_err(udp_metadata) = packet_in.user;
        udp_metadata_len_err(udp_metadata) = ((eth_packets_analyzed != eth_packets_expected) ? 1 : 0);

        udp_metadata_out << udp_metadata;
        state = INSPECT_HEADER;
        internal_counter++;
    }
}

