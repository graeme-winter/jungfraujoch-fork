// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>

#include "../fpga/host/HLSSimulatedDevice.h"

// ARP packet - from if_arp.h
#pragma pack(push)
#pragma pack(2)
struct RAW_ARP_Packet
{
    unsigned char dest_mac[6];
    unsigned char sour_mac[6];
    uint16_t ether_type;
    unsigned short int ar_hrd;		/* Format of hardware address.  */
    unsigned short int ar_pro;		/* Format of protocol address.  */
    unsigned char ar_hln;		    /* Length of hardware address.  */
    unsigned char ar_pln;		    /* Length of protocol address.  */
    unsigned short int ar_op;		/* ARP opcode (command).  */

    unsigned char __ar_sha[6];	/* Sender hardware address.  */
    unsigned char __ar_sip[4];		/* Sender IP address.  */
    unsigned char __ar_tha[6];	/* Target hardware address.  */
    unsigned char __ar_tip[4];		/* Target IP address.  */
};
#pragma pack(pop)

TEST_CASE("HLS_Network_ARP_Gratuitous") {
    HLSSimulatedDevice device(0, 64);

    DiffractionExperiment x;

    x.DataStreamModuleSize(2, {4});

    auto gain_from_file = GainCalibrationFromTestFile();

    for (int i = 0; i < x.GetModulesNum(); i++)
        REQUIRE_NOTHROW(device.LoadModuleGain(gain_from_file, i));

    device.CreateFinalPacket(x);

    device.StartAction(x);

    device.WaitForActionComplete();

    packet_512_t network_packet;
    device.OutputStream().read(network_packet);

    auto arp_packet = (RAW_ARP_Packet *) &network_packet.data;

    for (int i = 0; i < 6; i++) {
        REQUIRE(arp_packet->dest_mac[i] == 0xFF);
        REQUIRE(arp_packet->__ar_tha[i] == 0xFF);
    }

    REQUIRE(arp_packet->ether_type == htons(ETHER_ARP));
    REQUIRE(arp_packet->ar_op == htons(ARPOP_REQUEST));
    REQUIRE(arp_packet->ar_hrd == htons(ARPHRD_ETHER));
    REQUIRE(arp_packet->ar_pro == htons(ETHER_IP));
    REQUIRE(arp_packet->ar_hln == 6);
    REQUIRE(arp_packet->ar_pln == 4);
}

TEST_CASE("HLS_Network_UDP") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(129);
    jf_packet->framenum = 123456;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    auto jf_packet_axi = (ap_uint<512> *) packet;
    uint64_t packet_counter;

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 129);

    packet_512_t packet_out;
    udp_out.read(packet_out);
    REQUIRE(packet_out.data(63,0) == jf_packet->framenum);

    ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata_out;
    udp_metadata.read(udp_metadata_out);
    REQUIRE(udp_metadata_dest_port(udp_metadata_out) == 129);
    REQUIRE(udp_metadata_payload_size(udp_metadata_out) == 8240);
    REQUIRE(udp_metadata_eth_err(udp_metadata_out) == 0);
    REQUIRE(udp_metadata_len_err(udp_metadata_out) == 0);
}

TEST_CASE("HLS_Network_UDP_EthErr") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(129);
    jf_packet->framenum = 123456;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    auto jf_packet_axi = (ap_uint<512> *) packet;
    uint64_t packet_counter;

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = (i == 129) ? 1 : 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 129);

    packet_512_t packet_out;
    udp_out.read(packet_out);
    REQUIRE(packet_out.data(63,0) == jf_packet->framenum);

    ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata_out;
    udp_metadata.read(udp_metadata_out);
    REQUIRE(udp_metadata_dest_port(udp_metadata_out) == 129);
    REQUIRE(udp_metadata_payload_size(udp_metadata_out) == 8240);
    REQUIRE(udp_metadata_eth_err(udp_metadata_out) == 1);
    REQUIRE(udp_metadata_len_err(udp_metadata_out) == 0);
}

TEST_CASE("HLS_Network_UDP_LenErr") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(129);
    jf_packet->framenum = 123456;

    uint64_t packet_counter;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    for (int i = 0; i < 128; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 127);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 127);

    packet_512_t packet_out;
    udp_out.read(packet_out);
    REQUIRE(packet_out.data(63,0) == jf_packet->framenum);

    ap_uint<UDP_METADATA_STREAM_WIDTH> udp_metadata_out;
    udp_metadata.read(udp_metadata_out);
    REQUIRE(udp_metadata_dest_port(udp_metadata_out) == 129);
    REQUIRE(udp_metadata_payload_size(udp_metadata_out) == 8240);
    REQUIRE(udp_metadata_eth_err(udp_metadata_out) == 0);
    REQUIRE(udp_metadata_len_err(udp_metadata_out) == 1);
}

TEST_CASE("HLS_Network_UDP_SLS_detector_1") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(128 + 5);
    jf_packet->framenum = 123456;
    jf_packet->packetnum = 60;

    for (int i = 0; i < 4096; i++)
        jf_packet->data[i] = i;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    STREAM_512 raw_out;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;

    uint64_t packet_counter;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 129);

    while (!udp_out.empty()) {
        sls_detector(udp_out, udp_metadata, raw_out, addr0);
    }

    REQUIRE(raw_out.size() == 128);
    REQUIRE(addr0.size() == 2);

    for (int i = 0; i < 128; i++) {
        auto packet_out = raw_out.read();
        REQUIRE(memcmp(&(packet_out.data), jf_packet->data + 32 * i, 64) == 0);
    }

    auto addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 5 / 2);
    REQUIRE(addr_eth_packet(addr) == 2 * (jf_packet->packetnum | 64));

    addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 5 / 2);
    REQUIRE(addr_eth_packet(addr) == 2 * (jf_packet->packetnum | 64) + 1);
}

TEST_CASE("HLS_Network_UDP_SLS_detector_4kB") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(4096+48+8);
    jf_packet->udp_dest_port = htons(128 + 5);
    jf_packet->framenum = 123456;
    jf_packet->packetnum = 60;

    for (int i = 0; i < 2048; i++)
        jf_packet->data[i] = i;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    STREAM_512 raw_out;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;

    uint64_t packet_counter;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    for (int i = 0; i < 66; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 65);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 65);

    while (!udp_out.empty()) {
        sls_detector(udp_out, udp_metadata, raw_out, addr0);
    }

    REQUIRE(raw_out.size() == 64);
    REQUIRE(addr0.size() == 1);

    for (int i = 0; i < 64; i++) {
        auto packet_out = raw_out.read();
        REQUIRE(memcmp(&(packet_out.data), jf_packet->data + 32 * i, 64) == 0);
    }

    auto addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 5 / 4);
    REQUIRE(addr_eth_packet(addr) == (jf_packet->packetnum | 64));
}

TEST_CASE("HLS_Network_UDP_SLS_detector_2") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(128 + 4);
    jf_packet->framenum = UINT64_MAX - 300;
    jf_packet->packetnum = 35;

    for (int i = 0; i < 4096; i++)
        jf_packet->data[i] = i;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    STREAM_512 raw_out;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    uint64_t packet_counter;

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }
    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 1);
    REQUIRE(udp_out.size() == 129);

    while (!udp_out.empty()) {
        sls_detector(udp_out, udp_metadata, raw_out, addr0);
    }

    REQUIRE(raw_out.size() == 128);
    REQUIRE(addr0.size() == 2);

    for (int i = 0; i < 128; i++) {
        auto packet_out = raw_out.read();
        REQUIRE(memcmp(&(packet_out.data), jf_packet->data + 32 * i, 64) == 0);
    }

    auto addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 2);
    REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum);

    addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 2);
    REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum + 1);
}

TEST_CASE("HLS_Network_UDP_SLS_detector_2_packets") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(128 + 2);
    jf_packet->framenum = UINT64_MAX - 300;
    jf_packet->packetnum = 35;

    for (int i = 0; i < 4096; i++)
        jf_packet->data[i] = i;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    STREAM_512 raw_out;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    uint64_t packet_counter;

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }

    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 2);
    REQUIRE(udp_out.size() == 2*129);

    while (!udp_out.empty()) {
        sls_detector(udp_out, udp_metadata, raw_out, addr0);
    }

    REQUIRE(raw_out.size() == 256);
    REQUIRE(addr0.size() == 4);

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 128; i++) {
            auto packet_out = raw_out.read();
            REQUIRE(memcmp(&(packet_out.data), jf_packet->data + 32 * i, 64) == 0);
        }

        auto addr = addr0.read();
        REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
        REQUIRE(addr_module(addr) == 1);
        REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum);

        addr = addr0.read();
        REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
        REQUIRE(addr_module(addr) == 1);
        REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum + 1);
    }
}

TEST_CASE("HLS_Network_UDP_SLS_detector_2_packets_err") {
    char packet[130*64];

    auto jf_packet = (RAW_JFUDP_Packet *) packet;

    jf_packet->udp_length = htons(8248);
    jf_packet->udp_dest_port = htons(128 + 4);
    jf_packet->framenum = UINT64_MAX - 300;
    jf_packet->packetnum = 35;

    for (int i = 0; i < 4096; i++)
        jf_packet->data[i] = i;

    STREAM_512 udp_in;
    STREAM_512 udp_out;
    hls::stream<ap_uint<UDP_METADATA_STREAM_WIDTH> > udp_metadata;

    STREAM_512 raw_out;
    hls::stream<ap_uint<ADDR_STREAM_WIDTH> > addr0;

    auto jf_packet_axi = (ap_uint<512> *) packet;

    uint64_t packet_counter;

    for (int i = 0; i < 125; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 124);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }

    for (int i = 0; i < 130; i++) {
        packet_512_t packet_in;
        packet_in.user = 0;
        packet_in.last = (i == 129);
        packet_in.data = jf_packet_axi[i];
        udp_in.write(packet_in);
        udp(udp_in, udp_out, udp_metadata, packet_counter);
    }

    REQUIRE(udp_in.size() == 0);
    REQUIRE(udp_metadata.size() == 2);
    REQUIRE(udp_out.size() == 129+124);

    while (!udp_out.empty()) {
        sls_detector(udp_out, udp_metadata, raw_out, addr0);
    }

    REQUIRE(raw_out.size() == 128);
    REQUIRE(addr0.size() == 2);

    for (int i = 0; i < 128; i++) {
        auto packet_out = raw_out.read();
        REQUIRE(memcmp(&(packet_out.data), jf_packet->data + 32 * i, 64) == 0);
    }

    auto addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 2);
    REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum);

    addr = addr0.read();
    REQUIRE(addr_frame_number(addr) == jf_packet->framenum);
    REQUIRE(addr_module(addr) == 2);
    REQUIRE(addr_eth_packet(addr) == 2 * jf_packet->packetnum + 1);

}
