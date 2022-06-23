/************************************************
BSD 3-Clause License

Copyright (c) 2019, HPCN Group, UAM Spain (hpcn-uam.es)
and Systems Group, ETH Zurich (systems.ethz.ch)
All rights reserved.


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

************************************************/

// With modifications from Paul Scherrer Insitute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#include "hls_jfjoch.h"
#include "ip_header_checksum.h"

//Echo or Echo Reply Message
//
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     Type      |     Code      |          Checksum             |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |           Identifier          |        Sequence Number        |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     Data ...
//   +-+-+-+-+-
//   
//   Type
//      8 for echo message;
//      0 for echo reply message.
//      
//   Code
//
//      0
//
//   Checksum
//      The checksum is the 16-bit ones's complement of the one's
//      complement sum of the ICMP message starting with the ICMP Type.
//      For computing the checksum , the checksum field should be zero.
//      If the total length is odd, the received data is padded with one
//      octet of zeros for computing the checksum.
//
//   Identifier
//      If code = 0, an identifier to aid in matching echos and replies,
//      may be zero.
//      
//   Sequence Number
//      If code = 0, a sequence number to aid in matching echos and
//      replies, may be zero.
// Description
//
//      The data received in the echo message must be returned in the echo
//      reply message.

void icmp(AXI_STREAM& eth_in, AXI_STREAM& eth_out, uint64_t& counter) {

#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE axis register both port=eth_in name=s_axis_icmp
#pragma HLS INTERFACE axis register both port=eth_out name=m_axis_icmp
#pragma HLS INTERFACE ap_vld port=counter

#pragma HLS pipeline II=1 enable_flush

    enum   icmp_state {INSPECT_HEADER, FORWARD, DROP_PACKET};
    static icmp_state state = INSPECT_HEADER;

    static uint64_t internal_counter = 0;
    counter = internal_counter;

    ap_uint<8> icmp_type;
    ap_uint<8> icmp_code;
    ap_uint<17> icmp_checksum;

    packet_512_t    packet;

    eth_in.read(packet);

    if (state == INSPECT_HEADER) {

        ap_uint<48> dest_mac = get_mac_addr(packet.data, 0);
        ap_uint<48> src_mac = get_mac_addr(packet.data, 48);

        // Swap MAC addresses for reply
        packet.data(47, 0) = src_mac;
        packet.data(95, 48) = dest_mac;

        icmp_type = packet.data(ipv4_payload_pos + 7, ipv4_payload_pos);
        icmp_code = packet.data(ipv4_payload_pos + 15, ipv4_payload_pos + 8);

        icmp_checksum = (packet.data(ipv4_payload_pos + 23, ipv4_payload_pos + 16),
                packet.data(ipv4_payload_pos + 31, ipv4_payload_pos + 24))
                        + 0x0800;

        if ((icmp_type == ECHO_REQUEST) && (icmp_code == 0)) {

            ap_uint<32> ipv4_src_ip = packet.data(eth_payload_pos + 127, eth_payload_pos + 96);
            ap_uint<32> ipv4_dest_ip = packet.data(eth_payload_pos + 159, eth_payload_pos + 128);

            packet.data(eth_payload_pos + 71, eth_payload_pos + 64) = 128;                   // IP time to live
            packet.data(eth_payload_pos + 95, eth_payload_pos + 80) = 0;
            packet.data(eth_payload_pos + 127, eth_payload_pos + 96) = ipv4_dest_ip;
            packet.data(eth_payload_pos + 159, eth_payload_pos + 128) = ipv4_src_ip;         // swap IPs

            ap_uint<16> ipv4_header_checksum = computeCheckSum20B(
                    packet.data(eth_payload_pos + 159, eth_payload_pos + 0));
            packet.data(eth_payload_pos + 95, eth_payload_pos + 80) = ipv4_header_checksum;

            icmp_checksum = icmp_checksum(15, 0) + icmp_checksum.bit(16);

            packet.data(ipv4_payload_pos + 7, ipv4_payload_pos) = ECHO_REPLY;
            packet.data(ipv4_payload_pos + 31, ipv4_payload_pos + 16)
                    = (icmp_checksum(7, 0), icmp_checksum(15, 8));     // Insert ICMP checksum

            state = FORWARD;
        } else
            state = DROP_PACKET;
    }

    if (state == FORWARD) {
        packet.dest = 0;
        eth_out.write(packet);
    }

    if (packet.last) {
        state = INSPECT_HEADER;
        internal_counter++;
    }
}
