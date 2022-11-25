// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module resetn_sync (
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
(* X_INTERFACE_PARAMETER = "ASSOCIATED_RESET out_resetn" *)
input clk,
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 in_resetn RST" *)
(* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
input in_resetn,
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 out_resetn RST" *)
(* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
output out_resetn
  );

(* ASYNC_REG = "TRUE" *) reg q0, q1, q2;

always @(posedge clk or negedge in_resetn)
if (~in_resetn)
    q0 <= 1'b0;
else
    q0 <= in_resetn;

always @(posedge clk)
    begin
    q1 <= q0;
    q2 <= q1;
    end

assign out_resetn = q2;

endmodule

