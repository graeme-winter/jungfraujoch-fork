// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: CERN-OHL-S-2.0

`timescale 1ns/1ps

// parameters imported from source files
`define ACTION_TYPE 32'h52324158
`define RELEASE_LEVEL 32'h00000000
`define GIT_SHA1 32'h00000000
`define MAX_MODULES_FPGA 32'd8
`define MODULES_INTERNAL_PACKET_GEN 32'd1

`define ADDR_AP_CTRL             16'h0000
`define ADDR_SET_LED             16'h0008
`define ADDR_GIT_SHA1            16'h000C

`define ADDR_ACTION_TYPE         32'h0010
`define ADDR_RELEASE_LEVEL       32'h0014
`define ADDR_HBM_TEMP            16'h0018
`define ADDR_HBM_MAX_TEMP        16'h001C
`define ADDR_MAX_MODULES_FPGA    16'h0020
`define ADDR_MODS_INT_PKT_GEN    16'h0024

`define ADDR_STALLS_HOST_LO      16'h0028
`define ADDR_STALLS_HOST_HI      16'h002C
`define ADDR_STALLS_HBM_LO       16'h0030
`define ADDR_STALLS_HBM_HI       16'h0034
`define ADDR_FIFO_STATUS         16'h0038

`define ADDR_PACKETS_PROC_LO     16'h0040
`define ADDR_PACKETS_PROC_HI     16'h0044
`define ADDR_PACKETS_ETH_LO      16'h0048
`define ADDR_PACKETS_ETH_HI      16'h004C

`define ADDR_PACKETS_ICMP_LO     16'h0050
`define ADDR_PACKETS_ICMP_HI     16'h0054
`define ADDR_PACKETS_UDP_LO      16'h0058
`define ADDR_PACKETS_UDP_HI      16'h005C

`define ADDR_MAC_ADDR_LO         16'h0060
`define ADDR_MAC_ADDR_HI         16'h0064
`define ADDR_IPV4_ADDR           16'h0068
`define ADDR_NMODULES            16'h006C

`define ADDR_DATA_COL_MODE       16'h0070
`define ADDR_ONE_OVER_ENERGY     16'h0074
`define ADDR_FRAMES_PER_TRIG     16'h0078

module action_config
#(parameter
    C_S_AXI_ADDR_WIDTH = 16,
    C_S_AXI_DATA_WIDTH = 32
)(
    (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
    (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF s_axi, ASSOCIATED_RESET resetn" *)
    input                                   clk                   ,
    (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 resetn RST" *)
    (* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
    input                                   resetn                 ,

    input  wire [C_S_AXI_ADDR_WIDTH-1:0]   s_axi_AWADDR,
    input  wire                            s_axi_AWVALID,
    output wire                            s_axi_AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1:0]   s_axi_WDATA,
    input  wire [C_S_AXI_DATA_WIDTH/8-1:0] s_axi_WSTRB,
    input  wire                            s_axi_WVALID,
    output wire                            s_axi_WREADY,
    output wire [1:0]                      s_axi_BRESP,
    output wire                            s_axi_BVALID,
    input  wire                            s_axi_BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0]   s_axi_ARADDR,
    input  wire                            s_axi_ARVALID,
    output wire                            s_axi_ARREADY,
    output wire [C_S_AXI_DATA_WIDTH-1:0]   s_axi_RDATA,
    output wire [1:0]                      s_axi_RRESP,
    output wire                            s_axi_RVALID,
    input  wire                            s_axi_RREADY,

    output reg [31:0]                      data_collection_mode     ,
    output reg [47:0]                      fpga_mac_addr            ,
    output reg [31:0]                      fpga_ipv4_addr           ,
    output reg [31:0]                      one_over_energy          ,
    output reg [31:0]                      frames_per_trigger       ,
    output reg [7:0]                       nmodules                 ,

    output reg                             data_collection_start    ,
    output reg                             data_collection_cancel   ,
    input                                  data_collection_idle     ,
    input                                  qsfpdd_modprs            ,
    input [6:0]                            hbm_temperature          ,
    input                                  hbm_temp_trip            ,
    input                                  apb_complete_0           ,

    input                                  pede2gain_data_fifo_empty  ,
    input                                  pede2gain_data_fifo_full   ,
    input                                  pede2gain_addr_fifo_empty  ,
    input                                  pede2gain_addr_fifo_full   ,
    input                                  udp_fifo_empty             ,
    input                                  udp_fifo_full              ,
    input                                  host_mem_data_fifo_empty   ,
    input                                  host_mem_data_fifo_full    ,
    input                                  host_mem_cmd_fifo_empty    ,
    input                                  host_mem_cmd_fifo_full     ,
    input                                  work_req_fifo_empty        ,
    input                                  work_req_fifo_full         ,
    input                                  work_compl_fifo_empty      ,
    input                                  work_compl_fifo_full       ,
    input                                  last_data_fifo_empty       ,
    input                                  last_data_fifo_full        ,
    input                                  last_addr_fifo_empty       ,
    input                                  last_addr_fifo_full        ,

    input                                  mailbox_interrupt_0        ,
    input                                  mailbox_interrupt_1        ,

    input [63:0]                           stalls_hbm                 ,
    input [63:0]                           stalls_host                ,
    input                                  stalls_hbm_valid           ,
    input                                  stalls_host_valid          ,
    input [63:0]                           packets_processed          ,
    input                                  packets_processed_valid    ,
    input [63:0]                           packets_eth                ,
    input                                  packets_eth_valid          ,
    input [63:0]                           packets_icmp               ,
    input                                  packets_icmp_valid         ,
    input [63:0]                           packets_udp                ,
    input                                  packets_udp_valid          ,

    input [3:0]                            host_writer_err       ,
    input                                  host_writer_err_valid ,
    input                                  eth_stat_rx_status         ,
    input                                  eth_stat_rx_aligned        ,
    input                                  eth_busy                   ,
    input                                  eth_stat_rx_packet_bad_fcs ,
    input                                  mm2s_error                 ,
    input                                  s2mm_error                 ,
    output                                 qsfp_led_busy                ,
    output                                 qsfp_led_conn
);
//------------------------Parameter----------------------
localparam

    WRIDLE                 = 2'd0,
    WRDATA                 = 2'd1,
    WRRESP                 = 2'd2,
    WRRESET                = 2'd3,
    RDIDLE                 = 2'd0,
    RDDATA                 = 2'd1,
    RDRESET                = 2'd2,
    ADDR_BITS              = C_S_AXI_ADDR_WIDTH;

//------------------------Local signal-------------------
    reg  [1:0]                    wstate = WRRESET;
    reg  [1:0]                    wnext;
    reg  [ADDR_BITS-1:0]          waddr;
    wire [31:0]                   wmask;
    wire                          aw_hs;
    wire                          w_hs;
    reg  [1:0]                    rstate = RDRESET;
    reg  [1:0]                    rnext;
    reg  [31:0]                   rdata;
    wire                          ar_hs;
    wire [ADDR_BITS-1:0]          raddr;

// JFJoch signals
    (* ASYNC_REG = "TRUE" *) reg reg_eth_busy_1;
    (* ASYNC_REG = "TRUE" *) reg reg_eth_busy_2;

    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_status_1;
    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_aligned_1;

    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_status_2;
    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_aligned_2;

    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_packet_bad_fcs_1;
    (* ASYNC_REG = "TRUE" *) reg reg_eth_stat_rx_packet_bad_fcs_2;
    reg reg_eth_stat_rx_packet_bad_fcs_ever;

    reg [6:0]  reg_hbm_temperature;
    reg [6:0]  reg_hbm_max_temperature;
    reg        reg_hbm_temp_trip;
    reg        reg_hbm_temp_trip_ever;

    reg [63:0] reg_stalls_hbm;
    reg [63:0] reg_stalls_host;
    reg [63:0] reg_packets_processed;
    reg [63:0] reg_packets_eth;
    reg [63:0] reg_packets_icmp;
    reg [63:0] reg_packets_udp;
    reg [31:0] reg_fifo_status;
    reg [3:0]  reg_host_writer_err;
    reg        reg_qsfpdd_modprs;
    reg        reg_data_collection_idle;
//------------------------Instantiation------------------

//------------------------AXI write fsm------------------
assign s_axi_AWREADY = (wstate == WRIDLE);
assign s_axi_WREADY  = (wstate == WRDATA);
assign s_axi_BRESP   = 2'b00;  // OKAY
assign s_axi_BVALID  = (wstate == WRRESP);
assign wmask   = { {8{s_axi_WSTRB[3]}}, {8{s_axi_WSTRB[2]}}, {8{s_axi_WSTRB[1]}}, {8{s_axi_WSTRB[0]}} };
assign aw_hs   = s_axi_AWVALID & s_axi_AWREADY;
assign w_hs    = s_axi_WVALID & s_axi_WREADY;

// wstate
always @(posedge clk) begin
    if (!resetn)
        wstate <= WRRESET;
    else
        wstate <= wnext;
end

// wnext
always @(*) begin
    case (wstate)
        WRIDLE:
            if (s_axi_AWVALID)
                wnext = WRDATA;
            else
                wnext = WRIDLE;
        WRDATA:
            if (s_axi_WVALID)
                wnext = WRRESP;
            else
                wnext = WRDATA;
        WRRESP:
            if (s_axi_BREADY)
                wnext = WRIDLE;
            else
                wnext = WRRESP;
        default:
            wnext = WRIDLE;
    endcase
end

// waddr
always @(posedge clk) begin
    if (aw_hs)
        waddr <= s_axi_AWADDR[ADDR_BITS-1:0];
end

//------------------------AXI read fsm-------------------
assign s_axi_ARREADY = (rstate == RDIDLE);
assign s_axi_RDATA   = rdata;
assign s_axi_RRESP   = 2'b00;  // OKAY
assign s_axi_RVALID  = (rstate == RDDATA);
assign ar_hs   = s_axi_ARVALID & s_axi_ARREADY;
assign raddr   = s_axi_ARADDR[ADDR_BITS-1:0];

// rstate
always @(posedge clk) begin
    if (!resetn)
        rstate <= RDRESET;
    else
        rstate <= rnext;
end

// rnext
always @(*) begin
    case (rstate)
        RDIDLE:
            if (s_axi_ARVALID)
                rnext = RDDATA;
            else
                rnext = RDIDLE;
        RDDATA:
            if (s_axi_RREADY & s_axi_RVALID)
                rnext = RDIDLE;
            else
                rnext = RDDATA;
        default:
            rnext = RDIDLE;
    endcase
end

// rdata
always @(posedge clk) begin
    if (ar_hs) begin
        rdata <= 1'b0;
        case (raddr)
            `ADDR_AP_CTRL: begin
                rdata[0] <= data_collection_start;
                rdata[1] <= reg_data_collection_idle;
                rdata[2] <= data_collection_cancel;
                rdata[8] <= reg_eth_stat_rx_status_2;
                rdata[9] <= reg_eth_stat_rx_aligned_2;
                if (mm2s_error)
                    rdata[10] <= 1;
                if (s2mm_error)
                    rdata[11] <= 1;
                rdata[12] <= reg_hbm_temp_trip;
                rdata[13] <= reg_hbm_temp_trip_ever;
                rdata[14] <= apb_complete_0;
                rdata[15] <= reg_eth_stat_rx_packet_bad_fcs_ever;
                rdata[16] <= mailbox_interrupt_0;
                rdata[17] <= mailbox_interrupt_1;
                rdata[18] <= !reg_qsfpdd_modprs;
                rdata[27:24] <= reg_host_writer_err;
            end
            `ADDR_MAC_ADDR_HI: begin
                rdata <= fpga_mac_addr[47:32];
            end
            `ADDR_MAC_ADDR_LO: begin
                rdata <= fpga_mac_addr[31:0];
            end
            `ADDR_IPV4_ADDR: begin
                rdata <= fpga_ipv4_addr;
            end
            `ADDR_DATA_COL_MODE: begin
                rdata <= data_collection_mode;
            end
            `ADDR_ONE_OVER_ENERGY: begin
                rdata <= one_over_energy;
            end
            `ADDR_FRAMES_PER_TRIG: begin
                rdata <= frames_per_trigger;
            end
            `ADDR_NMODULES: begin
                rdata <= nmodules;
            end
            `ADDR_ACTION_TYPE: begin
                rdata <= `ACTION_TYPE;
            end
            `ADDR_RELEASE_LEVEL: begin
                rdata <= `RELEASE_LEVEL;
            end
            `ADDR_GIT_SHA1: begin
                rdata <= `GIT_SHA1;
            end
            `ADDR_MAX_MODULES_FPGA: begin
                rdata <= `MAX_MODULES_FPGA;
            end
            `ADDR_MODS_INT_PKT_GEN: begin
                rdata <= `MODULES_INTERNAL_PACKET_GEN;
            end
            `ADDR_HBM_TEMP: begin
                rdata <= reg_hbm_temperature;
            end
            `ADDR_HBM_MAX_TEMP: begin
                rdata <= reg_hbm_max_temperature;
            end
            `ADDR_STALLS_HBM_HI: begin
                rdata <= reg_stalls_hbm[63:32];
            end
            `ADDR_STALLS_HBM_LO: begin
                rdata <= reg_stalls_hbm[31:0];
            end
            `ADDR_STALLS_HOST_HI: begin
                rdata <= reg_stalls_host[63:32];
            end
            `ADDR_STALLS_HOST_LO: begin
                rdata <= reg_stalls_host[31:0];
            end
            `ADDR_PACKETS_ETH_HI: begin
                rdata <= reg_packets_eth[63:32];
            end
            `ADDR_PACKETS_ETH_LO: begin
                rdata <= reg_packets_eth[31:0];
            end
            `ADDR_PACKETS_UDP_HI: begin
                rdata <= reg_packets_udp[63:32];
            end
            `ADDR_PACKETS_UDP_LO: begin
                rdata <= reg_packets_udp[31:0];
            end
            `ADDR_PACKETS_ICMP_HI: begin
                rdata <= reg_packets_icmp[63:32];
            end
            `ADDR_PACKETS_ICMP_LO: begin
                rdata <= reg_packets_icmp[31:0];
            end
            `ADDR_PACKETS_PROC_HI: begin
                rdata <= reg_packets_processed[63:32];
            end
            `ADDR_PACKETS_PROC_LO: begin
                rdata <= reg_packets_processed[31:0];
            end
            `ADDR_FIFO_STATUS: begin
                rdata <= reg_fifo_status;
            end
        endcase
    end
end


//------------------------Register logic-----------------

initial data_collection_start = 1'b0;
initial data_collection_cancel = 1'b0;

always @(posedge clk) begin
    if (!resetn)
        data_collection_start <= 1'b0;
    else if (!reg_data_collection_idle)
        data_collection_start <= 1'b0;
    else if (w_hs && waddr == `ADDR_AP_CTRL && s_axi_WSTRB[0] && s_axi_WDATA[0])
        data_collection_start <= 1'b1;
end

always @(posedge clk) begin
    if (!resetn)
        data_collection_cancel <= 1'b0;
    else if (w_hs && waddr == `ADDR_AP_CTRL && s_axi_WSTRB[0])
        data_collection_cancel <= s_axi_WDATA[2];
end

always @(posedge clk) begin
    if (!resetn)
        reg_data_collection_idle <= 1'b1;
    else begin
        reg_data_collection_idle <= data_collection_idle;
    end
end

always @(posedge clk) begin
    if (!resetn)
        data_collection_mode <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_DATA_COL_MODE)
            data_collection_mode <= (s_axi_WDATA[31:0] & wmask) | (data_collection_mode & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        fpga_mac_addr[47:32] <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_MAC_ADDR_HI )
            fpga_mac_addr[47:32] <= (s_axi_WDATA[31:0] & wmask) | (fpga_mac_addr[47:32] & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        fpga_mac_addr[31:0] <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_MAC_ADDR_LO )
            fpga_mac_addr[31:0] <= (s_axi_WDATA[31:0] & wmask) | (fpga_mac_addr[31:0] & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        fpga_ipv4_addr <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_IPV4_ADDR )
            fpga_ipv4_addr <= (s_axi_WDATA[31:0] & wmask) | (fpga_ipv4_addr & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        one_over_energy <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_ONE_OVER_ENERGY )
            one_over_energy <= (s_axi_WDATA[31:0] & wmask) | (one_over_energy & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        frames_per_trigger <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_FRAMES_PER_TRIG )
            frames_per_trigger <= (s_axi_WDATA[31:0] & wmask) | (frames_per_trigger & !wmask);
    end
end

always @(posedge clk) begin
    if (!resetn)
        nmodules <= 0;
    else if (reg_data_collection_idle) begin
        if (w_hs && waddr == `ADDR_NMODULES )
            nmodules <= (s_axi_WDATA[7:0] & wmask[7:0]) | (nmodules & !wmask[7:0]);
    end
end

always @ (posedge clk) begin
    reg_eth_stat_rx_status_1  <= eth_stat_rx_status;
    reg_eth_stat_rx_status_2  <= reg_eth_stat_rx_status_1;

    reg_eth_stat_rx_aligned_1 <= eth_stat_rx_aligned;
    reg_eth_stat_rx_aligned_2 <= reg_eth_stat_rx_aligned_1;

    reg_eth_stat_rx_packet_bad_fcs_1 <= eth_stat_rx_packet_bad_fcs;
    reg_eth_stat_rx_packet_bad_fcs_2 <= reg_eth_stat_rx_packet_bad_fcs_1;

    reg_eth_busy_1            <= eth_busy;
    reg_eth_busy_2            <= reg_eth_busy_1;

    reg_hbm_temperature       <= hbm_temperature;
    reg_hbm_temp_trip         <= hbm_temp_trip;
end

always @ (posedge clk) begin
    if (stalls_hbm_valid)
        reg_stalls_hbm <= stalls_hbm;
    if (stalls_host_valid)
        reg_stalls_host <= stalls_host;
    if (packets_processed_valid)
        reg_packets_processed <= packets_processed;
    if (packets_eth_valid)
        reg_packets_eth <= packets_eth;
    if (packets_udp_valid)
        reg_packets_udp <= packets_udp;
    if (packets_icmp_valid)
        reg_packets_icmp <= packets_icmp;
    if (host_writer_err_valid)
        reg_host_writer_err <= host_writer_err;
end

// HBM temperature trip is only cleared on card restart to protect the card
always @ (posedge clk) begin
    if (!resetn)
        reg_hbm_temp_trip_ever <= 0;
    else if (hbm_temp_trip)
        reg_hbm_temp_trip_ever <= 1;
end

// Ethernet RX packet bad FCS is kept from last reset
always @ (posedge clk) begin
    if (!resetn)
        reg_eth_stat_rx_packet_bad_fcs_ever <= 0;
    else if (reg_eth_stat_rx_packet_bad_fcs_2)
        reg_eth_stat_rx_packet_bad_fcs_ever <= 1;
end

always @ (posedge clk) begin
    if (!resetn)
        reg_qsfpdd_modprs <= 0;
    else
        reg_qsfpdd_modprs <= qsfpdd_modprs;
end

// Max temperature is calculated from card restart
always @ (posedge clk) begin
    if (!resetn)
        reg_hbm_max_temperature <= 0;
    else if (reg_hbm_temperature > reg_hbm_max_temperature)
        reg_hbm_max_temperature <= reg_hbm_temperature;
end

// FIFO status
always @(posedge clk) begin
    if (!resetn)
        reg_fifo_status <= 32'h0;
    else
        begin
        reg_fifo_status[0] <= pede2gain_data_fifo_empty;
        reg_fifo_status[1] <= pede2gain_data_fifo_full;
        reg_fifo_status[2] <= pede2gain_addr_fifo_empty;
        reg_fifo_status[3] <= pede2gain_addr_fifo_full;
        reg_fifo_status[6] <= udp_fifo_empty;
        reg_fifo_status[7] <= udp_fifo_full;
        reg_fifo_status[8] <= host_mem_data_fifo_empty;
        reg_fifo_status[9] <= host_mem_data_fifo_full;
        reg_fifo_status[10] <= host_mem_cmd_fifo_empty;
        reg_fifo_status[11] <= host_mem_cmd_fifo_full;
        reg_fifo_status[12] <= work_req_fifo_empty;
        reg_fifo_status[13] <= work_req_fifo_full;
        reg_fifo_status[14] <= work_compl_fifo_empty;
        reg_fifo_status[15] <= work_compl_fifo_full;
        reg_fifo_status[16] <= last_data_fifo_empty;
        reg_fifo_status[17] <= last_data_fifo_full;
        reg_fifo_status[18] <= last_addr_fifo_empty;
        reg_fifo_status[19] <= last_addr_fifo_full;
        end
end

assign qsfp_led_conn = !reg_eth_stat_rx_status_2;
assign qsfp_led_busy = !reg_eth_busy_2;

endmodule
