// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

void internal_packet_generator(STREAM_512 &data_in, STREAM_512 &data_out,
                               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out,
                               volatile ap_uint<1> &in_cancel) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE register both axis port=addr_in
#pragma HLS INTERFACE register both axis port=addr_out
#pragma HLS INTERFACE ap_none register port=in_cancel

    packet_512_t packet_in;
    packet_512_t packet_out;

#ifdef __SYNTHESIS__
    ap_uint<512> module_cache[MODULES_INTERNAL_PACKET_GEN * (RAW_MODULE_SIZE * 2 / 64)];
#pragma HLS RESOURCE variable=module_cache core=RAM_2P_URAM latency=3
#else
    std::vector<ap_uint<512> > module_cache(MODULES_INTERNAL_PACKET_GEN * (RAW_MODULE_SIZE * 2 / 64));
#endif

    // Read and forward packet #0
    data_in >> packet_in;
    ap_uint<5> modules                   = ACT_REG_NMODULES(packet_in.data);
    ap_uint<64> mode                     = ACT_REG_MODE(packet_in.data);
    ap_uint<1> internal_packet_generator = (mode & MODE_INTERNAL_PACKET_GEN) ? 1 : 0;
    ap_uint<32> frames_per_trigger       = ACT_REG_FRAMES_PER_TRIGGER(packet_in.data);
    ap_uint<5>  storage_cells            = ACT_REG_NSTORAGE_CELLS(packet_in.data);
    ap_uint<1> use_8kB_addr              = (mode & MODE_INTERNAL_PACKET_GEN_4KB) ? 0 : 1;
    ap_uint<1> conversion                = (mode & MODE_CONV) ? 1 : 0;

    data_out << packet_in;
    ap_uint<ADDR_STREAM_WIDTH> addr;
    addr_in >> addr;
    addr_out << addr;

    if (conversion) {
        forward_gain:
        for (int i = 0; i < modules * (3 + storage_cells * 3) * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            data_out << packet_in;
        }
    }

    for (int i = 0; i < MODULES_INTERNAL_PACKET_GEN * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
        data_in >> packet_in;
        module_cache[i] = packet_in.data;
    }
    if (internal_packet_generator) {
        generate_frames:
        for (uint32_t frame_number = 1; frame_number <= frames_per_trigger; frame_number++ ) {
            for (uint8_t module = 0; module < modules; module++) {
                ap_uint<1> cancel = in_cancel;
                if (cancel)
                    break;

                for (uint32_t i = 0; i < RAW_MODULE_SIZE / 2048 * 64; i++) {
#pragma HLS PIPELINE II=1
                    uint32_t eth_packet = i / 64;
                    uint32_t axis_packet = i % 64;
                    if (axis_packet == 0)
                        addr_out << addr_packet(eth_packet, module, frame_number, 0, 0xABCDEF, 0, use_8kB_addr);
                    packet_out.user = 0;
                    packet_out.id   = 0;
                    packet_out.last = (axis_packet == 63) ? 1 : 0;
                    packet_out.data = module_cache[(module % MODULES_INTERNAL_PACKET_GEN) * (RAW_MODULE_SIZE / 32)
                                                   + eth_packet * 64 + axis_packet];
                    data_out << packet_out;
                }
            }
        }
    }

    addr_in >> addr;

    forward_packets:
    while (!addr_last_flag(addr)) {
#pragma HLS PIPELINE II=1
        data_in >> packet_in;
        data_out << packet_in;
        if (packet_in.last) {
            addr_out << addr;
            addr_in >> addr;
        }
    }

    addr_out << addr;
    data_in >> packet_in;
    data_out << packet_in;
}
