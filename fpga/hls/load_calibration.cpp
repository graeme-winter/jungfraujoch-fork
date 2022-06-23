// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

void load_calibration(STREAM_512 &data_in, STREAM_512 &data_out,
                      hls::stream<axis_datamover_ctrl> &datamover_in_cmd,
                      hls::stream<ap_axiu<512,1,1,1> > &host_memory_in,
                      uint64_t in_mem_location[LOAD_CALIBRATION_BRAM_SIZE]) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE register both axis port=datamover_in_cmd
#pragma HLS INTERFACE register both axis port=host_memory_in
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE bram port=in_mem_location storage_type=rom_1p

    packet_512_t packet_in;

    data_in >> packet_in;
    ap_uint<16> modules = ACT_REG_NMODULES(packet_in.data);
    data_out << packet_in;

    process_modules:
    for (int c = 0; c < 6; c++) {
        // 6 calibration stages
        for (int m = 0; m < modules ; m++) {
            setup_datamover(datamover_in_cmd, in_mem_location[m + c * MAX_MODULES_FPGA], RAW_MODULE_SIZE * 2);
            read_calibration1:
            for (int j = 0; j < (RAW_MODULE_SIZE * sizeof(int16_t) / 64); j++) {
#pragma HLS PIPELINE II=1
                ap_axiu<512, 1, 1, 1> data_packet;
                host_memory_in >> data_packet;
                packet_512_t packet_out;
                packet_out.last = 0;
                packet_out.user = 0;
                packet_out.id = 0;
                packet_out.data = data_packet.data;
                data_out << packet_out;
            }
        }
    }

    for (int i = 0; i < MODULES_INTERNAL_PACKET_GEN ; i++) {
        setup_datamover(datamover_in_cmd, in_mem_location[i + 6 * MAX_MODULES_FPGA],RAW_MODULE_SIZE * 2);
        read_calibration2:
        for (int j = 0; j < (RAW_MODULE_SIZE * sizeof(int16_t) / 64); j++) {
#pragma HLS PIPELINE II=1
            ap_axiu<512,1,1,1> data_packet;
            host_memory_in >> data_packet;
            packet_512_t packet_out;
            packet_out.last = 0;
            packet_out.user = 0;
            packet_out.id   = 0;
            packet_out.data = data_packet.data;
            data_out << packet_out;
        }
    }

    data_in >> packet_in;
    while (!packet_in.user) {
#pragma HLS PIPELINE II=1
        data_out << packet_in;
        data_in >> packet_in;
    }
    data_out << packet_in;
}