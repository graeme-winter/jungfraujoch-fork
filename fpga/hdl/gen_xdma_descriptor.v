// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module gen_xdma_descriptor_c2h(
 (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
 (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF S_AXIS" *)
 input clk,

 (* X_INTERFACE_PARAMETER = "HAS_TLAST 0,HAS_TKEEP 0,HAS_TREADY 1,TDATA_NUM_BYTES 13" *)
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)
 input [103:0] S_AXIS_tdata, // Transfer Data
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)
 input S_AXIS_tvalid, // Transfer valid
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)
 output S_AXIS_tready, // Transfer ready

 input dsc_ready,
 output [15:0] dsc_ctl,
 output [63:0] dsc_src_addr,
 output [63:0] dsc_dst_addr,
 output [27:0] dsc_len,
 output dsc_load
);
    assign S_AXIS_tready = dsc_ready;
    assign dsc_load      = S_AXIS_tvalid;
    assign dsc_src_addr  = 0;
    assign dsc_dst_addr  = S_AXIS_tdata[95:32];
    assign dsc_len       = S_AXIS_tdata[22:0];
    assign dsc_ctl       = 0;
endmodule

module gen_xdma_descriptor_h2c(
 (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
 (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF S_AXIS" *)
 input clk,

 (* X_INTERFACE_PARAMETER = "HAS_TLAST 0,HAS_TKEEP 0,HAS_TREADY 1,TDATA_NUM_BYTES 13" *)
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)
 input [103:0] S_AXIS_tdata, // Transfer Data
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)
 input S_AXIS_tvalid, // Transfer valid
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)
 output S_AXIS_tready, // Transfer ready

 input dsc_ready,
 output [15:0] dsc_ctl,
 output [63:0] dsc_src_addr,
 output [63:0] dsc_dst_addr,
 output [27:0] dsc_len,
 output dsc_load
);
    assign S_AXIS_tready = dsc_ready;
    assign dsc_load      = S_AXIS_tvalid;
    assign dsc_src_addr  = S_AXIS_tdata[95:32];
    assign dsc_dst_addr  = 0;
    assign dsc_len       = S_AXIS_tdata[22:0];
    assign dsc_ctl       = 0;
endmodule
