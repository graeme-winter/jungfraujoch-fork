// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module check_eth_busy(
  output reg eth_busy,
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
  (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF S_AXIS:M_AXIS" *)
  input clk,
  
  (* X_INTERFACE_PARAMETER = "HAS_TLAST 1,HAS_TKEEP 1,HAS_TREADY 1,TUSER_WIDTH 1,TDATA_NUM_BYTES 64" *)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)
  input [511:0] S_AXIS_tdata, // Transfer Data (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TKEEP" *)
  input [63:0] S_AXIS_tkeep, // Transfer Null Byte Indicators (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TLAST" *)
  input S_AXIS_tlast, // Packet Boundary Indicator (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TUSER" *)
  input S_AXIS_tuser, // Transfer user sideband (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)
  input S_AXIS_tvalid, // Transfer valid (required)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)
  output S_AXIS_tready, // Transfer ready (optional)
  
  (* X_INTERFACE_PARAMETER = "HAS_TLAST 1,HAS_TKEEP 1,HAS_TREADY 1,TUSER_WIDTH 1,TDATA_NUM_BYTES 64" *)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TDATA" *)
  output [511:0] M_AXIS_tdata, // Transfer Data (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TKEEP" *)
  output [63:0] M_AXIS_tkeep, // Transfer Null Byte Indicators (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TLAST" *)
  output M_AXIS_tlast, // Packet Boundary Indicator (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TUSER" *)
  output M_AXIS_tuser, // Transfer user sideband (optional)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TVALID" *)
  output M_AXIS_tvalid, // Transfer valid (required)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 M_AXIS TREADY" *)
  input M_AXIS_tready // Transfer ready (optional)
    );
  always @(posedge clk)
    eth_busy <= S_AXIS_tvalid;
  assign M_AXIS_tdata = S_AXIS_tdata;
  assign M_AXIS_tkeep = S_AXIS_tkeep;
  assign M_AXIS_tlast = S_AXIS_tlast;
  assign M_AXIS_tuser = S_AXIS_tuser;
  assign M_AXIS_tvalid = S_AXIS_tvalid;
  assign S_AXIS_tready = M_AXIS_tready;
endmodule
