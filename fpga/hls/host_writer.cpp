// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

#ifndef __SYNTHESIS__
#include <thread>
#endif

inline void write_completion(hls::stream<ap_uint<32> > &m_axis_completion,
                      const ap_uint<32> &handle,
                      const ap_uint<8> &module,
                      const ap_uint<1> &trigger,
                      const ap_uint<64> &frame_num,
                      const ap_uint<128> &packet_counter,
                      const ap_uint<32> &jf_debug,
                      const ap_uint<64> &timestamp,
                      const ap_uint<64> &bunchid) {
#pragma HLS INLINE
    ap_uint<16> fill16 = 0;
    ap_uint<1> all_packets_ok = packet_counter.and_reduce();
    ap_uint<8> status = 0;
    status[0] = all_packets_ok;
    status[1] = trigger;

    ap_uint<128> tmp = (handle, fill16, status, module, frame_num);
    status[7] = tmp.xor_reduce(); // ensure completion has even parity

    m_axis_completion << handle;
    m_axis_completion << (fill16, status, module);
    m_axis_completion << frame_num(63, 32);
    m_axis_completion << frame_num(31,  0);

    m_axis_completion << jf_debug;
    m_axis_completion << timestamp(31,0);
    m_axis_completion << bunchid(63,32);
    m_axis_completion << bunchid(31,0);

    if (!all_packets_ok) {
        m_axis_completion << packet_counter(127,96);
        m_axis_completion << packet_counter( 95,64);
        m_axis_completion << packet_counter( 63,32);
        m_axis_completion << packet_counter( 31, 0);
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
                 ap_uint<4> &err_reg) {
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

    ap_uint<4> internal_err_reg = 0;
    err_reg = internal_err_reg;

    ap_uint<128> packet_counter[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=packet_counter core=RAM_1P
    ap_uint<32> handle[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=handle core=RAM_1P
    ap_uint<64> curr_frame[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=curr_frame core=RAM_1P
    ap_uint<32> jf_debug[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=jf_debug core=RAM_1P
    ap_uint<64> jf_timestamp[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=jf_timestamp core=RAM_1P
    ap_uint<64> jf_bunchid[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=jf_bunchid core=RAM_1P
    ap_uint<64> curr_offset[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=curr_offset core=RAM_1P
    ap_uint<1> trigger[MAX_MODULES_FPGA*2];
#pragma HLS RESOURCE variable=trigger core=RAM_1P

    for (int i = 0; i < MAX_MODULES_FPGA*2; i++) {
#pragma HLS UNROLL
        curr_frame[i]          = UINT64_MAX;
        handle[i]              = 0;
        packet_counter[i]      = 0;
        trigger[i]             = 0;
        curr_offset[i]         = 0;
        jf_debug[i]            = 0;
        jf_timestamp[i]        = 0;
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

    Loop_good_packet:
    while (!addr_last_flag(addr)) {
        // Process one UDP packet per iteration
#pragma HLS PIPELINE II=128
        ap_uint<64> frame_number = addr_frame_number(addr);
        ap_uint<4> module = addr_module(addr);
        ap_uint<7> eth_packet = addr_eth_packet(addr);
        ap_uint<5> id = module * 2 + (frame_number % 2);

        for (int i = 0; i < 128; i++) {
            data_in >> packet_in;
            packet_out.data = packet_in.data;
            packet_out.last = packet_in.last;
            host_memory_out << packet_out;
        }

        if (packet_in.last != 1)
            internal_err_reg[1] = 1;

        if (curr_frame[id] != frame_number) {
            if (packet_counter[id] != 0) {
                ap_uint<32>  comp_handle      = handle[id];
                ap_uint<1>   comp_trigger     = trigger[id];
                ap_uint<64>  comp_frame       = curr_frame[id];
                ap_uint<128> comp_packet_mask = packet_counter[id];
                ap_uint<32>  comp_debug       = jf_debug[id];
                ap_uint<32>  comp_timestamp   = jf_timestamp[id];
                ap_uint<32>  comp_bunchid     = jf_bunchid[id];

                write_completion(m_axis_completion, comp_handle, module, comp_trigger,
                                 comp_frame, comp_packet_mask,
                                 comp_debug, comp_timestamp, comp_bunchid);
            }
            ap_uint<32> req_handle;
            ap_uint<64> req_host_offset;

            if (read_request(s_axis_work_request, req_handle, req_host_offset))
                internal_err_reg[2] = 1;

            handle[id]       = req_handle;
            curr_frame[id]   = frame_number;
            curr_offset[id]  = req_host_offset;

            trigger[id]      = addr_trigger_flag(addr);

            jf_debug[id]     = addr_jf_debug(addr);
            jf_timestamp[id] = addr_timestamp(addr);
            jf_bunchid[id]   = addr_bunch_id(addr);

            packet_counter[id] = ap_uint<128>(1) << eth_packet;
        } else
            packet_counter[id] |= ap_uint<128>(1) << eth_packet;

        size_t out_frame_addr = curr_offset[id] + eth_packet * (4 * RAW_MODULE_COLS * sizeof(int16_t));
        if (out_frame_addr % 128 != 0) internal_err_reg[0] = 1;

        packets_processed = ++total_counter;

        setup_datamover(datamover_out_cmd, out_frame_addr, 64 * 128);

        addr_in >> addr;
        err_reg = internal_err_reg;
    }

#ifndef __SYNTHESIS__
    while (!host_memory_out.empty())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif

    for (ap_uint<8> m = 0; m < MAX_MODULES_FPGA * 2; m++) {
#pragma HLS PIPELINE II = 32
        if (packet_counter[m] > 0)
            write_completion(m_axis_completion, handle[m], m / 2, trigger[m], curr_frame[m], packet_counter[m],
                             jf_debug[m], jf_timestamp[m], jf_bunchid[m]);
    }

    data_in >> packet_in;

    m_axis_completion << UINT32_MAX;
    m_axis_completion << total_counter;      // packets_ok
    m_axis_completion << 0;                  // fill to 128 bit
    m_axis_completion << 0;                  // fill to 128 bit

    ap_uint<32> tmp;
    s_axis_work_request >> tmp;
    while (tmp[31] != 1) {
        s_axis_work_request.read();
        s_axis_work_request.read();
        s_axis_work_request.read();
        s_axis_work_request >> tmp;
    }
    s_axis_work_request.read();
    s_axis_work_request.read();
    s_axis_work_request.read();
}
