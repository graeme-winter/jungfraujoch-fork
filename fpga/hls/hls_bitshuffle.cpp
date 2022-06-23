// Copyright (2019-2021) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#define AP_INT_MAX_W 2048
#include "hls_jfjoch.h"

struct bshuf_packet {
    ap_uint<512> data;
    ap_uint<3> dest;
    ap_uint<1> id;
    ap_uint<1> last;
    ap_uint<1> user;
};

typedef hls::stream<bshuf_packet> BSHUF_STREAM;

template<int N> ap_uint<16*N> bshuf16(const ap_uint<16*N> in) {
#pragma HLS INLINE
    ap_uint<16*N> out;
    for (int i = 0; i < N * 16; i++)
        out[(i%16) * N + (i/16)] = in[i];
    return out;
}

void bshuf256(STREAM_512 &data_in, BSHUF_STREAM &data_out) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=data_in
#pragma HLS INTERFACE axis port=data_out
    packet_512_t packet_in[4];
    bshuf_packet packet_out[4];

    data_in >> packet_in[0];
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in[0].data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    packet_out[0].data = packet_in[0].data;
    packet_out[0].id   = packet_in[0].id;
    packet_out[0].last = packet_in[0].last;
    packet_out[0].user = packet_in[0].user;
    packet_out[0].dest = 0;

    data_out << packet_out[0];

    data_in >> packet_in[0];

    ap_uint<2> counter = 0;

    if (shuffle)
        bitshuffle_loop:
        while (!packet_in[0].user) {
#pragma HLS PIPELINE II=4

            for (int i = 1; i < 4; i++)
                data_in >> packet_in[i];

            ap_uint<2048> tmp_reg_in = (packet_in[3].data, packet_in[2].data, packet_in[1].data, packet_in[0].data);
            ap_uint<2048> tmp_reg_out = bshuf16<128>(tmp_reg_in);
            for (int i = 0; i < 4; i++) {
                packet_out[i].data = tmp_reg_out(512*i+511,512*i);
                packet_out[i].dest = counter;
                packet_out[i].last = packet_in[i].last;
                packet_out[i].user = packet_in[i].user;
                packet_out[i].id   = packet_in[i].id;
                data_out << packet_out[i];
            }
            counter++;
            data_in >> packet_in[0];
        }

    forward_frames:
    while (!packet_in[0].user) {
#pragma HLS PIPELINE II=1
        packet_out[0].dest = 0;
        packet_out[0].data = packet_in[0].data;
        packet_out[0].last = packet_in[0].last;
        packet_out[0].user = packet_in[0].user;
        packet_out[0].id   = packet_in[0].id;
        data_out << packet_out[0];
        data_in >> packet_in[0];
    }
    packet_out[0].dest = 0;
    packet_out[0].data = packet_in[0].data;
    packet_out[0].last = packet_in[0].last;
    packet_out[0].user = packet_in[0].user;
    packet_out[0].id   = packet_in[0].id;
    data_out << packet_out[0];

}

void bshuf1k_axis_split(BSHUF_STREAM &in, BSHUF_STREAM &out_0, BSHUF_STREAM &out_1, BSHUF_STREAM &out_2, BSHUF_STREAM &out_3) {
    bshuf_packet packet_in;
    in >> packet_in;
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in.data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    out_0 << packet_in;

    in >> packet_in;

    forward_frames:
    while (!packet_in.user) {
#pragma HLS PIPELINE II=1
        if (packet_in.dest == 1)
            out_1 << packet_in;
        else if (packet_in.dest == 2)
            out_2 << packet_in;
        else if (packet_in.dest == 3)
            out_3 << packet_in;
        else
            out_0 << packet_in;
        in >> packet_in;
    }
    out_0 << packet_in;
}

void bshuf1k_axis_combine(BSHUF_STREAM &in_0, BSHUF_STREAM &in_1, BSHUF_STREAM &in_2, BSHUF_STREAM &in_3,
                          BSHUF_STREAM &out) {
    bshuf_packet packet_in;
    in_0 >> packet_in;
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in.data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    out << packet_in;

    in_0 >> packet_in;

    if (shuffle)
        bitshuffle_loop:
        while (!packet_in.user) {
#pragma HLS PIPELINE II=4
            out << packet_in;

            in_1 >> packet_in;
            out << packet_in;

            in_2 >> packet_in;
            out << packet_in;

            in_3 >> packet_in;
            out << packet_in;

            in_0 >> packet_in;
        }

    forward_frames:
    while (!packet_in.user) {
#pragma HLS PIPELINE II=1
        out << packet_in;
        in_0 >> packet_in;
    }
    out << packet_in;
}

void bshuf1k_shuffle(BSHUF_STREAM &in, BSHUF_STREAM &out) {
    bshuf_packet packet_in[4];
    in >> packet_in[0];
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in[0].data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    out << packet_in[0];

    in >> packet_in[0];

    ap_uint<5> counter = 0;

    if (shuffle)
        bitshuffle_loop:
        while (!packet_in[0].user) {
#pragma HLS PIPELINE II=4

            for (int i = 1; i < 4; i++)
                in >> packet_in[i];

            ap_uint<2048> tmp_reg_in = (packet_in[3].data, packet_in[2].data, packet_in[1].data, packet_in[0].data);
            ap_uint<2048> tmp_reg_out;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    tmp_reg_out(512 * i + 128 * j + 127, 512*i + 128 * j) = packet_in[j].data(128*i+127, 128*i);
                }
            }

            for (int i = 0; i < 4; i++) {
                packet_in[i].data = tmp_reg_out(512*i+511,512*i);
                packet_in[i].dest = (counter / 4) % 8;
                out << packet_in[i];
            }
            counter += 1;

            in >> packet_in[0];
        }

    forward_frames:
    while (!packet_in[0].user) {
#pragma HLS PIPELINE II=1
        packet_in[0].dest = 0;
        out << packet_in[0];
        in >> packet_in[0];
    }
    packet_in[0].dest = 0;
    out << packet_in[0];
}

void bshuf8k_axis_split(BSHUF_STREAM &in,
                        BSHUF_STREAM &out_0,
                        BSHUF_STREAM &out_1,
                        BSHUF_STREAM &out_2,
                        BSHUF_STREAM &out_3,
                        BSHUF_STREAM &out_4,
                        BSHUF_STREAM &out_5,
                        BSHUF_STREAM &out_6,
                        BSHUF_STREAM &out_7) {
    bshuf_packet packet_in;
    in >> packet_in;
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in.data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    out_0 << packet_in;

    in >> packet_in;

    forward_frames:
    while (!packet_in.user) {
#pragma HLS PIPELINE II=1
        if (packet_in.dest == 1)
            out_1 << packet_in;
        else if (packet_in.dest == 2)
            out_2 << packet_in;
        else if (packet_in.dest == 3)
            out_3 << packet_in;
        else if (packet_in.dest == 4)
            out_4 << packet_in;
        else if (packet_in.dest == 5)
            out_5 << packet_in;
        else if (packet_in.dest == 6)
            out_6 << packet_in;
        else if (packet_in.dest == 7)
            out_7 << packet_in;
        else
            out_0 << packet_in;
        in >> packet_in;
    }
    out_0 << packet_in;
}

void bshuf8k_axis_combine(BSHUF_STREAM &in_0, BSHUF_STREAM &in_1, BSHUF_STREAM &in_2, BSHUF_STREAM &in_3,
                          BSHUF_STREAM &in_4, BSHUF_STREAM &in_5, BSHUF_STREAM &in_6, BSHUF_STREAM &in_7,
                          STREAM_512 &out) {
    bshuf_packet packet_in;
    packet_512_t packet_out;

    in_0 >> packet_in;
    ap_uint<1> shuffle = (ACT_REG_MODE(packet_in.data) & MODE_BITSHUFFLE_FPGA) ? 1 : 0;
    packet_out.data = packet_in.data;
    packet_out.last = packet_in.last;
    packet_out.user = packet_in.user;
    packet_out.id   = packet_in.id;
    out << packet_out;

    in_0 >> packet_in;

    if (shuffle)
        bitshuffle_loop:
        while (!packet_in.user) {
#pragma HLS PIPELINE II=8
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_1 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_2 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_3 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_4 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_5 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_6 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_7 >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            packet_out.user = packet_in.user;
            packet_out.id   = packet_in.id;
            out << packet_out;

            in_0 >> packet_in;
        }

    forward_frames:
    while (!packet_in.user) {
#pragma HLS PIPELINE II=1
        packet_out.data = packet_in.data;
        packet_out.last = packet_in.last;
        packet_out.user = packet_in.user;
        packet_out.id   = packet_in.id;
        out << packet_out;
        in_0 >> packet_in;
    }
    packet_out.data = packet_in.data;
    packet_out.last = packet_in.last;
    packet_out.user = packet_in.user;
    packet_out.id   = packet_in.id;
    out << packet_out;
}

void bitshuffle(STREAM_512 &data_in, STREAM_512 &data_out) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis register both port=data_in
#pragma HLS INTERFACE axis register both port=data_out
#pragma HLS DATAFLOW

    BSHUF_STREAM fifo_0;
#pragma HLS STREAM variable=fifo_0 depth=3
#pragma HLS RESOURCE variable=fifo_0 core=FIFO_SRL
    BSHUF_STREAM fifo_1_0;
#pragma HLS STREAM variable=fifo_1_0 depth=8
#pragma HLS RESOURCE variable=fifo_1_0 core=FIFO_SRL
    BSHUF_STREAM fifo_1_1;
#pragma HLS STREAM variable=fifo_1_1 depth=8
#pragma HLS RESOURCE variable=fifo_1_1 core=FIFO_SRL
    BSHUF_STREAM fifo_1_2;
#pragma HLS STREAM variable=fifo_1_2 depth=8
#pragma HLS RESOURCE variable=fifo_1_2 core=FIFO_SRL
    BSHUF_STREAM fifo_1_3;
#pragma HLS STREAM variable=fifo_1_3 depth=8
#pragma HLS RESOURCE variable=fifo_1_3 core=FIFO_SRL
    BSHUF_STREAM fifo_2;
#pragma HLS STREAM variable=fifo_2 depth=3
#pragma HLS RESOURCE variable=fifo_2 core=FIFO_SRL
    BSHUF_STREAM fifo_3;
#pragma HLS STREAM variable=fifo_3 depth=3
#pragma HLS RESOURCE variable=fifo_3 core=FIFO_SRL
    BSHUF_STREAM fifo_4_0;
#pragma HLS STREAM variable=fifo_4_0 depth=32
    BSHUF_STREAM fifo_4_1;
#pragma HLS STREAM variable=fifo_4_1 depth=32
    BSHUF_STREAM fifo_4_2;
#pragma HLS STREAM variable=fifo_4_2 depth=32
    BSHUF_STREAM fifo_4_3;
#pragma HLS STREAM variable=fifo_4_3 depth=32
    BSHUF_STREAM fifo_4_4;
#pragma HLS STREAM variable=fifo_4_4 depth=32
    BSHUF_STREAM fifo_4_5;
#pragma HLS STREAM variable=fifo_4_5 depth=32
    BSHUF_STREAM fifo_4_6;
#pragma HLS STREAM variable=fifo_4_6 depth=32
    BSHUF_STREAM fifo_4_7;
#pragma HLS STREAM variable=fifo_4_7 depth=32

    bshuf256(data_in, fifo_0);
    bshuf1k_axis_split(fifo_0, fifo_1_0, fifo_1_1, fifo_1_2, fifo_1_3);
    bshuf1k_axis_combine(fifo_1_0, fifo_1_1, fifo_1_2, fifo_1_3, fifo_2);
    bshuf1k_shuffle(fifo_2, fifo_3);
    bshuf8k_axis_split(fifo_3,
                       fifo_4_0, fifo_4_1, fifo_4_2, fifo_4_3,
                       fifo_4_4, fifo_4_5, fifo_4_6, fifo_4_7);
    bshuf8k_axis_combine(fifo_4_0, fifo_4_1, fifo_4_2, fifo_4_3,
                         fifo_4_4, fifo_4_5, fifo_4_6, fifo_4_7,
                         data_out);
}
