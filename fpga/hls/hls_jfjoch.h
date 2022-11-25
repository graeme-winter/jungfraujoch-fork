// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#ifndef __HW_ACTION_RX100G_H__
#define __HW_ACTION_RX100G_H__

#include <ap_int.h>
#include <stdint.h>
#include <string.h>

#ifdef __SYNTHESIS__
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_burst_maxi.h>
#else
#include "../include/hls_burst_maxi.h"
#include <parallel_stream.h>
#endif

#include "../../common/Definitions.h"

// Number of modules that can be simultaneously handled by the FPGA
#define MAX_MODULES_FPGA  10

// Number of modules to include in the internal packet generator
#define MODULES_INTERNAL_PACKET_GEN 1

#define ADDR_STREAM_WIDTH 256
#define LOAD_CALIBRATION_BRAM_SIZE 1024

typedef ap_ufixed<16,14> pedeG1G2_t;

typedef ap_ufixed<16,2, AP_RND_CONV> gainG0_t;
typedef ap_ufixed<16,4, AP_RND_CONV> gainG1_t;
typedef ap_ufixed<16,6, AP_RND_CONV> gainG2_t;

typedef ap_ufixed<32,12,AP_RND_CONV> one_over_energy_t;

typedef ap_uint<256> rx100g_hbm_t;

#ifndef __SYNTHESIS__
template<int D,int U,int TI,int TD>
  struct ap_axiu{
    ap_uint<D>       data;
    ap_uint<(D+7)/8> keep;
    ap_uint<(D+7)/8> strb;
    ap_uint<U>       user;
    ap_uint<1>       last;
    ap_uint<TI>      id;
    ap_uint<TD>      dest;
  };
#endif

typedef ap_axiu<512,1, 1, 1> packet_512_t;

typedef hls::stream<packet_512_t> AXI_STREAM;
typedef hls::stream<packet_512_t> STREAM_512;

#define addr_frame_number(x)  x(63,  0)
#define addr_eth_packet(x)    x(71, 64)
#define addr_module(x)        x(76, 72)
#define addr_last_flag(x)     x[79]
#define addr_flag_8kb(x)      x[80]
#define addr_jf_debug(x)      x(127, 96)
#define addr_timestamp(x)     x(191,128)
#define addr_bunch_id(x)      x(255,192)

#define ACT_REG_MODE(x)                   ((x)(63,0))    // 64 bit
#define ACT_REG_ONE_OVER_ENERGY(x)        ((x)(95,64))   // 32 bit
#define ACT_REG_FRAMES_PER_TRIGGER(x)     ((x)(127,96))  // 32 bit
#define ACT_REG_NMODULES(x)               ((x)(132,128)) // 5 bit (0..31)
#define ACT_REG_NSTORAGE_CELLS(x)         ((x)(148,144)) // 5 bit

typedef struct {
    ap_uint<40+64> data;
} axis_datamover_ctrl;

struct packet_header_t {
	ap_uint<48> dest_mac ;
	ap_uint<48> src_mac  ;
	ap_uint<16> ether_type ;

	ap_uint<4> ip_version ;
	ap_uint<4> ipv4_header_len ;
	ap_uint<8> ipv4_protocol  ;
	ap_uint<32> ipv4_total_len ;
	// uint32_t ipv4_header_checksum;
	ap_uint<32> ipv4_source_ip ;
	ap_uint<32> ipv4_dest_ip ;

	ap_uint<16> udp_src_port;
	ap_uint<16> udp_dest_port;
	ap_uint<16> udp_len;
	ap_uint<16> udp_checksum;

	ap_uint<64> jf_frame_number;
	ap_uint<32> jf_exptime;
	ap_uint<32> jf_packet_number;
	ap_uint<64> jf_bunch_id;
	ap_uint<64> jf_timestamp;
	ap_uint<16> jf_module_id;
	ap_uint<16> jf_xcoord;
	ap_uint<16> jf_ycoord;
	ap_uint<16> jf_zcoord;
	ap_uint<32> jf_debug;
	ap_uint<16> jf_round_robin;
	ap_uint<8> jf_detector_type;
	ap_uint<8> jf_header_version_type;
};

void setup_datamover (hls::stream<axis_datamover_ctrl> &datamover_cmd_stream, uint64_t address, size_t bytes_to_write);

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
                         ap_uint<4>  nstorage_cells);

void load_calibration(STREAM_512 &data_in, STREAM_512 &data_out,
                      hls::stream<axis_datamover_ctrl> &datamover_in_cmd,
                      hls::stream<ap_axiu<512,1,1,1> > &host_memory_in,
                      uint64_t in_mem_location[LOAD_CALIBRATION_BRAM_SIZE]);

void jf_conversion(STREAM_512 &data_in, STREAM_512 &data_out,
               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p0, hls::burst_maxi<rx100g_hbm_t> d_hbm_p1,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p2, hls::burst_maxi<rx100g_hbm_t> d_hbm_p3,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p4, hls::burst_maxi<rx100g_hbm_t> d_hbm_p5,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p6, hls::burst_maxi<rx100g_hbm_t> d_hbm_p7,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p8, hls::burst_maxi<rx100g_hbm_t> d_hbm_p9,
               hls::burst_maxi<rx100g_hbm_t> d_hbm_p10, hls::burst_maxi<rx100g_hbm_t> d_hbm_p11);

void host_writer(STREAM_512 &data_in,
                 hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                 hls::stream<ap_axiu<512,1,1,1> > &host_memory_out,
                 hls::stream<axis_datamover_ctrl> &datamover_out_cmd,
                 hls::stream<ap_uint<32> > &s_axis_work_request,
                 hls::stream<ap_uint<32> > &m_axis_completion,
                 volatile uint64_t &packets_processed,
                 ap_uint<8> &err_reg);

void data_and_cmd_stream_sync(hls::stream<ap_axiu<512,1,1,1> > &data_in,
                              hls::stream<ap_axiu<512,1,1,1> > &data_out,
                              hls::stream<axis_datamover_ctrl> &cmd_in,
                              hls::stream<axis_datamover_ctrl> &cmd_out);

void timer_hbm(STREAM_512 &in, STREAM_512 &data_out, uint64_t &counter);
void timer_host(STREAM_512 &data_in, STREAM_512 &data_out, uint64_t &counter);

void internal_packet_generator(STREAM_512 &data_in, STREAM_512 &data_out,
                               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                               hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out,
                               volatile ap_uint<1> &in_cancel);

void bitshuffle(STREAM_512 &data_in, STREAM_512 &out);

template<int N> ap_uint<N*32> pack32(ap_int<N> in[32]) {
#pragma HLS INLINE
    ap_uint<N*32> out;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < N; j++)
            out[i*N+j] = in[i][j];
    }
    return out;
}

template<int N> void unpack32(const ap_uint<N*32>& in, ap_int<N> out[32]) {
    for (int i = 0; i < 32; i++)
        for (int j = 0; j < N; j++)
            out[i][j] = in[i*N+j];
}

inline void setup_datamover (hls::stream<axis_datamover_ctrl> &datamover_cmd_stream, uint64_t address, size_t bytes_to_write) {
#pragma HLS INLINE
    axis_datamover_ctrl msg;
    msg.data = 0;
    msg.data(22,0) = bytes_to_write;
    msg.data[23] = 1; // INCR burst type
    msg.data[30] = 1; // Assert LAST on the last command
    msg.data(31+64,32) = address;
    datamover_cmd_stream << msg;
}

inline ap_uint<ADDR_STREAM_WIDTH> addr_packet(ap_uint<8> eth_packet,
                                              ap_uint<5> module,
                                              ap_uint<64> frame,
                                              ap_uint<32> jf_debug,
                                              ap_uint<64> timestamp,
                                              ap_uint<64> bunchid,
                                              ap_uint<1> packet_8kb) {
#pragma HLS INLINE
    ap_uint<ADDR_STREAM_WIDTH> retval = 0;
    addr_eth_packet(retval) = eth_packet;
    addr_module(retval) = module;
    addr_frame_number(retval) = frame;
    addr_jf_debug(retval) = jf_debug;
    addr_timestamp(retval) = timestamp;
    addr_bunch_id(retval) = bunchid;
    addr_flag_8kb(retval) = packet_8kb;
    return retval;
}

inline ap_uint<16> get_header_field_16(ap_uint<512> data, size_t position) {
    ap_uint<16> tmp = data(position+15, position);
    ap_uint<16> retval;
    // Swap endian
    retval(15,8) = tmp(7,0);
    retval(7,0) = tmp(15,8);
    return retval;
}

inline ap_uint<48> get_mac_addr(ap_uint<512> data, size_t position) {
    return data(position+47,position);
}

inline ap_uint<32> get_header_field_32_network_order(ap_uint<512> data, size_t position) {
    return data(position+31, position);
}

static const uint8_t ECHO_REQUEST  = 0x08;
static const uint8_t ECHO_REPLY    = 0x00;
static const uint8_t PROTOCOL_ICMP = 0x01;
static const uint8_t PROTOCOL_UDP  = 0x11;
static const uint16_t ETHER_IP     = 0x0800;
static const uint16_t ETHER_ARP    = 0x0806;
static const ap_uint<48> MAC_BROADCAST = 0xFFFFFFFFFFFF;

static const uint32_t eth_payload_pos = 14 * 8; // 112 bits
static const uint32_t ipv4_payload_pos = eth_payload_pos + 160; // 112 + 160 = 272 bits
static const uint32_t udp_payload_pos = ipv4_payload_pos + 64; // 112 + 160 + 64 = 336 bits (42 bytes)


// Network cores
#define UDP_METADATA_STREAM_WIDTH 34
#define udp_metadata_dest_port(x)     x(15,  0)
#define udp_metadata_payload_size(x)  x(31, 16)
#define udp_metadata_eth_err(x)       x[32]
#define udp_metadata_len_err(x)       x[33]

void ethernet(AXI_STREAM &eth_in,
              AXI_STREAM &ip_out,
              AXI_STREAM &arp_out,
              ap_uint<48> fpga_mac_addr,
              uint64_t& counter);

void ipv4(AXI_STREAM &eth_in,
          AXI_STREAM &udp_out,
          AXI_STREAM &icmp_out,
          ap_uint<32> fpga_ipv4_addr);

void icmp(AXI_STREAM& eth_in, AXI_STREAM& eth_out, uint64_t& counter);

void arp(AXI_STREAM &arp_in,
         AXI_STREAM &eth_out,
         ap_uint<48> fpga_mac_addr,
         ap_uint<32> fpga_ipv4_addr,
         ap_uint<1> enable,
         ap_uint<1> start);

void udp(AXI_STREAM &eth_in,
         AXI_STREAM &udp_payload_out,
         hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > &udp_metadata_out,
         uint64_t& counter);

void sls_detector(AXI_STREAM &udp_payload_in,
                  hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > &udp_metadata_in,
                  AXI_STREAM &data_out,
                  hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out);

#endif  /* __ACTION_RX100G_H__*/
