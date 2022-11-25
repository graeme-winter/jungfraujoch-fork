// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HLSSIMULATEDDEVICE_H
#define JUNGFRAUJOCH_HLSSIMULATEDDEVICE_H

#include <thread>

#include "datamover_model.h"
#include "../../common/DiffractionExperiment.h"
#include "AcquisitionDevice.h"

// full JF packet - for simulation
#pragma pack(push)
#pragma pack(2)
struct RAW_JFUDP_Packet
{
    char dest_mac[6];
    char sour_mac[6];
    uint16_t ether_type;
    uint16_t ipv4_header_h;
    uint16_t ipv4_header_total_length;
    uint16_t ipv4_header_identification;
    uint16_t ipv4_header_flags_frag;
    uint16_t ipv4_header_ttl_protocol;
    uint16_t ipv4_header_checksum;
    uint32_t ipv4_header_sour_ip;
    uint32_t ipv4_header_dest_ip;
    uint16_t udp_sour_port;
    uint16_t udp_dest_port;
    uint16_t udp_length;
    uint16_t udp_checksum;
    // 42 bytes
    uint64_t framenum;
    uint32_t exptime; // x 1e-7 sec
    uint32_t packetnum;
    uint64_t bunchid;
    uint64_t timestamp;
    uint16_t moduleID;
    uint16_t xCoord;
    uint16_t yCoord;
    uint16_t zCoord;
    uint32_t debug;
    uint16_t roundRobin;
    uint8_t detectortype;
    uint8_t headerVersion;
    // 48 + 42 = 90 bytes
    uint16_t data[4096];
    // 96 + 8192 =  8282 bytes
};
#pragma pack(pop)

void hls_action(hls::stream<axis_datamover_ctrl> &in_datamover_cmd_stream, hls::stream<ap_axiu<512,1,1,1> > &in_datamover_stream,
                hls::stream<axis_datamover_ctrl> &out_datamover_cmd_stream, hls::stream<ap_axiu<512,1,1,1>  > &out_datamover_stream,
                rx100g_hbm_t *d_hbm_p0,  rx100g_hbm_t *d_hbm_p1,
                rx100g_hbm_t *d_hbm_p2,  rx100g_hbm_t *d_hbm_p3,
                rx100g_hbm_t *d_hbm_p4,  rx100g_hbm_t *d_hbm_p5,
                rx100g_hbm_t *d_hbm_p6,  rx100g_hbm_t *d_hbm_p7,
                rx100g_hbm_t *d_hbm_p8,  rx100g_hbm_t *d_hbm_p9,
                rx100g_hbm_t *d_hbm_p10, rx100g_hbm_t *d_hbm_p11,
                AXI_STREAM &din_eth, AXI_STREAM &dout_eth,
                uint64_t fpga_mac_addr,
                const ActionConfig &act_reg,
                hls::stream<ap_uint<32> > &s_axis_work_request,
                hls::stream<ap_uint<32> > &m_axis_completion,
                volatile ap_uint<1> &run_data_collection,
                volatile ap_uint<1> &cancel_data_collection,
                uint64_t in_mem_location[LOAD_CALIBRATION_BRAM_SIZE]);

uint16_t checksum(const uint16_t *addr, size_t count);

class HLSSimulatedDevice : public AcquisitionDevice {
    AXI_STREAM din_eth;
    AXI_STREAM dout_eth;

    ActionConfig cfg;

    volatile bool idle;
    std::vector<char> hbm_memory[12];

    hls::stream<ap_uint<32> > work_request_stream;
    hls::stream<ap_uint<32> > completion_stream;
    std::thread action_thread;

    Datamover<512> datamover_in;
    Datamover<512> datamover_out;
    ap_uint<1> run_data_collection;
    ap_uint<1> cancel_data_collection;
    uint64_t in_mem_location[LOAD_CALIBRATION_BRAM_SIZE];
    static const uint64_t fpga_mac_addr = 0xCCAA11223344;

    void HW_ReadActionRegister(ActionConfig *job) override;
    void HW_WriteActionRegister(const ActionConfig *job) override;

    void HW_StartAction() override;
    bool HW_IsIdle() const override;
    bool HW_ReadMailbox(uint32_t values[4]) override;
    void HW_SetCancelDataCollectionBit() override;
    bool HW_SendWorkRequest(uint32_t handle) override;
    uint64_t HW_GetMACAddress() const override;
    void HW_GetStatus(ActionStatus *status) const override;
public:
    HLSSimulatedDevice(uint16_t data_stream, size_t in_frame_buffer_size_modules, int16_t numa_node = -1);
    ~HLSSimulatedDevice();
    void SendPacket(char *buffer, int len, uint8_t user = 0);
    void CreatePacketJF(const DiffractionExperiment& experiment, uint64_t frame_number, uint32_t eth_packet,
                        uint32_t module, const uint16_t *data, bool trigger, int8_t adjust_axis = 0, uint8_t user = 0);
    void CreatePacketEIGER(const DiffractionExperiment& experiment, uint64_t frame_number, uint32_t eth_packet,
                        uint32_t module, const uint16_t *data, bool trigger, int8_t adjust_axis = 0, uint8_t user = 0);
    void CreatePackets(const DiffractionExperiment& experiment, uint64_t frame_number_0, uint64_t frames,
                       uint32_t module, const uint16_t *data, bool trigger, int8_t adjust_axis = 0,
                       uint8_t user = 0);
    void CreateFinalPacket(const DiffractionExperiment& experiment);
    AXI_STREAM &OutputStream();

};


#endif //JUNGFRAUJOCH_HLSSIMULATEDDEVICE_H
