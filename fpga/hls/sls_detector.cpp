// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

void sls_detector(AXI_STREAM &udp_payload_in,
                hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > &udp_metadata_in,
                AXI_STREAM &data_out,
                hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out) {
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE axis register both port=udp_payload_in
#pragma HLS INTERFACE axis register both port=data_out
#pragma HLS INTERFACE axis register both port=udp_metadata_in
#pragma HLS INTERFACE axis register both port=addr_out

#pragma HLS pipeline II=1 enable_flush

    enum   udp_state {INSPECT_HEADER, FORWARD, DISCARD};
    static udp_state state = INSPECT_HEADER;

    static ap_uint<128> reminder = 0;
    packet_512_t packet_in;
    packet_512_t packet_out;

    packet_out.user = 0;
    packet_out.dest = 0;
    packet_out.id = 0;

    udp_payload_in >> packet_in;
    ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata;

    switch (state) {
        case INSPECT_HEADER:
            state = DISCARD;
            udp_metadata_in >> udp_metadata;
            if ((udp_metadata_len(udp_metadata) == 8248)
                && (udp_metadata_eth_err(udp_metadata) == 0)
                && (udp_metadata_len_err(udp_metadata) == 0)) {

                ap_uint<16> udp_dest_port = udp_metadata_dest_port(udp_metadata);
                ap_uint<5> module = (udp_dest_port % 64) / 2;
                ap_uint<7> eth_packet = (packet_in.data(127,96) % 128) | ((udp_dest_port[0] == 1) ? 64 : 0);
                if (module < MAX_MODULES_FPGA) {
                    addr_out << addr_packet(eth_packet,
                                            module,
                                            packet_in.data(63, 0),
                                            0,
                                            packet_in.data[5 * 64 + 31],
                                            packet_in.data(5 * 64 + 31, 5 * 64),
                                            packet_in.data(3 * 64 + 63, 3 * 64),
                                            packet_in.data(2 * 64 + 63, 2 * 64));

                    reminder = packet_in.data(511, 384);
                    state = FORWARD;
                }
            }

            break;
        case FORWARD:
            packet_out.data = (packet_in.data(383, 0), reminder);
            packet_out.last = packet_in.last;
            data_out << packet_out;
            reminder = packet_in.data(511, 384);
            break;
        case DISCARD:
            break;
    }

    if (packet_in.last)
        state = INSPECT_HEADER;
}