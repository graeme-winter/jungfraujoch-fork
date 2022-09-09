## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

# Hierarchical cell: mac_100g
proc create_hier_cell_mac_100g { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_mac_100g() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
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
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 m_axis_eth_in

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 qsfp

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 qsfp_ref

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_eth_out


  # Create pins
  create_bd_pin -dir I -type rst ap_rst_n
  create_bd_pin -dir I -type clk axiclk
  create_bd_pin -dir O eth_busy
  create_bd_pin -dir I -type clk refclk100
  create_bd_pin -dir I -type rst resetn
  create_bd_pin -dir O stat_rx_aligned
  create_bd_pin -dir O stat_rx_packet_bad_fcs
  create_bd_pin -dir O stat_rx_status

  # Create instance: axis_data_fifo_rx_0, and set properties
  set axis_data_fifo_rx_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_rx_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
   CONFIG.IS_ACLK_ASYNC {1} \
 ] $axis_data_fifo_rx_0

  # Create instance: axis_data_fifo_tx, and set properties
  set axis_data_fifo_tx [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_tx ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MODE {2} \
   CONFIG.IS_ACLK_ASYNC {1} \
 ] $axis_data_fifo_tx

  # Create instance: check_eth_busy_0, and set properties
  set block_name check_eth_busy
  set block_cell_name check_eth_busy_0
  if { [catch {set check_eth_busy_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $check_eth_busy_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: cmac_usplus_0, and set properties
  set cmac_usplus_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:cmac_usplus:3.1 cmac_usplus_0 ]
  set_property -dict [ list \
   CONFIG.CMAC_CAUI4_MODE {1} \
   CONFIG.CMAC_CORE_SELECT {CMACE4_X0Y3} \
   CONFIG.ENABLE_AXI_INTERFACE {1} \
   CONFIG.ENABLE_PIPELINE_REG {1} \
   CONFIG.GT_DRP_CLK {100.00} \
   CONFIG.GT_GROUP_SELECT {X0Y24~X0Y27} \
   CONFIG.GT_REF_CLK_FREQ {161.1328125} \
   CONFIG.INCLUDE_RS_FEC {1} \
   CONFIG.INCLUDE_STATISTICS_COUNTERS {1} \
   CONFIG.NUM_LANES {4x25} \
   CONFIG.RX_EQ_MODE {AUTO} \
   CONFIG.RX_FLOW_CONTROL {0} \
   CONFIG.TX_FLOW_CONTROL {0} \
   CONFIG.USER_INTERFACE {AXIS} \
 ] $cmac_usplus_0

  # Create instance: resetn_sync_0, and set properties
  set block_name resetn_sync
  set block_cell_name resetn_sync_0
  if { [catch {set resetn_sync_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $resetn_sync_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: util_vector_logic_0, and set properties
  set util_vector_logic_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_0 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_0

  # Create instance: util_vector_logic_1, and set properties
  set util_vector_logic_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_1 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_1

  # Create instance: zero_0, and set properties
  set zero_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 zero_0 ]
  set_property -dict [ list \
   CONFIG.CONST_VAL {0} \
 ] $zero_0

  # Create interface connections
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins qsfp] [get_bd_intf_pins cmac_usplus_0/gt_serial_port]
  connect_bd_intf_net -intf_net axis_data_fifo_rx_0_M_AXIS [get_bd_intf_pins m_axis_eth_in] [get_bd_intf_pins axis_data_fifo_rx_0/M_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_tx_M_AXIS [get_bd_intf_pins axis_data_fifo_tx/M_AXIS] [get_bd_intf_pins cmac_usplus_0/axis_tx]
  connect_bd_intf_net -intf_net axis_switch_0_M00_AXIS [get_bd_intf_pins s_axis_eth_out] [get_bd_intf_pins axis_data_fifo_tx/S_AXIS]
  connect_bd_intf_net -intf_net check_eth_busy_0_M_AXIS [get_bd_intf_pins axis_data_fifo_rx_0/S_AXIS] [get_bd_intf_pins check_eth_busy_0/M_AXIS]
  connect_bd_intf_net -intf_net cmac_usplus_0_axis_rx [get_bd_intf_pins check_eth_busy_0/S_AXIS] [get_bd_intf_pins cmac_usplus_0/axis_rx]
  connect_bd_intf_net -intf_net gt_ref_clk_0_1 [get_bd_intf_pins qsfp_ref] [get_bd_intf_pins cmac_usplus_0/gt_ref_clk]
  connect_bd_intf_net -intf_net s_axi_1 [get_bd_intf_pins s_axi] [get_bd_intf_pins cmac_usplus_0/s_axi]

  # Create port connections
  connect_bd_net -net ap_rst_n_1 [get_bd_pins ap_rst_n] [get_bd_pins resetn_sync_0/in_resetn]
  connect_bd_net -net axi_clk_net [get_bd_pins axiclk] [get_bd_pins axis_data_fifo_rx_0/m_axis_aclk] [get_bd_pins axis_data_fifo_tx/s_axis_aclk]
  connect_bd_net -net check_eth_busy_0_eth_busy [get_bd_pins eth_busy] [get_bd_pins check_eth_busy_0/eth_busy]
  connect_bd_net -net cmac_usplus_0_gt_txusrclk2 [get_bd_pins axis_data_fifo_rx_0/s_axis_aclk] [get_bd_pins axis_data_fifo_tx/m_axis_aclk] [get_bd_pins check_eth_busy_0/clk] [get_bd_pins cmac_usplus_0/gt_txusrclk2] [get_bd_pins cmac_usplus_0/rx_clk]
  connect_bd_net -net cmac_usplus_0_stat_rx_aligned [get_bd_pins stat_rx_aligned] [get_bd_pins cmac_usplus_0/stat_rx_aligned]
  connect_bd_net -net cmac_usplus_0_stat_rx_packet_bad_fcs [get_bd_pins stat_rx_packet_bad_fcs] [get_bd_pins cmac_usplus_0/stat_rx_packet_bad_fcs]
  connect_bd_net -net cmac_usplus_0_usr_rx_reset [get_bd_pins cmac_usplus_0/usr_rx_reset] [get_bd_pins util_vector_logic_1/Op1]
  connect_bd_net -net eth_stat_rx_status_1 [get_bd_pins stat_rx_status] [get_bd_pins cmac_usplus_0/stat_rx_status]
  connect_bd_net -net init_clk_1 [get_bd_pins refclk100] [get_bd_pins cmac_usplus_0/init_clk] [get_bd_pins cmac_usplus_0/s_axi_aclk] [get_bd_pins resetn_sync_0/clk]
  connect_bd_net -net resetn_1 [get_bd_pins resetn] [get_bd_pins axis_data_fifo_tx/s_axis_aresetn]
  connect_bd_net -net resetn_sync_0_out_resetn [get_bd_pins resetn_sync_0/out_resetn] [get_bd_pins util_vector_logic_0/Op1]
  connect_bd_net -net util_vector_logic_0_Res [get_bd_pins cmac_usplus_0/s_axi_sreset] [get_bd_pins cmac_usplus_0/sys_reset] [get_bd_pins util_vector_logic_0/Res]
  connect_bd_net -net util_vector_logic_1_Res [get_bd_pins axis_data_fifo_rx_0/s_axis_aresetn] [get_bd_pins util_vector_logic_1/Res]
  connect_bd_net -net zero_0_dout [get_bd_pins cmac_usplus_0/core_drp_reset] [get_bd_pins cmac_usplus_0/core_rx_reset] [get_bd_pins cmac_usplus_0/core_tx_reset] [get_bd_pins cmac_usplus_0/drp_clk] [get_bd_pins zero_0/dout]

  # Restore current instance
  current_bd_instance $oldCurInst
}