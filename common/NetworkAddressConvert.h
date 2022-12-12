// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_NETWORKADDRESSCONVERT_H
#define JUNGFRAUJOCH_NETWORKADDRESSCONVERT_H

#include <cstdint>
#include <string>

std::string IPv4AddressToStr(int32_t addr);
std::string MacAddressToStr(uint64_t addr);
int32_t IPv4AddressFromStr(const std::string& addr);
uint64_t MacAddressFromStr(const std::string& addr);

#endif //JUNGFRAUJOCH_NETWORKADDRESSCONVERT_H
