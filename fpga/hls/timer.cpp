// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#ifdef __SYNTHESIS__
#include "ap_utils.h"
#else
#define ap_wait()
#endif

#include "hls_jfjoch.h"

#define HBM_CALIB_DELAY 256

void timer_hbm(STREAM_512 &data_in, STREAM_512 &data_out, uint64_t &counter) {
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE register ap_vld port=counter
#pragma HLS INTERFACE ap_ctrl_none port=return
	packet_512_t packet_in;

	// Read and forward packet #0
    data_in >> packet_in;
    uint64_t counter_internal = 0;
    counter = 0; // Counter is regenerated when action is starting, not earlier
    ap_uint<5> modules = ACT_REG_NMODULES(packet_in.data);
    ap_uint<5> storage_cells = ACT_REG_NSTORAGE_CELLS(packet_in.data);
    ap_uint<1> conversion    = (ACT_REG_MODE(packet_in.data) & MODE_CONV) ? 1 : 0;

    data_out << packet_in;

    // Don't care about pipeline stalls when gain & pedestal are transferred

    if (conversion) {
        forward_calibration:
        for (int i = 0; i < modules * (3 + 3 * storage_cells) * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            data_out << packet_in;
        }
    }

    for (int i = 0; i < HBM_CALIB_DELAY; i++) {
        ap_wait();
    }

	data_in >> packet_in;

	while (!packet_in.user) {
#pragma HLS PIPELINE
		if (data_out.full()) {
            if (counter_internal < UINT64_MAX)
                counter_internal++;
            counter = counter_internal;
        } else {
			data_out << packet_in;
			data_in >> packet_in;
		}
    }
	data_out << packet_in;
}

void timer_host(STREAM_512 &data_in, STREAM_512 &data_out, uint64_t &counter) {
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE register ap_vld port=counter
#pragma HLS INTERFACE ap_ctrl_none port=return
    packet_512_t packet_in;

    data_in >> packet_in;
    uint64_t counter_internal = 0;
    counter = 0;
    data_out << packet_in;

    data_in >> packet_in;

    while (!packet_in.user) {
#pragma HLS PIPELINE
        if (data_out.full()) {
            if (counter_internal < UINT64_MAX)
                counter_internal++;
            counter = counter_internal;
        } else {
            data_out << packet_in;
            data_in >> packet_in;
        }
    }
    data_out << packet_in;
}
