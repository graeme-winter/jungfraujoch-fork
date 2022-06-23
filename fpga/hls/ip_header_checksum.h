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

// With modifications from Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0 or GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_IP_HEADER_CHECKSUM_H
#define JUNGFRAUJOCH_IP_HEADER_CHECKSUM_H

#include "hls_jfjoch.h"

inline ap_uint<16> computeCheckSum20B(ap_uint<160> ipHeader){
#pragma HLS INLINE
    ap_uint<16> checksumL0[10]={0,0,0,0,0,0,0,0,0,0};
    ap_uint<17> checksumL1[5] = {0,0,0,0,0};
    ap_uint<18> checksumL2[2] = {0,0};
    ap_uint<20> checksumL3;
    ap_uint<17> checksumL4_r;
    ap_uint<17> checksumL4_o;
    ap_uint<16> checksum;

    assign2array: for (int m = 0; m < 10 ; m++){
#pragma HLS UNROLL
    checksumL0[m] = ipHeader((m*16)+15,m*16);
}

    sumElementsL1 : for (int m = 0; m < 10 ; m=m+2){
#pragma HLS UNROLL
    checksumL1[m/2] = checksumL0[m] + checksumL0[m+1];
}

    checksumL2[0] = checksumL1[0] + checksumL1[1];
    checksumL2[1] = checksumL1[2] + checksumL1[3];
    checksumL3 = checksumL2[0] + checksumL2[1] + checksumL1[4];

    checksumL4_r = checksumL3(15, 0) + checksumL3(19,16);
    checksumL4_o = checksumL3(15, 0) + checksumL3(19,16) + 1;

    if (checksumL4_r.bit(16))
        checksum   = ~checksumL4_o(15,0);
    else
        checksum   = ~checksumL4_r(15,0);

    return checksum;
}

#endif //JUNGFRAUJOCH_IP_HEADER_CHECKSUM_H
