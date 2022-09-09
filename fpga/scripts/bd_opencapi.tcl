## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

## Env Variables

set hls_ip_dir    @HLS_IP_DIR@
set src_dir       @SRC_DIR@
set_property ip_repo_paths [concat [get_property ip_repo_paths [current_project]] ${hls_ip_dir} "${src_dir}/ad9h3-bci"] [current_project]

create_bd_design "action"

# Hierarchical cell: aximm_host_infrastructure
proc create_hier_cell_aximm_host_infrastructure { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_msg_id "BD_TCL-102" "ERROR" "create_hier_cell_aximm_host_infrastructure() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 M_AXIS_MM2S

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 S_AXIS_MM2S_CMD

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 S_AXIS_S2MM_CMD

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 S_AXIS_S2MM_DATA

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_host_mem


  # Create pins
  create_bd_pin -dir I -type clk ap_clk
  create_bd_pin -dir I -type rst ap_rst_n
  create_bd_pin -dir O err_encountered_mm2s
  create_bd_pin -dir O err_encountered_s2mm
  create_bd_pin -dir O host_mem_data_fifo_empty
  create_bd_pin -dir O host_mem_data_fifo_full
  create_bd_pin -dir O host_mem_cmd_fifo_empty
  create_bd_pin -dir O host_mem_cmd_fifo_full

  # Create instance: axi_datamover_0, and set properties
  set axi_datamover_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_datamover:* axi_datamover_0 ]
  set_property -dict [ list \
   CONFIG.c_addr_width {64} \
   CONFIG.c_dummy {1} \
   CONFIG.c_enable_cache_user {false} \
   CONFIG.c_m_axi_mm2s_data_width {1024} \
   CONFIG.c_m_axi_s2mm_data_width {1024} \
   CONFIG.c_m_axis_mm2s_tdata_width {512} \
   CONFIG.c_mm2s_btt_used {23} \
   CONFIG.c_mm2s_burst_size {32} \
   CONFIG.c_s2mm_btt_used {23} \
   CONFIG.c_s2mm_burst_size {32} \
   CONFIG.c_s_axis_s2mm_tdata_width {1024} \
   CONFIG.c_single_interface {1} \
 ] $axi_datamover_0

  # Create instance: axis_data_fifo_s2mm_cmd, and set properties
  set axis_data_fifo_s2mm_cmd [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:* axis_data_fifo_s2mm_cmd ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_s2mm_cmd

  # Create instance: axis_data_fifo_s2mm_data, and set properties
  set axis_data_fifo_s2mm_data [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:* axis_data_fifo_s2mm_data ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
   CONFIG.FIFO_MEMORY_TYPE {block} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_s2mm_data

  # Create instance: axis_dwidth_converter_0, and set properties
  set axis_dwidth_converter_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_dwidth_converter:* axis_dwidth_converter_0 ]
  set_property -dict [ list \
   CONFIG.HAS_MI_TKEEP {1} \
   CONFIG.HAS_TSTRB {0} \
   CONFIG.M_TDATA_NUM_BYTES {128} \
 ] $axis_dwidth_converter_0

  # Create instance: check_datamover_error_mm2s, and set properties
  set block_name check_datamover_error
  set block_cell_name check_datamover_error_mm2s
  if { [catch {set check_datamover_error_mm2s [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $check_datamover_error_mm2s eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: check_datamover_error_s2mm, and set properties
  set block_name check_datamover_error
  set block_cell_name check_datamover_error_s2mm
  if { [catch {set check_datamover_error_s2mm [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $check_datamover_error_s2mm eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create interface connections
  connect_bd_intf_net -intf_net S_AXIS1_1 [get_bd_intf_pins S_AXIS_S2MM_DATA] [get_bd_intf_pins axis_dwidth_converter_0/S_AXIS]
  connect_bd_intf_net -intf_net axi_datamover_0_M_AXI [get_bd_intf_pins m_axi_host_mem] [get_bd_intf_pins axi_datamover_0/M_AXI]
  connect_bd_intf_net -intf_net axi_datamover_0_M_AXIS_MM2S [get_bd_intf_pins M_AXIS_MM2S] [get_bd_intf_pins axi_datamover_0/M_AXIS_MM2S]
  connect_bd_intf_net -intf_net axi_datamover_0_M_AXIS_MM2S_STS [get_bd_intf_pins axi_datamover_0/M_AXIS_MM2S_STS] [get_bd_intf_pins check_datamover_error_mm2s/S_AXIS]
  connect_bd_intf_net -intf_net axi_datamover_0_M_AXIS_S2MM_STS [get_bd_intf_pins axi_datamover_0/M_AXIS_S2MM_STS] [get_bd_intf_pins check_datamover_error_s2mm/S_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_0_M_AXIS [get_bd_intf_pins axi_datamover_0/S_AXIS_S2MM] [get_bd_intf_pins axis_data_fifo_s2mm_data/M_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_s2mm_cmd_M_AXIS [get_bd_intf_pins axi_datamover_0/S_AXIS_S2MM_CMD] [get_bd_intf_pins axis_data_fifo_s2mm_cmd/M_AXIS]
  connect_bd_intf_net -intf_net axis_dwidth_converter_0_M_AXIS [get_bd_intf_pins axis_data_fifo_s2mm_data/S_AXIS] [get_bd_intf_pins axis_dwidth_converter_0/M_AXIS]
  connect_bd_intf_net -intf_net hls_jf_action_0_in_datamover_cmd_stream_V_data_V [get_bd_intf_pins S_AXIS_MM2S_CMD] [get_bd_intf_pins axi_datamover_0/S_AXIS_MM2S_CMD]
  connect_bd_intf_net -intf_net hls_jf_action_0_out_datamover_cmd_stream_V_data_V [get_bd_intf_pins S_AXIS_S2MM_CMD] [get_bd_intf_pins axis_data_fifo_s2mm_cmd/S_AXIS]

  # Create port connections
  connect_bd_net -net ap_clk_1 [get_bd_pins ap_clk] [get_bd_pins axi_datamover_0/m_axi_mm2s_aclk] [get_bd_pins axi_datamover_0/m_axi_s2mm_aclk] [get_bd_pins axi_datamover_0/m_axis_mm2s_cmdsts_aclk] [get_bd_pins axi_datamover_0/m_axis_s2mm_cmdsts_awclk] [get_bd_pins axis_data_fifo_s2mm_cmd/s_axis_aclk] [get_bd_pins axis_data_fifo_s2mm_data/s_axis_aclk] [get_bd_pins axis_dwidth_converter_0/aclk] [get_bd_pins check_datamover_error_mm2s/clk] [get_bd_pins check_datamover_error_s2mm/clk]
  connect_bd_net -net ap_rst_n_1 [get_bd_pins ap_rst_n] [get_bd_pins axi_datamover_0/m_axi_mm2s_aresetn] [get_bd_pins axi_datamover_0/m_axi_s2mm_aresetn] [get_bd_pins axi_datamover_0/m_axis_mm2s_cmdsts_aresetn] [get_bd_pins axi_datamover_0/m_axis_s2mm_cmdsts_aresetn] [get_bd_pins axis_data_fifo_s2mm_cmd/s_axis_aresetn] [get_bd_pins axis_data_fifo_s2mm_data/s_axis_aresetn] [get_bd_pins axis_dwidth_converter_0/aresetn] [get_bd_pins check_datamover_error_mm2s/resetn] [get_bd_pins check_datamover_error_s2mm/resetn]
  connect_bd_net -net check_datamover_error_mm2s_err_encountered [get_bd_pins err_encountered_mm2s] [get_bd_pins check_datamover_error_mm2s/err_encountered]
  connect_bd_net -net check_datamover_error_s2mm_err_encountered [get_bd_pins err_encountered_s2mm] [get_bd_pins check_datamover_error_s2mm/err_encountered]

  connect_bd_net [get_bd_pins host_mem_data_fifo_empty] [get_bd_pins axis_data_fifo_s2mm_data/almost_empty]
  connect_bd_net [get_bd_pins host_mem_data_fifo_full] [get_bd_pins axis_data_fifo_s2mm_data/almost_full]
  connect_bd_net [get_bd_pins host_mem_cmd_fifo_empty] [get_bd_pins axis_data_fifo_s2mm_cmd/almost_empty]
  connect_bd_net [get_bd_pins host_mem_cmd_fifo_full] [get_bd_pins axis_data_fifo_s2mm_cmd/almost_full]

  # Restore current instance
  current_bd_instance $oldCurInst
}

source ${src_dir}/scripts/hbm.tcl
source ${src_dir}/scripts/network_stack.tcl
source ${src_dir}/scripts/jfjoch.tcl
source ${src_dir}/scripts/mac_100g.tcl

# Create interface ports


set uc [ create_bd_intf_port -mode Slave -vlnv alpha-data.com:user:avr_rtl:1.0 uc ]

set gt_ref [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 gt_ref ]
set_property -dict [ list \
CONFIG.FREQ_HZ {161132812} \
] $gt_ref

set gt_100g [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 gt_100g ]

set qsfpdd_modprs [ create_bd_port -dir I qsfpdd_modprs ]

set m_axi_host_mem [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_host_mem ]
set_property -dict [ list \
CONFIG.ADDR_WIDTH {64} \
CONFIG.DATA_WIDTH {1024} \
CONFIG.FREQ_HZ {200000000} \
CONFIG.NUM_READ_OUTSTANDING {24} \
CONFIG.NUM_WRITE_OUTSTANDING {8} \
CONFIG.PROTOCOL {AXI4} \
] $m_axi_host_mem

set ref300 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 ref300 ]
set_property -dict [ list \
CONFIG.FREQ_HZ {300000000} \
] $ref300

set s_axi_ctrl_reg [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi_ctrl_reg ]
set_property -dict [ list \
CONFIG.ADDR_WIDTH {32} \
CONFIG.ARUSER_WIDTH {0} \
CONFIG.AWUSER_WIDTH {0} \
CONFIG.BUSER_WIDTH {0} \
CONFIG.DATA_WIDTH {32} \
CONFIG.FREQ_HZ {200000000} \
CONFIG.HAS_BRESP {1} \
CONFIG.HAS_BURST {1} \
CONFIG.HAS_CACHE {1} \
CONFIG.HAS_LOCK {1} \
CONFIG.HAS_PROT {1} \
CONFIG.HAS_QOS {1} \
CONFIG.HAS_REGION {1} \
CONFIG.HAS_RRESP {1} \
CONFIG.HAS_WSTRB {1} \
CONFIG.ID_WIDTH {0} \
CONFIG.MAX_BURST_LENGTH {1} \
CONFIG.NUM_READ_OUTSTANDING {1} \
CONFIG.NUM_READ_THREADS {1} \
CONFIG.NUM_WRITE_OUTSTANDING {1} \
CONFIG.NUM_WRITE_THREADS {1} \
CONFIG.PROTOCOL {AXI4LITE} \
CONFIG.READ_WRITE_MODE {READ_WRITE} \
CONFIG.RUSER_BITS_PER_BYTE {0} \
CONFIG.RUSER_WIDTH {0} \
CONFIG.SUPPORTS_NARROW_BURST {0} \
CONFIG.WUSER_BITS_PER_BYTE {0} \
CONFIG.WUSER_WIDTH {0} \
] $s_axi_ctrl_reg


# Create ports
set ap_clk [ create_bd_port -dir I -type clk -freq_hz 200000000 ap_clk ]
set_property -dict [ list \
CONFIG.ASSOCIATED_BUSIF {m_axi_host_mem:s_axi_ctrl_reg} \
] $ap_clk
set ap_rst_n [ create_bd_port -dir I -type rst ap_rst_n ]
set user_led_a0 [ create_bd_port -dir O user_led_a0 ]
set user_led_a1 [ create_bd_port -dir O user_led_a1 ]
set user_led_g0 [ create_bd_port -dir O user_led_g0 ]
set user_led_g1 [ create_bd_port -dir O user_led_g1 ]

# Create instance: aximm_host_infrastructure
create_hier_cell_aximm_host_infrastructure [current_bd_instance .] aximm_host_infrastructure

# Create instance: bci_admpcie9h3_0, and set properties
set bci_admpcie9h3_0 [ create_bd_cell -type ip -vlnv alpha-data.com:user:bci_admpcie9h3:2.0 bci_admpcie9h3_0 ]

# Create instance: hbm_infrastructure
create_hier_cell_hbm_infrastructure [current_bd_instance .] hbm_infrastructure RIGHT

# Create instance: jungfraujoch
create_hier_cell_jungfraujoch [current_bd_instance .] jungfraujoch

# Create instance: mac_100g
create_hier_cell_mac_100g [current_bd_instance .] mac_100g

# Create instance: refclk_ibufds_inst, and set properties
set refclk_ibufds_inst [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:* refclk_ibufds_inst ]
set_property -dict [ list \
CONFIG.C_BUF_TYPE {IBUFDS} \
] $refclk_ibufds_inst

# Create instance: proc_sys_reset_0, and set properties
set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:* proc_sys_reset_0 ]
set_property -dict [ list \
  CONFIG.C_EXT_RST_WIDTH {1} \
] $proc_sys_reset_0

set one [create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:* one]
  # Create instance: smartconnect_0, and set properties
  set smartconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_0 ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {2} \
   CONFIG.NUM_MI {3} \
   CONFIG.NUM_SI {1} \
 ] $smartconnect_0

# Create instance: refclk300to100, and set properties
  set block_name refclk300to100
  set block_cell_name refclk300to100
  if { [catch {set refclk300to100 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
  } elseif { $refclk300to100 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
  }

  # Create interface connections
  connect_bd_intf_net -intf_net S_AXIS_MM2S_CMD_1 [get_bd_intf_pins aximm_host_infrastructure/S_AXIS_MM2S_CMD] [get_bd_intf_pins jungfraujoch/m_axis_h2c_datamover_cmd]
  connect_bd_intf_net -intf_net S_AXIS_S2MM_DATA_1 [get_bd_intf_pins aximm_host_infrastructure/S_AXIS_S2MM_DATA] [get_bd_intf_pins jungfraujoch/m_axis_c2h_data]
  connect_bd_intf_net -intf_net axi_host_mem_aximm_host_infra_0_M_AXI [get_bd_intf_ports m_axi_host_mem] [get_bd_intf_pins aximm_host_infrastructure/m_axi_host_mem]
  connect_bd_intf_net -intf_net aximm_host_infrastructure_M_AXIS_MM2S [get_bd_intf_pins aximm_host_infrastructure/M_AXIS_MM2S] [get_bd_intf_pins jungfraujoch/s_axis_h2c_data]
  connect_bd_intf_net -intf_net eth_in_1 [get_bd_intf_pins jungfraujoch/eth_in] [get_bd_intf_pins mac_100g/m_axis_eth_in]
  connect_bd_intf_net -intf_net gt_ref_1 [get_bd_intf_ports gt_ref] [get_bd_intf_pins mac_100g/qsfp_ref]
  connect_bd_intf_net -intf_net jungfraujoch_eth_out [get_bd_intf_pins jungfraujoch/eth_out] [get_bd_intf_pins mac_100g/s_axis_eth_out]
  connect_bd_intf_net -intf_net mac_100g_qsfp0 [get_bd_intf_ports gt_100g] [get_bd_intf_pins mac_100g/qsfp]
  connect_bd_intf_net -intf_net ref300_1 [get_bd_intf_ports ref300] [get_bd_intf_pins refclk_ibufds_inst/CLK_IN_D]
  connect_bd_intf_net -intf_net s_axi_1 [get_bd_intf_pins jungfraujoch/s_axi] [get_bd_intf_pins smartconnect_0/M00_AXI]
  connect_bd_intf_net -intf_net s_axi_2 [get_bd_intf_pins mac_100g/s_axi] [get_bd_intf_pins smartconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net s_axi_ctrl_reg_1 [get_bd_intf_ports s_axi_ctrl_reg] [get_bd_intf_pins smartconnect_0/S00_AXI]
  connect_bd_intf_net -intf_net s_axi_hbm_0_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_0] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p0]
  connect_bd_intf_net -intf_net s_axi_hbm_1_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_1] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p1]
  connect_bd_intf_net -intf_net s_axi_hbm_10_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_10] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p10]
  connect_bd_intf_net -intf_net s_axi_hbm_11_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_11] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p11]
  connect_bd_intf_net -intf_net s_axi_hbm_2_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_2] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p2]
  connect_bd_intf_net -intf_net s_axi_hbm_3_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_3] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p3]
  connect_bd_intf_net -intf_net s_axi_hbm_4_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_4] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p4]
  connect_bd_intf_net -intf_net s_axi_hbm_5_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_5] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p5]
  connect_bd_intf_net -intf_net s_axi_hbm_6_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_6] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p6]
  connect_bd_intf_net -intf_net s_axi_hbm_7_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_7] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p7]
  connect_bd_intf_net -intf_net s_axi_hbm_8_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_8] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p8]
  connect_bd_intf_net -intf_net s_axi_hbm_9_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_9] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p9]
  connect_bd_intf_net -intf_net smartconnect_0_M02_AXI [get_bd_intf_pins bci_admpcie9h3_0/axil] [get_bd_intf_pins smartconnect_0/M01_AXI]
  connect_bd_intf_net -intf_net uc_1 [get_bd_intf_ports uc] [get_bd_intf_pins bci_admpcie9h3_0/avr]
  connect_bd_intf_net -intf_net write_data_0_datamover_out_cmd_V_data_V [get_bd_intf_pins aximm_host_infrastructure/S_AXIS_S2MM_CMD] [get_bd_intf_pins jungfraujoch/m_axis_c2h_datamover_cmd]

  # Create port connections
  connect_bd_net -net ap_clk_1 [get_bd_ports ap_clk] [get_bd_pins aximm_host_infrastructure/ap_clk] [get_bd_pins bci_admpcie9h3_0/aclk] [get_bd_pins hbm_infrastructure/clk] [get_bd_pins jungfraujoch/axi_clk] [get_bd_pins mac_100g/axiclk] [get_bd_pins proc_sys_reset_0/slowest_sync_clk] [get_bd_pins smartconnect_0/aclk]
  connect_bd_net -net ap_rst_n_2 [get_bd_ports ap_rst_n] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net axilite_ctrl_infrastructure_user_led_a1 [get_bd_ports user_led_a1] [get_bd_pins jungfraujoch/qsfp_led_busy]
  connect_bd_net -net axilite_ctrl_infrastructure_user_led_g1 [get_bd_ports user_led_g1] [get_bd_pins jungfraujoch/qsfp_led_conn]
  connect_bd_net -net aximm_host_infrastructure_err_encountered [get_bd_pins aximm_host_infrastructure/err_encountered_mm2s] [get_bd_pins jungfraujoch/mm2s_error]
  connect_bd_net -net aximm_host_infrastructure_err_encountered1 [get_bd_pins aximm_host_infrastructure/err_encountered_s2mm] [get_bd_pins jungfraujoch/s2mm_error]
  connect_bd_net -net aximm_host_infrastructure_host_mem_cmd_fifo_empty [get_bd_pins aximm_host_infrastructure/host_mem_cmd_fifo_empty] [get_bd_pins jungfraujoch/host_mem_cmd_fifo_empty]
  connect_bd_net -net aximm_host_infrastructure_host_mem_cmd_fifo_full [get_bd_pins aximm_host_infrastructure/host_mem_cmd_fifo_full] [get_bd_pins jungfraujoch/host_mem_cmd_fifo_full]
  connect_bd_net -net aximm_host_infrastructure_host_mem_data_fifo_empty [get_bd_pins aximm_host_infrastructure/host_mem_data_fifo_empty] [get_bd_pins jungfraujoch/host_mem_data_fifo_empty]
  connect_bd_net -net aximm_host_infrastructure_host_mem_data_fifo_full [get_bd_pins aximm_host_infrastructure/host_mem_data_fifo_full] [get_bd_pins jungfraujoch/host_mem_data_fifo_full]
  connect_bd_net -net eth_stat_rx_aligned_1 [get_bd_pins jungfraujoch/eth_stat_rx_aligned] [get_bd_pins mac_100g/stat_rx_aligned]
  connect_bd_net -net eth_stat_rx_packet_bad_fcs_1 [get_bd_pins jungfraujoch/eth_stat_rx_packet_bad_fcs] [get_bd_pins mac_100g/stat_rx_packet_bad_fcs]
  connect_bd_net -net hbm_infrastructure_apb_complete_0 [get_bd_pins hbm_infrastructure/apb_complete_0] [get_bd_pins jungfraujoch/apb_complete]
  connect_bd_net -net hbm_infrastructure_hbm_temp_trip [get_bd_pins hbm_infrastructure/hbm_temp_trip_0] [get_bd_pins jungfraujoch/hbm_temp_trip]
  connect_bd_net -net hbm_infrastructure_hbm_temperature [get_bd_pins hbm_infrastructure/hbm_temperature_0] [get_bd_pins jungfraujoch/hbm_temperature]
  connect_bd_net -net mac_100g_eth_busy [get_bd_pins jungfraujoch/eth_busy] [get_bd_pins mac_100g/eth_busy]
  connect_bd_net -net mac_100g_stat_rx_status [get_bd_pins jungfraujoch/eth_stat_rx_status] [get_bd_pins mac_100g/stat_rx_status]
  connect_bd_net -net one_dout [get_bd_ports user_led_a0] [get_bd_ports user_led_g0] [get_bd_pins one/dout] [get_bd_pins proc_sys_reset_0/dcm_locked]
  connect_bd_net -net proc_sys_reset_0_interconnect_aresetn [get_bd_pins aximm_host_infrastructure/ap_rst_n] [get_bd_pins hbm_infrastructure/resetn] [get_bd_pins jungfraujoch/axi_rst_n] [get_bd_pins mac_100g/resetn] [get_bd_pins proc_sys_reset_0/interconnect_aresetn] [get_bd_pins smartconnect_0/aresetn]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins bci_admpcie9h3_0/aresetn] [get_bd_pins jungfraujoch/ap_rst_n] [get_bd_pins mac_100g/ap_rst_n] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
  connect_bd_net -net qsfpdd_modprs_1 [get_bd_ports qsfpdd_modprs] [get_bd_pins jungfraujoch/qsfpdd_modprs]
  connect_bd_net -net refclk300to100_refclk100 [get_bd_pins hbm_infrastructure/refclk100] [get_bd_pins mac_100g/refclk100] [get_bd_pins refclk300to100/refclk100] [get_bd_pins smartconnect_0/aclk1]
  connect_bd_net -net refclk_ibufds_inst_IBUF_OUT [get_bd_pins hbm_infrastructure/hbm_clk] [get_bd_pins refclk300to100/refclk300] [get_bd_pins refclk_ibufds_inst/IBUF_OUT]

# Create address segments
assign_bd_address -offset 0x00000000 -range 0x00010000000000000000 -target_address_space [get_bd_addr_spaces aximm_host_infrastructure/axi_datamover_0/Data] [get_bd_addr_segs m_axi_host_mem/Reg] -force
assign_bd_address -offset 0x00000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces s_axi_ctrl_reg] [get_bd_addr_segs jungfraujoch/action_config_0/s_axi/reg0] -force
assign_bd_address -offset 0x00040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces s_axi_ctrl_reg] [get_bd_addr_segs mac_100g/cmac_usplus_0/s_axi/Reg] -force
assign_bd_address -offset 0x00030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces s_axi_ctrl_reg] [get_bd_addr_segs jungfraujoch/mailbox_0/S0_AXI/Reg] -force
assign_bd_address -offset 0x00010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces s_axi_ctrl_reg] [get_bd_addr_segs bci_admpcie9h3_0/axil/reg0] -force
assign_bd_address -offset 0x00060000 -range 0x00002000 -target_address_space [get_bd_addr_spaces s_axi_ctrl_reg] [get_bd_addr_segs jungfraujoch/axi_bram_ctrl_0/S_AXI/Mem0] -force
assign_bd_address -offset 0x00000000 -range 0x00008000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/eth_ctrl/microblaze_0_local_memory/dlmb_bram_if_cntlr/SLMB/Mem] -force
assign_bd_address -offset 0x00040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/cmac_usplus_0/s_axi/Reg] -force
assign_bd_address -offset 0x00080000 -range 0x00010000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/eth_ctrl/axi_intc_0/S_AXI/Reg] -force
assign_bd_address -offset 0x00000000 -range 0x00008000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Instruction] [get_bd_addr_segs mac_100g/eth_ctrl/microblaze_0_local_memory/ilmb_bram_if_cntlr/SLMB/Mem] -force
assign_bd_address

set_property -dict [ list \
         CONFIG.CMAC_CORE_SELECT {CMACE4_X0Y1} \
         CONFIG.GT_GROUP_SELECT {X0Y8~X0Y11} \
] [get_bd_cells mac_100g/cmac_usplus_0]

set start_eth_elf_file [file normalize "${src_dir}/microblaze/start_eth.elf"]
add_files -norecurse $start_eth_elf_file
add_files -fileset sim_1 -norecurse [file normalize $start_eth_elf_file]
set_property SCOPED_TO_REF action [get_files -all -of_objects [get_fileset sources_1] $start_eth_elf_file]
set_property SCOPED_TO_CELLS { mac_100g/eth_ctrl/microblaze_0 } [get_files -all -of_objects [get_fileset sources_1] $start_eth_elf_file]
set_property SCOPED_TO_REF action [get_files -all -of_objects [get_fileset sim_1] $start_eth_elf_file]
set_property SCOPED_TO_CELLS { mac_100g/eth_ctrl/microblaze_0 } [get_files -all -of_objects [get_fileset sim_1] $start_eth_elf_file]

save_bd_design
validate_bd_design
save_bd_design

set_property synth_checkpoint_mode None [get_files ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/action.bd]
generate_target all                     [get_files ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/action.bd]

#add_files -norecurse ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/ip/action_hbm_0/hdl/rtl/xpm_internal_config_file_1.mem
#add_files -norecurse ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/ip/action_hbm_0/hdl/rtl/xpm_internal_config_file_0.mem

#import_files -fileset sim_1 -norecurse ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/ip/action_hbm_0/hdl/rtl/xpm_internal_config_file_sim_1.mem
#import_files -fileset sim_1 -norecurse ${src_dir}/oc-accel/hardware/viv_project/framework.srcs/sources_1/bd/action/ip/action_hbm_0/hdl/rtl/xpm_internal_config_file_sim_0.mem

add_files -fileset constrs_1 -norecurse ${src_dir}/xdc/oc_9h3.xdc

write_hw_platform -fixed -force -file jfjoch_opencapi.xsa

set_property -name {ies.simulate.ncsim.more_options} -value {+notimingchecks} -objects [get_filesets sim_1]
set_property -name {ies.elaborate.ncelab.more_options} -value {-access +rwc -notimingchecks} -objects [get_filesets sim_1]
set_property -name {ies.simulate.runtime} -value {1ms} -objects [get_filesets sim_1]

set_property flow {Vivado Synthesis 2022} [get_runs synth_1]
set_property strategy Flow_PerfOptimized_high [get_runs synth_1]
set_property strategy Performance_ExploreWithRemap [get_runs impl_1]
