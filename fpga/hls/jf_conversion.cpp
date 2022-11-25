// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"

#define HBM_BURST 16

// HBM and in-memory order:
// p0 ,p1  - gain G0
// p2 ,p3  - gain G1
// p4 ,p5  - gain G2
// p6 ,p7  - pedestal G0
// p8 ,p9  - pedestal G1
// p10,p11 - pedestal G2

template <class T> void unpack_2xhbm_to_32x16bit(const ap_uint<256> in1, const ap_uint<256> in2, T out[32]) {
#pragma HLS INLINE
    for (int i = 0; i < 16; i ++) {
        for (int j = 0; j < 16; j ++) {
            out[i][j] = in1[i*16+j];
            out[i+16][j] = in2[i*16+j];
        }
    }
}

ap_uint<512> convert(ap_uint<512> data_in,
                     ap_uint<256> packed_gainG0_1, ap_uint<256> packed_gainG0_2,
                     ap_uint<256> packed_gainG1_1, ap_uint<256> packed_gainG1_2,
                     ap_uint<256> packed_gainG2_1, ap_uint<256> packed_gainG2_2,
                     ap_uint<256> packed_pedeG0_1, ap_uint<256> packed_pedeG0_2,
                     ap_uint<256> packed_pedeG1_1, ap_uint<256> packed_pedeG1_2,
                     ap_uint<256> packed_pedeG2_1, ap_uint<256> packed_pedeG2_2,
                     ap_uint<1> save_raw,
                     one_over_energy_t one_over_energy_in_keV)
{
#pragma HLS PIPELINE
#pragma HLS INLINE off

    const ap_fixed<21, 18, AP_RND_CONV> half = 0.5f;

    ap_uint<16> in_val[32];
    ap_int<16> out_val[32];
    Loop0: for (int i = 0; i < 512; i++) in_val[i/16][i%16] = data_in[i];

    pedeG1G2_t  pedeG0[32];
    pedeG1G2_t  pedeG1[32];
    pedeG1G2_t  pedeG2[32];

    gainG0_t    gainG0[32];
    gainG1_t    gainG1[32];
    gainG2_t    gainG2[32];

    unpack_2xhbm_to_32x16bit(packed_gainG0_1, packed_gainG0_2, gainG0);
    unpack_2xhbm_to_32x16bit(packed_gainG1_1, packed_gainG1_2, gainG1);
    unpack_2xhbm_to_32x16bit(packed_gainG2_1, packed_gainG2_2, gainG2);
    unpack_2xhbm_to_32x16bit(packed_pedeG0_1, packed_pedeG0_2, pedeG0);
    unpack_2xhbm_to_32x16bit(packed_pedeG1_1, packed_pedeG1_2, pedeG1);
    unpack_2xhbm_to_32x16bit(packed_pedeG2_1, packed_pedeG2_2, pedeG2);

    Convert:
    for (int i = 0; i < 32; i++) {
        ap_uint<2> gain = in_val[i](15,14);
        if (save_raw) for (int j = 0; j < 16; j++) out_val[i][j] = in_val[i][j];
        else if (gainG0[i] == 0) out_val[i] = PIXEL_OUT_LOST; // if G0 gain factor is zero - mask pixel
        else if (in_val[i] == 0xc000) out_val[i] = PIXEL_OUT_SATURATION; // can saturate G2 - overload
        else if (in_val[i] == 0xffff) out_val[i] = PIXEL_OUT_0xFFFF; //error
        else if (in_val[i] == 0x4000) out_val[i] = PIXEL_OUT_G1_SATURATION; //cannot saturate G1 - error
        else if (gain == 2) out_val[i] = PIXEL_OUT_GAINBIT_2; // invalid gain
        else if ((pedeG0[i] > 16383) && (gain == 0)) out_val[i] = PIXEL_OUT_LOST;
        else if ((pedeG1[i] > 16383) && (gain == 1)) out_val[i] = PIXEL_OUT_LOST;
        else if ((pedeG2[i] > 16383) && (gain == 3)) out_val[i] = PIXEL_OUT_LOST;
        else {
            ap_fixed<18,16, AP_RND_CONV> val_diff = 0;
            ap_ufixed<25,6,AP_RND_CONV> val_gain = 0;
            ap_uint<14> adu = in_val[i](13,0); // take first two bits
            if (gain == 0) {
                val_diff = adu - pedeG0[i];
                val_gain = ap_ufixed<25,6,AP_RND_CONV>(gainG0[i]) / 32;
            } else if (gain == 1) {
                val_diff   = pedeG1[i] - adu;
                val_gain   = gainG1[i];
            } else {
                val_diff   = pedeG2[i] - adu;
                val_gain   = gainG2[i];
            }

            ap_fixed<21, 18, AP_RND_CONV> val_result = (val_diff * val_gain) * one_over_energy_in_keV;
            if (val_result > PIXEL_OUT_SATURATION)
                out_val[i] = PIXEL_OUT_SATURATION;
            else if (val_result >= 0)
                out_val[i] = val_result + half;
            else
                out_val[i] = val_result - half;
        }
    }

    ap_uint<512> data_out = pack32(out_val);
    //data_pack(data_out, out_val);
    return data_out;
}

void jf_conversion(STREAM_512 &data_in, STREAM_512 &data_out,
                   hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_in,
                   hls::stream<ap_uint<ADDR_STREAM_WIDTH> > &addr_out,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p0, hls::burst_maxi<rx100g_hbm_t> d_hbm_p1,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p2, hls::burst_maxi<rx100g_hbm_t> d_hbm_p3,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p4, hls::burst_maxi<rx100g_hbm_t> d_hbm_p5,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p6, hls::burst_maxi<rx100g_hbm_t> d_hbm_p7,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p8, hls::burst_maxi<rx100g_hbm_t> d_hbm_p9,
                   hls::burst_maxi<rx100g_hbm_t> d_hbm_p10, hls::burst_maxi<rx100g_hbm_t> d_hbm_p11) {
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE register both axis port=data_in
#pragma HLS INTERFACE register both axis port=data_out
#pragma HLS INTERFACE register both axis port=addr_in
#pragma HLS INTERFACE register both axis port=addr_out
    // For ports d_hbm_p0 - d_hbm_p9
// Critical is read performance, in bursts of up to 4 and assuming significant latency (here 100 cycles)
#pragma HLS INTERFACE m_axi port=d_hbm_p0 bundle=d_hbm_p0 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p1 bundle=d_hbm_p1 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p2 bundle=d_hbm_p2 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p3 bundle=d_hbm_p3 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p4 bundle=d_hbm_p4 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p5 bundle=d_hbm_p5 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p6 bundle=d_hbm_p6 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p7 bundle=d_hbm_p7 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p8 bundle=d_hbm_p8 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p9 bundle=d_hbm_p9 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p10 bundle=d_hbm_p10 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9
#pragma HLS INTERFACE m_axi port=d_hbm_p11 bundle=d_hbm_p11 depth=512 offset=off \
        max_read_burst_length=16  max_write_burst_length=16 latency=130 num_write_outstanding=8 num_read_outstanding=9

    packet_512_t packet_in;

    ap_uint<ADDR_STREAM_WIDTH> addr;
    addr_in >> addr;
    addr_out << addr;

    data_in >> packet_in;
    ap_uint<64> mode = ACT_REG_MODE(packet_in.data);
    ap_uint<1> conversion = (mode & MODE_CONV) ? 1 : 0;
    ap_uint<5> modules = ACT_REG_NMODULES(packet_in.data);
    ap_uint<32> in_one_over_energy = ACT_REG_ONE_OVER_ENERGY(packet_in.data);
    ap_uint<5>  storage_cells = ACT_REG_NSTORAGE_CELLS(packet_in.data);

    one_over_energy_t one_over_energy;
    for (int i = 0; i < 32; i++)
        one_over_energy[i] = in_one_over_energy[i];

    data_out << packet_in;

    if (conversion) {
        // Load calibration into HBM
        save_gainG0:
        for (int i = 0; i < modules * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p0.write_request(i, HBM_BURST);
                d_hbm_p1.write_request(i, HBM_BURST);
            }
            d_hbm_p0.write(packet_in.data(255, 0));
            d_hbm_p1.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p0.write_response();
                d_hbm_p1.write_response();
            }
        }

        save_gainG1:
        for (int i = 0; i < modules * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p2.write_request(i, HBM_BURST);
                d_hbm_p3.write_request(i, HBM_BURST);
            }
            d_hbm_p2.write(packet_in.data(255, 0));
            d_hbm_p3.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p2.write_response();
                d_hbm_p3.write_response();
            }
        }

        save_gainG2:
        for (int i = 0; i < modules * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p4.write_request(i, HBM_BURST);
                d_hbm_p5.write_request(i, HBM_BURST);
            }
            d_hbm_p4.write(packet_in.data(255, 0));
            d_hbm_p5.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p4.write_response();
                d_hbm_p5.write_response();
            }
        }

        save_pedeG0:
        for (int i = 0; i < modules * storage_cells * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p6.write_request(i, HBM_BURST);
                d_hbm_p7.write_request(i, HBM_BURST);
            }
            d_hbm_p6.write(packet_in.data(255, 0));
            d_hbm_p7.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p6.write_response();
                d_hbm_p7.write_response();
            }
        }

        save_pedeG1:
        for (int i = 0; i < modules * storage_cells * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p8.write_request(i, HBM_BURST);
                d_hbm_p9.write_request(i, HBM_BURST);
            }
            d_hbm_p8.write(packet_in.data(255, 0));
            d_hbm_p9.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p8.write_response();
                d_hbm_p9.write_response();
            }
        }

        save_pedeG2:
        for (int i = 0; i < modules * storage_cells * (RAW_MODULE_SIZE * 2 / 64); i++) {
#pragma HLS PIPELINE II=1
            data_in >> packet_in;
            if (i % HBM_BURST == 0) {
                d_hbm_p10.write_request(i, HBM_BURST);
                d_hbm_p11.write_request(i, HBM_BURST);
            }
            d_hbm_p10.write(packet_in.data(255, 0));
            d_hbm_p11.write(packet_in.data(511, 256));
            if (i % HBM_BURST == HBM_BURST - 1) {
                d_hbm_p10.write_response();
                d_hbm_p11.write_response();
            }
        }
    }

    addr_in >> addr;

    if (conversion) {
        ap_uint<6> counter = 0;

        pixel_conversion:
        while (!addr_last_flag(addr)) {
#pragma HLS PIPELINE II=1
            //ap_uint<17> offset = packet_in.user(16,0);
            if (counter % 16 == 0) {
                ap_uint<17> gain_offset = (addr_module(addr), addr_eth_packet(addr), counter);
                ap_uint<12> pedestal_location = addr_module(addr);

                if (storage_cells > 1) {
                    ap_uint<4> storage_cell_id = (addr_frame_number(addr) - 1) % storage_cells;
                    pedestal_location += modules * storage_cell_id;
                }

                ap_uint<26> pedestal_offset = (pedestal_location, addr_eth_packet(addr), counter);

                d_hbm_p0.read_request(gain_offset, 16);
                d_hbm_p1.read_request(gain_offset, 16);
                d_hbm_p2.read_request(gain_offset, 16);
                d_hbm_p3.read_request(gain_offset, 16);
                d_hbm_p4.read_request(gain_offset, 16);
                d_hbm_p5.read_request(gain_offset, 16);
                d_hbm_p6.read_request(pedestal_offset, 16);
                d_hbm_p7.read_request(pedestal_offset, 16);
                d_hbm_p8.read_request(pedestal_offset, 16);
                d_hbm_p9.read_request(pedestal_offset, 16);
                d_hbm_p10.read_request(pedestal_offset, 16);
                d_hbm_p11.read_request(pedestal_offset, 16);
            }
            ap_uint<256> packed_gainG0_1 = d_hbm_p0.read();
            ap_uint<256> packed_gainG0_2 = d_hbm_p1.read();
            ap_uint<256> packed_gainG1_1 = d_hbm_p2.read();
            ap_uint<256> packed_gainG1_2 = d_hbm_p3.read();
            ap_uint<256> packed_gainG2_1 = d_hbm_p4.read();
            ap_uint<256> packed_gainG2_2 = d_hbm_p5.read();
            ap_uint<256> packed_pedeG0_1 = d_hbm_p6.read();
            ap_uint<256> packed_pedeG0_2 = d_hbm_p7.read();
            ap_uint<256> packed_pedeG1_1 = d_hbm_p8.read();
            ap_uint<256> packed_pedeG1_2 = d_hbm_p9.read();
            ap_uint<256> packed_pedeG2_1 = d_hbm_p10.read();
            ap_uint<256> packed_pedeG2_2 = d_hbm_p11.read();

            data_in >> packet_in;
            packet_in.data = convert(packet_in.data,
                                     packed_gainG0_1, packed_gainG0_2,
                                     packed_gainG1_1, packed_gainG1_2,
                                     packed_gainG2_1, packed_gainG2_2,
                                     packed_pedeG0_1, packed_pedeG0_2,
                                     packed_pedeG1_1, packed_pedeG1_2,
                                     packed_pedeG2_1, packed_pedeG2_2,
                                     packet_in.id[0],
                                     one_over_energy);
            data_out << packet_in;

            if (counter == 63) {
                addr_out << addr;
                addr_in >> addr;
            }
            counter++;
        }
    } else {
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
    }

    data_in >> packet_in;
#ifndef __SYNTHESIS__
    if (!addr_in.empty()) throw std::runtime_error("Address in not empty");
    if (packet_in.user == 0) throw std::runtime_error("Something wrong with packet order");
    if (packet_in.last == 0) throw std::runtime_error("Something wrong with packet order");
#endif
    addr_out << addr;
    data_out << packet_in;
}
