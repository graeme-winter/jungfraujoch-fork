// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <arpa/inet.h>
#include <sstream>
#include <iomanip>

#include "NetworkAddressConvert.h"
#include "JFJochException.h"

int32_t IPv4AddressFromStr(const std::string& addr) {
    in_addr ipv4_addr{};
    if (inet_pton(AF_INET, addr.c_str(), &ipv4_addr) != 1)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Need proper IPv4 address");
    return ipv4_addr.s_addr;
}

std::string IPv4AddressToStr(int32_t addr) {
    char tmp[256];
    if (inet_ntop(AF_INET, &addr, tmp, 255) != tmp)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Error in exporting IPv4 address");
    return {tmp};
}

std::string MacAddressToStr(uint64_t addr) {
    uint64_t mac_address_network_order = addr;
    std::ostringstream ss;
    for (int i = 0; i < 6; i++) {
        ss << std::setfill('0') << std::setw(2) << std::hex << ((mac_address_network_order >> (i * 8)) & 0xFF);
        if (i != 5) ss << ":";
    }
    return ss.str();
}

uint64_t MacAddressFromStr(const std::string &addr) {
    uint64_t ret = 0;
    uint32_t values[6];
    char end_char;
    if( sscanf( addr.c_str(), "%x:%x:%x:%x:%x:%x%c",
                &values[0], &values[1], &values[2],
                &values[3], &values[4], &values[5], &end_char ) != 6 )
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "MAC address invalid");

    for (int i = 0; i < 6; i++) {
        if (values[i] > 255)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "MAC address out of range");
        ret |= (uint64_t)values[i] << (i*8);
    }
    return ret;
}
