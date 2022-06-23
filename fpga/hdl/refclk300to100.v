// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module refclk300to100 (
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 refclk300 CLK" *)
(* X_INTERFACE_PARAMETER = "FREQ_HZ 300000000" *)
input refclk300,
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 refclk100 CLK" *)
(* X_INTERFACE_PARAMETER = "FREQ_HZ 100000000" *)
output refclk100
  );
       BUFGCE_DIV #(
          .BUFGCE_DIVIDE(3)
       )
       BUFGCE_DIV_inst (
          .O(refclk100),
          .CE(1'b1),
          .CLR(1'b0),
          .I(refclk300)
       );
endmodule
