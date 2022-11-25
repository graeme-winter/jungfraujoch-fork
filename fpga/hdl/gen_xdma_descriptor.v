// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns / 1ps

module gen_xdma_descriptor(
 (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
 (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF S_AXIS, ASSOCIATED_RESET resetn" *)
 input clk,

 (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 resetn RST" *)
 (* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
 input resetn,

 (* X_INTERFACE_PARAMETER = "HAS_TLAST 0,HAS_TKEEP 0,HAS_TREADY 1,TDATA_NUM_BYTES 13" *)
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)
 input [103:0] S_AXIS_tdata, // Transfer Data
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)
 input S_AXIS_tvalid, // Transfer valid
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)
 output S_AXIS_tready, // Transfer ready

 input dsc_ready,
 output reg [15:0] dsc_ctl,
 output reg [63:0] dsc_addr,
 output reg [27:0] dsc_len,
 output reg [31:0] counter,
 output reg dsc_load
);
    reg     read_axi;

    initial read_axi      = 1'b1;
    initial counter       = 32'h0;
    initial dsc_load      = 0'b0;
    assign S_AXIS_tready  = read_axi && resetn;

    always @(posedge clk) begin
        if (!resetn)
            begin
               read_axi   <= 1'b1;
               counter    <= 32'h0;
            end
        else if (read_axi && S_AXIS_tvalid)
            begin
               dsc_addr   <= S_AXIS_tdata[95:32];
               dsc_len    <= S_AXIS_tdata[22:0];
               dsc_ctl    <= 16'b10000;
               read_axi   <= 1'b0;
            end
        else if ((!read_axi) && dsc_ready)
            begin
               read_axi   <= 1'b1;
               counter    <= counter + 1;
            end
    end

    always @(posedge clk) begin
       if (resetn && (!read_axi) && dsc_ready)
               dsc_load   <= 1'b1;
       else
               dsc_load   <= 1'b0;
    end
endmodule
