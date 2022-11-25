// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "hls_jfjoch.h"

void data_and_cmd_stream_sync(hls::stream<ap_axiu<512,1,1,1> > &data_in,
                              hls::stream<ap_axiu<512,1,1,1> > &data_out,
                              hls::stream<axis_datamover_ctrl> &cmd_in,
                              hls::stream<axis_datamover_ctrl> &cmd_out) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE register both axis port=cmd_in
#pragma HLS INTERFACE register both axis port=cmd_out
#pragma HLS PIPELINE II=1 style=flp

    ap_axiu<512,1,1,1> packet_in, packet_out;

    data_in >> packet_in;

    packet_out.keep = UINT64_MAX;
    packet_out.strb = UINT64_MAX;
    packet_out.dest = 0;
    packet_out.id = 0;
    packet_out.user = 0;
    packet_out.last = packet_in.last;
    packet_out.data = packet_in.data;

    data_out << packet_out;

    if (packet_in.last) {
        axis_datamover_ctrl cmd;
        cmd_in >> cmd;
        cmd_out << cmd;
    }
}
