// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HLSSimulatedDevice.h"

#include <bitset>
#include <arpa/inet.h>
#include "datamover_model.h"

uint16_t checksum(const uint16_t *addr, size_t count) {
/* Compute Internet Checksum for "count" bytes
 *         beginning at location "addr".
 */
    long sum = 0;

    for (int i = 0; i < count / 2; i++)
        sum += addr[i];

/*  Add left-over byte, if any */
    if (count % 2 == 1)
        sum += ((uint8_t *) addr)[count / 2];

/*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

   return ~sum;
}

HLSSimulatedDevice::HLSSimulatedDevice(uint16_t data_stream, size_t in_frame_buffer_size_modules, int16_t numa_node)
        : AcquisitionDevice(data_stream),
          datamover_in(Direction::Input),
          datamover_out(Direction::Output, nullptr, 256),
          idle(true) {

    MapBuffersStandard(in_frame_buffer_size_modules,
                       (3 + 3 * 16)  * MAX_MODULES_FPGA + MODULES_INTERNAL_PACKET_GEN, numa_node);

    auto in_mem_location32 = (uint32_t *) in_mem_location;

    for (int i = 0; i < MAX_MODULES_FPGA * (3 + 3 * 16) + MODULES_INTERNAL_PACKET_GEN; i++) {
        in_mem_location32[2 * i    ] = ((uint64_t) buffer_h2c[i]) & UINT32_MAX;
        in_mem_location32[2 * i + 1] = ((uint64_t) buffer_h2c[i]) >> 32;
    }

    for (int m = 0; m < MODULES_INTERNAL_PACKET_GEN; m++)
        SetDefaultInternalGeneratorFrame((3+3*16) * MAX_MODULES_FPGA + m);

    for (auto &i: hbm_memory)
        // i.resize(SIZE_OF_HBM_BLOCK_IN_BYTES);
        i.resize(32*1024*1024); // only 32 MiB instead of 256 MiB per HBM interface (should be more than enough for all the tests anyway)
}

void HLSSimulatedDevice::CreateFinalPacket(const DiffractionExperiment& experiment) {
    CreatePacket(experiment, UINT64_MAX, 0, 0, nullptr, 0);
}

void HLSSimulatedDevice::SendPacket(char *buffer, int len, uint8_t user) {
    auto obuff = (ap_uint<512> *)buffer;

    for (int i = 0; i < (len + 63) / 64; i++) {
        packet_512_t packet_in;
        if (i == (len + 63) / 64 - 1) packet_in.last = 1;
        else packet_in.last = 0;
        packet_in.keep = 0xFFFFFFFFFFFFFFFF;
        packet_in.user = user;
        packet_in.data = obuff[i];
        din_eth.write(packet_in);
    }

}
void HLSSimulatedDevice::CreatePacket(const DiffractionExperiment& experiment, uint64_t frame_number, uint32_t eth_packet,
                                      uint32_t module, const uint16_t *data, bool trigger, int8_t adjust_axis, uint8_t user) {

    char buff[256*64];
    memset(buff, 0, 256*64);

    auto packet = (RAW_JFUDP_Packet *)buff;

    packet->ether_type = htons(0x0800);
    packet->sour_mac[0] = 0x00; // module 0

    uint64_t tmp_mac = fpga_mac_addr;
    for (int i = 0; i < 6; i++)
        packet->dest_mac[i] = (tmp_mac >> (8*i)) % 256;

    packet->ipv4_header_h = htons(0x4500); // Big endian in IP header!
    packet->ipv4_header_total_length = htons(8268); // Big endian in IP header!
    packet->ipv4_header_dest_ip = experiment.GetDestIPv4Address(data_stream); // Big endian in IP header!
    packet->ipv4_header_sour_ip = 0;

    packet->ipv4_header_ttl_protocol = htons(0x0011);
    packet->ipv4_header_checksum = checksum( (uint16_t *) &packet->ipv4_header_h, 20); // checksum is already in network order

    packet->udp_dest_port = htons(experiment.GetDestUDPPort(data_stream, module) + ((eth_packet >= 64) ? 1 : 0)); // module number
    packet->udp_sour_port = htons(0xDFAC);
    packet->udp_length = htons(8248);

    // JF headers are little endian
    packet->timestamp = 0xABCDEF0000FEDCBAL;
    packet->bunchid   = 0x1234567898765431L;
    packet->framenum = frame_number;
    packet->packetnum = eth_packet % 64;
    if (trigger) packet->debug =  1<<31;
    if (data != nullptr) {
        for (int i = 0; i < 4096; i++)
            packet->data[i] = data[i];
    }
    packet->udp_checksum = htons(checksum( (uint16_t *) (buff+18+24), 8192+8));

    SendPacket(buff, (130+adjust_axis)*64, user);
}

void HLSSimulatedDevice::CreatePackets(const DiffractionExperiment& experiment, uint64_t frame_number_0, uint64_t frames,
                                       uint32_t module, const uint16_t *data, bool trigger, int8_t adjust_axis,
                                       uint8_t user) {
    for (uint64_t i = 0; i < frames; i++) {
        for (int j = 0; j < 128; j++)
            CreatePacket(experiment, frame_number_0 + i, j, module, data + (i * 128 + j) * 4096, trigger, adjust_axis, user);
    }
}

AXI_STREAM & HLSSimulatedDevice::OutputStream() {
    return dout_eth;
}

void HLSSimulatedDevice::HW_ReadActionRegister(ActionConfig *job) {
    memcpy(job, &cfg, sizeof(ActionConfig));
}

void HLSSimulatedDevice::HW_WriteActionRegister(const ActionConfig *job) {
    memcpy(&cfg, job, sizeof(ActionConfig));
}

void HLSSimulatedDevice::HW_StartAction() {
    if (action_thread.joinable())
        action_thread.join();

    run_data_collection = 1;
    cancel_data_collection = 0;

    action_thread = std::thread([&] {
                                    idle = false;
                                    hls_action(datamover_in.GetCtrlStream(), datamover_in.GetDataStream(),
                                               datamover_out.GetCtrlStream(), datamover_out.GetDataStream(),
                                               (rx100g_hbm_t *) (hbm_memory[0].data()),
                                               (rx100g_hbm_t *) (hbm_memory[1].data()),
                                               (rx100g_hbm_t *) (hbm_memory[2].data()),
                                               (rx100g_hbm_t *) (hbm_memory[3].data()),
                                               (rx100g_hbm_t *) (hbm_memory[4].data()),
                                               (rx100g_hbm_t *) (hbm_memory[5].data()),
                                               (rx100g_hbm_t *) (hbm_memory[6].data()),
                                               (rx100g_hbm_t *) (hbm_memory[7].data()),
                                               (rx100g_hbm_t *) (hbm_memory[8].data()),
                                               (rx100g_hbm_t *) (hbm_memory[9].data()),
                                               (rx100g_hbm_t *) (hbm_memory[10].data()),
                                               (rx100g_hbm_t *) (hbm_memory[11].data()),
                                               din_eth, dout_eth, fpga_mac_addr, cfg,
                                               work_request_stream,
                                               completion_stream,
                                               run_data_collection,
                                               cancel_data_collection,
                                               in_mem_location);
                                    while (!datamover_out.IsIdle())
                                        std::this_thread::sleep_for(std::chrono::milliseconds(100));

                                    idle = true;
                                }
    );
}

HLSSimulatedDevice::~HLSSimulatedDevice() {
    if (action_thread.joinable())
        action_thread.join();
}

bool HLSSimulatedDevice::HW_ReadMailbox(uint32_t values[4]) {
    if (completion_stream.size() < 4)
        return false;
    values[0] = completion_stream.read();
    values[1] = completion_stream.read();
    values[2] = completion_stream.read();
    values[3] = completion_stream.read();
    return true;
}

void HLSSimulatedDevice::HW_SetCancelDataCollectionBit() {
    cancel_data_collection = 1;
}

bool HLSSimulatedDevice::HW_IsIdle() const {
    return idle && datamover_out.IsIdle();
}


bool HLSSimulatedDevice::HW_SendWorkRequest(uint32_t handle) {
    uint64_t address = (handle == UINT32_MAX) ? 0 : (uint64_t) buffer_c2h.at(handle);
    uint32_t parity = (std::bitset<32>(handle).count() + std::bitset<64>(address).count()) % 2;

    work_request_stream.write(handle);
    work_request_stream.write(address >> 32);
    work_request_stream.write(address & UINT32_MAX);
    work_request_stream.write(parity);
    return true;
}

uint32_t HLSSimulatedDevice::HW_GetInternalPacketGeneratorModuleNum() {
    return MODULES_INTERNAL_PACKET_GEN;
}

uint32_t HLSSimulatedDevice::HW_GetMaxModuleNum() {
    return MAX_MODULES_FPGA;
}

void hls_action(hls::stream<axis_datamover_ctrl> &in_datamover_cmd_stream,
                hls::stream<ap_axiu<512,1,1,1> > &in_datamover_stream,
                hls::stream<axis_datamover_ctrl> &out_datamover_cmd_stream,
                hls::stream<ap_axiu<512,1,1,1> > &out_datamover_stream,
                rx100g_hbm_t *d_hbm_p0, rx100g_hbm_t *d_hbm_p1,
                rx100g_hbm_t *d_hbm_p2, rx100g_hbm_t *d_hbm_p3,
                rx100g_hbm_t *d_hbm_p4, rx100g_hbm_t *d_hbm_p5,
                rx100g_hbm_t *d_hbm_p6, rx100g_hbm_t *d_hbm_p7,
                rx100g_hbm_t *d_hbm_p8, rx100g_hbm_t *d_hbm_p9,
                rx100g_hbm_t *d_hbm_p10, rx100g_hbm_t *d_hbm_p11,
                AXI_STREAM &din_eth, AXI_STREAM &dout_eth,
                uint64_t fpga_mac_addr,
                const ActionConfig &act_reg,
                hls::stream<ap_uint<32> > &s_axis_work_request,
                hls::stream<ap_uint<32> > &m_axis_completion,
                volatile ap_uint<1> &run_data_collection,
                volatile ap_uint<1> &cancel_data_collection,
                uint64_t in_mem_location[LOAD_CALIBRATION_BRAM_SIZE]) {

    uint64_t counter_hbm;
    uint64_t counter_host;
    uint64_t eth_packets;
    uint64_t icmp_packets;
    uint64_t udp_packets;

    uint64_t packets_processed;

    std::vector<std::thread> hls_cores;

    STREAM_512 ip1, udp1, udp2, icmp1, arp1;

    STREAM_512 raw0;
    STREAM_512 raw1;
    STREAM_512 raw2;
    STREAM_512 raw3;
    STREAM_512 raw4;
    STREAM_512 raw5;

    hls::stream<ap_uint<18 * 32> > pedestalG0_subtracted;

    STREAM_512 converted_1;
    STREAM_512 converted_2;
    STREAM_512 converted_3;

    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr1;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr2;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr3;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr4;

    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;
    ap_uint<1> idle_data_collection;

    ap_uint<4> err_reg;

    while(!din_eth.empty())
        ethernet(din_eth, ip1, arp1, fpga_mac_addr, eth_packets);

    while(!ip1.empty())
        ipv4(ip1, udp1, icmp1, act_reg.fpga_ipv4_addr);

    arp(arp1,
        dout_eth,
        fpga_mac_addr,
        act_reg.fpga_ipv4_addr,
        1, run_data_collection);

    while (!arp1.empty()) {
        arp(arp1,
            dout_eth,
            fpga_mac_addr,
            act_reg.fpga_ipv4_addr,
            1, run_data_collection);
    }

    // reset static counter
    arp(arp1,
        dout_eth,
        fpga_mac_addr,
        act_reg.fpga_ipv4_addr,
        0, run_data_collection);

    while(!icmp1.empty())
        icmp(icmp1, dout_eth, icmp_packets);

    while (!udp1.empty())
        udp(udp1, udp2, udp_metadata, udp_packets);

    while (!udp2.empty())
        sls_detector(udp2, udp_metadata, raw0, addr0);

    idle_data_collection = 0;
    hls_cores.emplace_back([&] {
        while (idle_data_collection == 0) {
            data_collection_fsm(raw0, raw1,
                                addr0, addr1,
                                run_data_collection,
                                cancel_data_collection,
                                idle_data_collection,
                                act_reg.mode,
                                act_reg.one_over_energy,
                                act_reg.frames_internal_packet_gen,
                                act_reg.nmodules,
                                act_reg.nstorage_cells);
            run_data_collection = 0;
        }
    });

    hls_cores.emplace_back([&] { load_calibration(raw1, raw2, in_datamover_cmd_stream, in_datamover_stream,
                                                  in_mem_location); });

    hls_cores.emplace_back([&] { internal_packet_generator(raw2, raw3, addr1, addr2, cancel_data_collection); });

    // Timer procedure - count how many times pedestal_corr/gain_corr is not accepting input (to help track down latency issues)
    hls_cores.emplace_back([&] { timer_hbm(raw3, raw4, counter_hbm); });

    // 3. Apply pedestal & gain corrections
    hls_cores.emplace_back([&] { jf_conversion(raw4, converted_1,
                                           addr2, addr4,
                                           d_hbm_p0, d_hbm_p1,
                                           d_hbm_p2, d_hbm_p3,
                                           d_hbm_p4, d_hbm_p5,
                                           d_hbm_p6, d_hbm_p7,
                                           d_hbm_p8, d_hbm_p9,
                                           d_hbm_p10, d_hbm_p11); });

    // Timer procedure - count how many times write_data is not accepting input (to help track down latency issues)
    hls_cores.emplace_back([&] { bitshuffle(converted_1, converted_2); });

    // Timer procedure - count how many times write_data is not accepting input (to help track down latency issues)
    hls_cores.emplace_back([&] { timer_host(converted_2, converted_3, counter_host); });

    // 5. Frame statistics
    hls_cores.emplace_back([&] {
        host_writer(converted_3, addr4, out_datamover_stream,
                    out_datamover_cmd_stream, s_axis_work_request, m_axis_completion,
                    packets_processed, err_reg); });

    for (auto &i : hls_cores)
        i.join();

    if (!din_eth.empty())
        throw std::runtime_error("din_eth queue not empty");

    if (!addr1.empty())
        throw std::runtime_error("Addr1 queue not empty");

    if (!addr2.empty())
        throw std::runtime_error("Addr2 queue not empty");

    if (!addr3.empty())
        throw std::runtime_error("Addr3 queue not empty");

    if (!addr4.empty())
        throw std::runtime_error("Addr4 queue not empty");

    if (!raw1.empty())
        throw std::runtime_error("Raw1 queue not empty");

    if (!raw2.empty())
        throw std::runtime_error("Raw2 queue not empty");

    if (!raw3.empty())
        throw std::runtime_error("Raw3 queue not empty");

    if (!raw4.empty())
        throw std::runtime_error("Raw4 queue not empty");

    if (!pedestalG0_subtracted.empty())
        throw std::runtime_error("PedestalG0_subtracted queue not empty");

    if (!converted_1.empty())
        throw std::runtime_error("Converted_1 queue not empty");

    if (!converted_2.empty())
        throw std::runtime_error("Converted_2 queue not empty");

    if (!converted_3.empty())
        throw std::runtime_error("Converted_2 queue not empty");

    if (!in_datamover_stream.empty())
        throw std::runtime_error("Datamover queue is not empty");

    if (!s_axis_work_request.empty())
        throw std::runtime_error("Work request stream is not empty");

    if (err_reg != 0)
        throw std::runtime_error("Error reg for frame_statistics not zero, val=" + std::to_string(err_reg));

}

uint64_t HLSSimulatedDevice::HW_GetMACAddress() const {
    return fpga_mac_addr;
}