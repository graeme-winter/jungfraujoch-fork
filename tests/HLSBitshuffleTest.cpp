// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <bitshuffle/bitshuffle.h>

#include "../fpga/host/HLSSimulatedDevice.h"

TEST_CASE("HLS_bitshuffle") {
    STREAM_512 data_in;
    STREAM_512 data_out;

    packet_512_t packet_in, packet_out;

    ACT_REG_MODE(packet_in.data) = MODE_BITSHUFFLE_FPGA;
    data_in << packet_in;
    packet_in.user = 0;
    packet_in.last = 0;

    ap_int<16> image[4096];

    for (int i = 0; i < 4096; i++)
        image[i] = i;

    for (int i = 0; i < 128; i++) {
        packet_in.data = pack32(image + i * 32);
        data_in << packet_in;
    }

    packet_in.user = 1;
    data_in << packet_in;

    bitshuffle(data_in, data_out);

    REQUIRE(data_out.size() == 130);
    data_out >> packet_out;

    ap_int<16> image_out_shuf[128*32];
    ap_int<16> image_out[128*32];
    for (int i = 0; i < 128; i++) {
        data_out >> packet_out;
        unpack32(packet_out.data, image_out_shuf + 32 * i);
    }
    bshuf_bitunshuffle(image_out_shuf, image_out, 128*32, 2, 128*32);

    int diff = 0;
    for (int i = 0; i < 128*32; i++) {
        if (image_out[i] != image[i]) diff++;
    }
    REQUIRE(diff == 0);
}
