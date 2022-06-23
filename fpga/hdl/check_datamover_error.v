// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module check_datamover_error(
   output reg err_encountered,
   (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
   (* X_INTERFACE_PARAMETER = "ASSOCIATED_RESET resetn, ASSOCIATED_BUSIF S_AXIS" *)
   input clk,

   (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 resetn RST" *)
   (* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
   input resetn,

  (* X_INTERFACE_PARAMETER = "HAS_TLAST 1,HAS_TKEEP 1,HAS_TREADY 1,TUSER_WIDTH 0,TDATA_NUM_BYTES 1" *)
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)
  input [7:0] S_AXIS_tdata,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TKEEP" *)
  input  S_AXIS_tkeep,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TLAST" *)
  input S_AXIS_tlast,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)
  input S_AXIS_tvalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)
  output S_AXIS_tready
    );

  assign S_AXIS_tready = resetn;

  always @(posedge clk) begin
    if (!resetn)
        err_encountered <= 0;
    else if (S_AXIS_tvalid)
        err_encountered <= !S_AXIS_tdata[7];
  end

endmodule
