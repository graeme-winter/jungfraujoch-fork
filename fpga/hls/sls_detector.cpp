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

#pragma HLS pipeline II=1 style=flp

    enum   sls_detector_state {INSPECT_HEADER, FORWARD_4K, FORWARD_8K, DISCARD};
    static sls_detector_state state = INSPECT_HEADER;

    static ap_uint<128> reminder = 0;
    packet_512_t packet_in;
    packet_512_t packet_out;

    packet_out.user = 0;
    packet_out.dest = 0;
    packet_out.id = 0;

    udp_payload_in >> packet_in;
    ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata;
    static ap_uint<5> module;
    static ap_uint<8> eth_packet;
    static ap_uint<64> frame_number;
    static ap_uint<64> bunchid;
    static ap_uint<64> timestamp;
    static ap_uint<32> jf_debug;
    static ap_uint<8> counter;

#pragma HLS RESET variable=state

    switch (state) {
        case INSPECT_HEADER:
            state = DISCARD;
            udp_metadata_in >> udp_metadata;

            if ((udp_metadata_eth_err(udp_metadata) == 0)
                && (udp_metadata_len_err(udp_metadata) == 0)) {
                frame_number = packet_in.data(63, 0);
                jf_debug = packet_in.data(5 * 64 + 31, 5 * 64);
                timestamp = packet_in.data(3 * 64 + 63, 3 * 64);
                bunchid = packet_in.data(2 * 64 + 63, 2 * 64);
                counter = 0;
                ap_uint<16> udp_dest_port = udp_metadata_dest_port(udp_metadata);
                reminder = packet_in.data(511, 384);

                if (udp_metadata_payload_size(udp_metadata) == 8240) {
                    module = (udp_dest_port % 64) / 2;
                    ap_uint<1> module_part = (udp_dest_port % 64) % 2;
                    eth_packet = (packet_in.data(127, 96) % 128) | (module_part * 64);
                    if (module < MAX_MODULES_FPGA) {
                        state = FORWARD_8K;
                    }
                } else if (udp_metadata_payload_size(udp_metadata) == 4144) {
                    module = (udp_dest_port % 64) / 4;
                    ap_uint<2> module_part = (udp_dest_port % 64) % 4;

                    eth_packet = (packet_in.data(127, 96) % 128) | (module_part * 64);
                    if (module < MAX_MODULES_FPGA) {
                        state = FORWARD_4K;
                    }
                }
            }
            break;
        case FORWARD_4K:
            if (counter == 63) {
                addr_out << addr_packet(eth_packet, module, frame_number,
                                        jf_debug, timestamp, bunchid, 0);
                packet_out.last = 1;
            } else
                packet_out.last = 0;
            packet_out.data = (packet_in.data(383, 0), reminder);
            data_out << packet_out;
            reminder = packet_in.data(511, 384);

            counter++;
            break;
        case FORWARD_8K:
            if (counter == 63) {
                addr_out << addr_packet(2 * eth_packet, module, frame_number,
                                        jf_debug, timestamp, bunchid, 1);
                packet_out.last = 1;
            } else if (counter == 127) {
                addr_out << addr_packet(2 * eth_packet + 1, module, frame_number,
                                        jf_debug, timestamp, bunchid, 1);
                packet_out.last = 1;
            } else
                packet_out.last = 0;
            packet_out.data = (packet_in.data(383, 0), reminder);
            data_out << packet_out;
            reminder = packet_in.data(511, 384);

            counter++;
            break;
        case DISCARD:
            break;
    }

    if (packet_in.last)
        state = INSPECT_HEADER;
}