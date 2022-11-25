// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

#ifndef __SYNTHESIS__
#include <thread>
#endif

#define PACKET_SIZE 4096

inline void write_completion(hls::stream<ap_uint<32> > &m_axis_completion,
                      const ap_uint<32> &handle,
                      const ap_uint<8> &module,
                      const ap_uint<64> &frame_num,
                      const ap_uint<256> &packet_mask,
                      const ap_uint<16> &packet_count,
                      const ap_uint<32> &debug,
                      const ap_uint<64> &timestamp,
                      const ap_uint<64> &bunchid) {
#pragma HLS INLINE
    ap_uint<1> all_packets_ok = packet_mask.and_reduce();
    ap_uint<1> any_packets_received = packet_mask.or_reduce();
    ap_uint<8> status = 0;
    status[0] = all_packets_ok;
    status[1] = any_packets_received;

    ap_uint<128> tmp = (handle, packet_count, status, module, frame_num);
    status[7] = tmp.xor_reduce(); // ensure completion has even parity

    m_axis_completion << handle;
    m_axis_completion << (packet_count, status, module);
    m_axis_completion << frame_num(63, 32);
    m_axis_completion << frame_num(31,  0);

    m_axis_completion << debug;
    m_axis_completion << timestamp(31,0);
    m_axis_completion << bunchid(63,32);
    m_axis_completion << bunchid(31,0);

    if (!all_packets_ok) {
        m_axis_completion << packet_mask(128+127,128+96);
        m_axis_completion << packet_mask( 128+95,128+64);
        m_axis_completion << packet_mask( 128+63,128+32);
        m_axis_completion << packet_mask( 128+31, 128+0);

        m_axis_completion << packet_mask(127,96);
        m_axis_completion << packet_mask( 95,64);
        m_axis_completion << packet_mask( 63,32);
        m_axis_completion << packet_mask( 31, 0);
    }
}


inline ap_uint<1> read_request(hls::stream<ap_uint<32> > &s_axis_work_request,
                             ap_uint<32> &handle,
                             ap_uint<64> &address) {
#pragma HLS INLINE
    ap_uint<32> tmp1, tmp2, tmp3, tmp4;

    s_axis_work_request >> tmp1;
    s_axis_work_request >> tmp2;
    s_axis_work_request >> tmp3;
    s_axis_work_request >> tmp4;

    handle = tmp1;
    address = (tmp2, tmp3);

    ap_uint<128> tmp_all = (tmp1, tmp2, tmp3, tmp4);

    if (tmp_all.xor_reduce() != 0)
        return 1;
    else
        return 0;
}

void host_writer(STREAM_512 &data_in,
                 hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                 hls::stream<ap_axiu<512,1,1,1> > &host_memory_out,
                 hls::stream<axis_datamover_ctrl> &datamover_out_cmd,
                 hls::stream<ap_uint<32> > &s_axis_work_request,
                 hls::stream<ap_uint<32> > &m_axis_completion,
                 volatile uint64_t &packets_processed,
                 ap_uint<8> &err_reg) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=host_memory_out
#pragma HLS INTERFACE register both axis port=addr_in
#pragma HLS INTERFACE register both axis port=datamover_out_cmd
#pragma HLS INTERFACE register both axis port=m_axis_completion
#pragma HLS INTERFACE register both axis port=s_axis_work_request
#pragma HLS INTERFACE register ap_vld port=packets_processed
#pragma HLS INTERFACE register ap_vld port=err_reg

    ap_uint<ADDR_STREAM_WIDTH> addr;
    addr_in >> addr;

    packet_512_t packet_in;
    data_in >> packet_in;

    ap_uint<8> internal_err_reg = 0;
    err_reg = internal_err_reg;

    ap_uint<256> packet_mask[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=packet_mask core=RAM_1P
    ap_uint<16> packet_count[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=packet_count core=RAM_1P
    ap_uint<32> handle[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=handle core=RAM_1P
    ap_uint<64> curr_frame[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=curr_frame core=RAM_1P
    ap_uint<32> debug[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=debug core=RAM_1P
    ap_uint<64> timestamp[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=timestamp core=RAM_1P
    ap_uint<64> jf_bunchid[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=jf_bunchid core=RAM_1P
    ap_uint<64> curr_offset[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=curr_offset core=RAM_1P

    for (int i = 0; i < MAX_MODULES_FPGA*2; i++) {
#pragma HLS UNROLL
        curr_frame[i]          = UINT64_MAX;
        handle[i]              = 0;
        packet_mask[i]         = 0;
        packet_count[i]        = 0;
        curr_offset[i]         = 0;
        debug[i]            = 0;
        timestamp[i]        = 0;
        jf_bunchid[i]          = 0;
    }

    uint64_t total_counter = 0;
    packets_processed = 0;
    addr_in >> addr;

    ap_axiu<512,1,1,1> packet_out;
    packet_out.keep = UINT64_MAX;
    packet_out.strb = UINT64_MAX;
    packet_out.dest = 0;
    packet_out.id = 0;
    packet_out.user = 0;

    ap_uint<32> req_handle;
    ap_uint<64> req_host_offset;

    Loop_good_packet:
    while (!addr_last_flag(addr)) {
        // Process one UDP packet per iteration
#pragma HLS PIPELINE II=64
        ap_uint<64> frame_number = addr_frame_number(addr);
        ap_uint<4> module = addr_module(addr);
        ap_uint<8> eth_packet = addr_eth_packet(addr);
        ap_uint<5> id = module * 2 + (frame_number % 2);
        ap_uint<1> flag_8kB = addr_flag_8kb(addr);
        ap_uint<1> first_packet_8kB = flag_8kB & (eth_packet % 2 == 0);

        for (int i = 0; i < 64; i++) {
            data_in >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = (!first_packet_8kB) & packet_in.last;
            host_memory_out << packet_out;
        }

        if (packet_in.last != 1)
            internal_err_reg[1] = 1;

        if (curr_frame[id] != frame_number) {
            if (packet_mask[id] != 0) {
                ap_uint<32>  comp_handle       = handle[id];
                ap_uint<64>  comp_frame        = curr_frame[id];
                ap_uint<256> comp_packet_mask  = packet_mask[id];
                ap_uint<16>  comp_packet_count = packet_count[id];
                ap_uint<32>  comp_debug        = debug[id];
                ap_uint<32>  comp_timestamp    = timestamp[id];
                ap_uint<32>  comp_bunchid      = jf_bunchid[id];

                write_completion(m_axis_completion, comp_handle, module,
                                 comp_frame, comp_packet_mask, comp_packet_count,
                                 comp_debug, comp_timestamp, comp_bunchid);
            }

            if (read_request(s_axis_work_request, req_handle, req_host_offset))
                internal_err_reg[2] = 1;

            if (req_handle == UINT32_MAX)
                internal_err_reg[4] = 1;

            handle[id]       = req_handle;
            curr_frame[id]   = frame_number;
            curr_offset[id]  = req_host_offset;

            debug[id]        = addr_jf_debug(addr);
            timestamp[id]    = addr_timestamp(addr);
            jf_bunchid[id]   = addr_bunch_id(addr);

            packet_mask[id]  = ap_uint<256>(1) << eth_packet/2 + ((eth_packet % 2) ? 128:0);
            packet_count[id] = 1;
        } else {
            packet_count[id]++;
            packet_mask[id] |= ap_uint<256>(1) << eth_packet / 2 + ((eth_packet % 2) ? 128 : 0);
        }

        size_t out_frame_addr = curr_offset[id] + eth_packet * PACKET_SIZE;
        if (out_frame_addr % 128 != 0) internal_err_reg[0] = 1;
        if (curr_offset[id] == 0) internal_err_reg[3] = 1;

        packets_processed = ++total_counter;

        if (first_packet_8kB)
            setup_datamover(datamover_out_cmd, out_frame_addr, 2 * PACKET_SIZE);
        else if (!flag_8kB)
            setup_datamover(datamover_out_cmd, out_frame_addr, PACKET_SIZE);

        addr_in >> addr;
        err_reg = internal_err_reg;
    }

#ifndef __SYNTHESIS__
    while (!host_memory_out.empty())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif

    for (ap_uint<8> m = 0; m < MAX_MODULES_FPGA * 2; m++) {
#pragma HLS PIPELINE II=16
        if (packet_mask[m] > 0)
            write_completion(m_axis_completion, handle[m], m / 2, curr_frame[m],
                             packet_mask[m], packet_count[m],
                             debug[m], timestamp[m], jf_bunchid[m]);
    }

    data_in >> packet_in;

    m_axis_completion << UINT32_MAX;
    m_axis_completion << total_counter;      // packets_ok
    m_axis_completion << 0;                  // fill to 128 bit
    m_axis_completion << 0;                  // fill to 128 bit

    read_request(s_axis_work_request, req_handle, req_host_offset);

    while (req_handle != UINT32_MAX) {
#pragma HLS PIPELINE II=4
        read_request(s_axis_work_request, req_handle, req_host_offset);
    }
}
