// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include <iostream>
#include "hls_jfjoch.h"

void data_collection_fsm(AXI_STREAM &eth_in,
                STREAM_512 &data_out,
                hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out,
                volatile ap_uint<1> &in_run,
                volatile ap_uint<1> &in_cancel,
                volatile ap_uint<1> &out_idle,
                ap_uint<32> mode,
                ap_uint<32> one_over_energy,
                ap_uint<32> frames_per_trigger,
                ap_uint<8>  nmodules,
                ap_uint<4>  nstorage_cells) {
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE axis register both port=eth_in
#pragma HLS INTERFACE axis register both port=data_out
#pragma HLS INTERFACE axis register both port=addr_in
#pragma HLS INTERFACE axis register both port=addr_out
#pragma HLS INTERFACE ap_none register port=in_run
#pragma HLS INTERFACE ap_none register port=in_cancel
#pragma HLS INTERFACE ap_none register port=out_idle

#pragma HLS INTERFACE ap_none register port=mode
#pragma HLS INTERFACE ap_none register port=one_over_energy
#pragma HLS INTERFACE ap_none register port=frames_per_trigger
#pragma HLS INTERFACE ap_none register port=nmodules
#pragma HLS INTERFACE ap_none register port=nstorage_cells

#pragma HLS PIPELINE II=1 style=flp

    packet_512_t packet_in;
    packet_512_t packet_out;
    ap_uint<ADDR_STREAM_WIDTH> addr;

    enum rcv_state_t {RCV_WAIT_FOR_START = 0, RCV_WAIT_FOR_START_LOW = 1, RCV_START = 2, RCV_INIT = 3, RCV_GOOD = 4, RCV_FLUSH = 5, RCV_LAST = 6, RCV_FLUSH_IDLE = 7};
    static rcv_state_t rcv_state = RCV_WAIT_FOR_START;
    static ap_uint<8> axis_packet = 0; // used in for loops, so need one extra bit to allow loop termination

#pragma HLS RESET variable=rcv_state

    ap_uint<1> run;

    switch (rcv_state) {
        case RCV_WAIT_FOR_START:
            if (in_run.read() == 1) {
                rcv_state = RCV_WAIT_FOR_START_LOW;
                out_idle = 0;
            } else if (!addr_in.empty()) {
                addr_in.read();
                axis_packet = 0;
                rcv_state = RCV_FLUSH_IDLE;
                out_idle = 1;
            } else
                out_idle = 1;
            break;
        case RCV_FLUSH_IDLE:
            out_idle = 1;
            eth_in >> packet_in;
            if (axis_packet == 127)
                rcv_state = RCV_WAIT_FOR_START;
            else
                axis_packet++;
            break;
        case RCV_WAIT_FOR_START_LOW:
            out_idle = 0;
            if (in_run.read() == 0)
                rcv_state = RCV_START;
            break;
        case RCV_START:
            out_idle = 0;
            packet_out.data = 0;
            ACT_REG_MODE(packet_out.data) = mode;
            ACT_REG_ONE_OVER_ENERGY(packet_out.data) = one_over_energy;
            ACT_REG_FRAMES_PER_TRIGGER(packet_out.data) = frames_per_trigger;
            ACT_REG_NMODULES(packet_out.data) = nmodules;
            ACT_REG_NSTORAGE_CELLS(packet_out.data) = nstorage_cells + 1;
            packet_out.user = 0;
            packet_out.last = 0;
            packet_out.dest = 0;
            packet_out.id   = 1;
            data_out << packet_out;

            addr = 0;
            addr(63, 0)  = mode;
            addr(79,64)  = nmodules;
            addr_out << addr;

            if (mode & MODE_INTERNAL_PACKET_GEN)
                rcv_state = RCV_LAST;
            else
                rcv_state = RCV_INIT;
            break;
        case RCV_INIT:
            out_idle = 0;
            if (in_cancel.read() == 1)
                rcv_state = RCV_LAST;
            else if (!addr_in.empty()) {
                addr_in >> addr;
                axis_packet = 0;
                rcv_state = RCV_GOOD;
                if (addr_frame_number(addr) == UINT64_MAX) {
                    rcv_state = RCV_FLUSH;
                } else {
                    addr_out << addr;
                }
            }
            break;
        case RCV_GOOD:
            out_idle = 0;
            eth_in >> packet_in;
            packet_in.user = 0;
            packet_in.id   = 0;
            data_out << packet_in;
            if (axis_packet == 127)
                rcv_state = RCV_INIT;
            else
                axis_packet++;
            break;
        case RCV_FLUSH:
            out_idle = 0;
            eth_in >> packet_in;
            if (axis_packet == 127)
                rcv_state = RCV_LAST;
            else
                axis_packet++;
            break;
        case RCV_LAST:
            out_idle = 0;
            addr = 0;
            addr_last_flag(addr) = 1;
            addr_out << addr;

            // Finish data collection
            packet_out.data = 0;
            packet_out.dest = 0;
            packet_out.id   = 1; // Special packet
            packet_out.last = 1;
            packet_out.user = 1;
            data_out << packet_out;
            rcv_state = RCV_WAIT_FOR_START;
            break;
    }
}

