// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <bitset>

#include "../common/JFJochException.h"
#include "OpenCAPIDevice.h"

int main(int argc, char **argv) {

#ifdef __PPC__
    std::vector<uint16_t> pci_slot_number = {4, 5, 6, 7};
#else
    std::vector<int16_t> pci_slot_number = {4};
#endif

    for (const auto &i: pci_slot_number) {
        std::cout << "Trying slot " << i << std::endl;
        std::cout << std::endl;
        try {
            OpenCAPIDevice test(i, 0, 512);
            auto status = test.GetStatus();

            std::cout << "OpenCAPI card detected      " << std::endl;
            std::cout << "Git SHA1                    " << std::hex << status.git_sha1() << std::endl;
            std::cout << "Max modules                 " << std::dec << status.max_modules() << std::endl;

            std::cout << "QSFP-DD module present      " << status.qsfp_module_present() << std::endl;
            std::cout << "Ethernet aligned            " << status.ethernet_rx_aligned() << std::endl;
            std::cout << "Ethernet bad FCS status     " << status.ethernet_bad_fcs() << std::endl;

            std::cout << "FPGA  12V rail current [A]  " << status.current_edge_12v_a() << std::endl;
            std::cout << "FPGA  12V rail voltage [V]  " << status.voltage_edge_12v_v() << std::endl;
            std::cout << "FPGA 3.3V rail current [A]  " << status.current_edge_3p3v_a() << std::endl;
            std::cout << "FPGA 3.3V rail voltage [V]  " << status.voltage_edge_3p3v_v() << std::endl;
            std::cout << "FPGA temperature            " << status.fpga_temp_degc() << std::endl;
            std::cout << "HBM temperature             " << status.hbm_temp() << std::endl;
            std::cout << "HBM temperature (max.)      " << status.max_hbm_temp() << std::endl;
            std::cout << "Idle                        " << status.fpga_idle() << std::endl;

            std::cout << "Full status register        " << std::bitset<32>(status.full_status_register()) << std::endl;

            std::cout << "Stalls HBM                  " << status.stalls_hbm() << std::endl;
            std::cout << "Stalls host mem             " << status.stalls_host() << std::endl;

            ActionConfig cfg = test.ReadActionRegister();
            std::cout << "IPv4 address                " << DiffractionExperiment::IPv4AddressToStr(cfg.fpga_ipv4_addr) << std::endl;
            std::cout << "MAC address                 " << test.GetMACAddress() << std::endl;
            std::cout << "Mode                        " << std::hex << cfg.mode << std::dec << std::endl;
            std::cout << "Modules                     " << std::dec << cfg.nmodules  << std::endl;
            std::cout << "Frames int. pkt. gen.       " << std::dec << cfg.frames_internal_packet_gen  << std::endl;
            std::cout << std::endl;
            std::cout << "FPGA FIFO status:           " << std::endl;
            for (const auto &[x,y]: status.fifo_status()) {
                std::string s = x;
                s.resize(28, ' ');
                switch (y) {
                    case JFJochProtoBuf::FPGAFIFOStatus::PARTIAL:
                        s += "Partial";
                        break;
                    case JFJochProtoBuf::FPGAFIFOStatus::EMPTY:
                        s += "Empty";
                        break;
                    case JFJochProtoBuf::FPGAFIFOStatus::FULL:
                        s += "Full";
                        break;
                }
                std::cout << s << std::endl;
            }

            std::cout << std::endl;
            std::cout << "Packet counters   - ETH     " << status.packets_ether() << std::endl;
            std::cout << "                  - UDP     " << status.packets_udp() << std::endl;
            std::cout << "                  - ICMP    " << status.packets_icmp() << std::endl;
            std::cout << "                  - JFJoch  " << status.packets_jfjoch() << std::endl;

            std::cout << std::endl;
        } catch (const JFJochException &e) {
            std::cout << e.what() << std::endl;
            std::cout << std::endl;
        }
    }
}
