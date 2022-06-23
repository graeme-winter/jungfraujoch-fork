## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

# Hierarchical cell: pcie_dma
proc create_hier_cell_pcie_dma { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_pcie_dma() - Empty argument(s)!"}
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
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_ctrl

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 m_axis_h2c_data

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:pcie_7x_mgt_rtl:1.0 pcie_mgt

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 pcie_refclk

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_c2h_cmd

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_c2h_data

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_h2c_cmd


  # Create pins
  create_bd_pin -dir O -type clk axi_aclk
  create_bd_pin -dir O -type rst axi_aresetn
  create_bd_pin -dir O host_mem_cmd_fifo_empty
  create_bd_pin -dir O host_mem_cmd_fifo_full
  create_bd_pin -dir O host_mem_data_fifo_empty
  create_bd_pin -dir O host_mem_data_fifo_full
  create_bd_pin -dir I -type rst pcie_perstn
  create_bd_pin -dir I -type rst s_axis_aresetn
  create_bd_pin -dir O user_lnk_up

  # Create instance: axi_firewall_0, and set properties
  set axi_firewall_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_firewall:1.2 axi_firewall_0 ]

  # Create instance: axis_data_fifo_c2h_cmd, and set properties
  set axis_data_fifo_c2h_cmd [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_c2h_cmd ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_c2h_cmd

  # Create instance: axis_data_fifo_c2h_data, and set properties
  set axis_data_fifo_c2h_data [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_c2h_data ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {512} \
   CONFIG.FIFO_MEMORY_TYPE {block} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_c2h_data

  # Create instance: axis_data_fifo_h2c_cmd, and set properties
  set axis_data_fifo_h2c_cmd [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_h2c_cmd ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {64} \
 ] $axis_data_fifo_h2c_cmd

  # Create instance: axis_data_fifo_h2c_data, and set properties
  set axis_data_fifo_h2c_data [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_h2c_data ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {128} \
   CONFIG.FIFO_MEMORY_TYPE {block} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
   CONFIG.TDATA_NUM_BYTES {64} \
 ] $axis_data_fifo_h2c_data

  # Create instance: gen_xdma_descriptor_0, and set properties
  set block_name gen_xdma_descriptor_c2h
  set block_cell_name gen_xdma_descriptor_0
  if { [catch {set gen_xdma_descriptor_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $gen_xdma_descriptor_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: gen_xdma_descriptor_1, and set properties
  set block_name gen_xdma_descriptor_h2c
  set block_cell_name gen_xdma_descriptor_1
  if { [catch {set gen_xdma_descriptor_1 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $gen_xdma_descriptor_1 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: pcie_clk_buf_inst, and set properties
  set pcie_clk_buf_inst [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 pcie_clk_buf_inst ]
  set_property -dict [ list \
   CONFIG.C_BUF_TYPE {IBUFDSGTE} \
 ] $pcie_clk_buf_inst

  # Create instance: smartconnect_0, and set properties
  set smartconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_0 ]
  set_property -dict [ list \
   CONFIG.NUM_MI {2} \
   CONFIG.NUM_SI {1} \
 ] $smartconnect_0

  # Create instance: xdma_0, and set properties
  set xdma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xdma:4.1 xdma_0 ]
  set_property -dict [ list \
     CONFIG.INS_LOSS_NYQ {5} \
     CONFIG.PF0_DEVICE_ID_mqdma {9048} \
     CONFIG.PF0_SRIOV_VF_DEVICE_ID {A048} \
     CONFIG.PF1_SRIOV_VF_DEVICE_ID {A148} \
     CONFIG.PF2_DEVICE_ID_mqdma {9248} \
     CONFIG.PF2_SRIOV_VF_DEVICE_ID {A248} \
     CONFIG.PF3_DEVICE_ID_mqdma {9348} \
     CONFIG.PF3_SRIOV_VF_DEVICE_ID {A348} \
     CONFIG.axi_data_width {512_bit} \
     CONFIG.axi_id_width {2} \
     CONFIG.axil_master_64bit_en {false} \
     CONFIG.axilite_master_en {true} \
     CONFIG.axilite_master_size {1} \
     CONFIG.axisten_freq {250} \
     CONFIG.cfg_mgmt_if {false} \
     CONFIG.copy_pf0 {true} \
     CONFIG.coreclk_freq {500} \
     CONFIG.dsc_bypass_rd {0001} \
     CONFIG.dsc_bypass_wr {0001} \
     CONFIG.en_gt_selection {false} \
     CONFIG.ins_loss_profile {Chip-to-Chip} \
     CONFIG.mode_selection {Advanced} \
     CONFIG.pcie_blk_locn {PCIE4C_X1Y0} \
     CONFIG.pf0_base_class_menu {Processing_accelerators} \
     CONFIG.pf0_class_code {120000} \
     CONFIG.pf0_class_code_base {12} \
     CONFIG.pf0_class_code_interface {00} \
     CONFIG.pf0_class_code_sub {00} \
     CONFIG.pf0_device_id {9048} \
     CONFIG.pf0_msix_cap_pba_bir {BAR_1} \
     CONFIG.pf0_msix_cap_pba_offset {00008FE0} \
     CONFIG.pf0_msix_cap_table_bir {BAR_1} \
     CONFIG.pf0_msix_cap_table_offset {00008000} \
     CONFIG.pf0_msix_cap_table_size {01F} \
     CONFIG.pf0_msix_enabled {true} \
     CONFIG.pf0_sub_class_interface_menu {Unknown} \
     CONFIG.pf0_subsystem_id {5232} \
     CONFIG.pf0_subsystem_vendor_id {10EE} \
     CONFIG.pf1_msix_cap_pba_offset {00000000} \
     CONFIG.pf1_msix_cap_table_offset {00000000} \
     CONFIG.pf1_msix_cap_table_size {000} \
     CONFIG.pl_link_cap_max_link_speed {16.0_GT/s} \
     CONFIG.pl_link_cap_max_link_width {X8} \
     CONFIG.plltype {QPLL0} \
     CONFIG.runbit_fix {false} \
     CONFIG.select_quad {GTY_Quad_227} \
     CONFIG.vendor_id {10EE} \
     CONFIG.xdma_axi_intf_mm {AXI_Stream} \
     CONFIG.xdma_axilite_slave {false} \
     CONFIG.xdma_wnum_chnl {1} \
   ] $xdma_0

  # Create interface connections
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins s_axis_c2h_data] [get_bd_intf_pins axis_data_fifo_c2h_data/S_AXIS]
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_pins m_axi_ctrl] [get_bd_intf_pins axi_firewall_0/M_AXI]
  connect_bd_intf_net -intf_net Conn3 [get_bd_intf_pins pcie_mgt] [get_bd_intf_pins xdma_0/pcie_mgt]
  connect_bd_intf_net -intf_net Conn4 [get_bd_intf_pins m_axis_h2c_data] [get_bd_intf_pins axis_data_fifo_h2c_data/M_AXIS]
  connect_bd_intf_net -intf_net Conn5 [get_bd_intf_pins pcie_refclk] [get_bd_intf_pins pcie_clk_buf_inst/CLK_IN_D]
  connect_bd_intf_net -intf_net Conn7 [get_bd_intf_pins s_axis_c2h_cmd] [get_bd_intf_pins axis_data_fifo_c2h_cmd/S_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_0_M_AXIS [get_bd_intf_pins axis_data_fifo_h2c_cmd/M_AXIS] [get_bd_intf_pins gen_xdma_descriptor_1/S_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_c2h_cmd_M_AXIS [get_bd_intf_pins axis_data_fifo_c2h_cmd/M_AXIS] [get_bd_intf_pins gen_xdma_descriptor_0/S_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_c2h_data_M_AXIS [get_bd_intf_pins axis_data_fifo_c2h_data/M_AXIS] [get_bd_intf_pins xdma_0/S_AXIS_C2H_0]
  connect_bd_intf_net -intf_net s_axis_h2c_cmd_1 [get_bd_intf_pins s_axis_h2c_cmd] [get_bd_intf_pins axis_data_fifo_h2c_cmd/S_AXIS]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins axi_firewall_0/S_AXI] [get_bd_intf_pins smartconnect_0/M00_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M01_AXI [get_bd_intf_pins axi_firewall_0/S_AXI_CTL] [get_bd_intf_pins smartconnect_0/M01_AXI]
  connect_bd_intf_net -intf_net xdma_0_M_AXIS_H2C_0 [get_bd_intf_pins axis_data_fifo_h2c_data/S_AXIS] [get_bd_intf_pins xdma_0/M_AXIS_H2C_0]
  connect_bd_intf_net -intf_net xdma_0_M_AXI_LITE [get_bd_intf_pins smartconnect_0/S00_AXI] [get_bd_intf_pins xdma_0/M_AXI_LITE]

  # Create port connections
  connect_bd_net -net axis_data_fifo_c2h_cmd_almost_empty [get_bd_pins host_mem_cmd_fifo_empty] [get_bd_pins axis_data_fifo_c2h_cmd/almost_empty]
  connect_bd_net -net axis_data_fifo_c2h_cmd_almost_full [get_bd_pins host_mem_cmd_fifo_full] [get_bd_pins axis_data_fifo_c2h_cmd/almost_full]
  connect_bd_net -net axis_data_fifo_c2h_data_almost_empty [get_bd_pins host_mem_data_fifo_empty] [get_bd_pins axis_data_fifo_c2h_data/almost_empty]
  connect_bd_net -net axis_data_fifo_c2h_data_almost_full [get_bd_pins host_mem_data_fifo_full] [get_bd_pins axis_data_fifo_c2h_data/almost_full]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_ctl [get_bd_pins gen_xdma_descriptor_0/dsc_ctl] [get_bd_pins xdma_0/c2h_dsc_byp_ctl_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_dst_addr [get_bd_pins gen_xdma_descriptor_0/dsc_dst_addr] [get_bd_pins xdma_0/c2h_dsc_byp_dst_addr_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_len [get_bd_pins gen_xdma_descriptor_0/dsc_len] [get_bd_pins xdma_0/c2h_dsc_byp_len_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_load [get_bd_pins gen_xdma_descriptor_0/dsc_load] [get_bd_pins xdma_0/c2h_dsc_byp_load_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_src_addr [get_bd_pins gen_xdma_descriptor_0/dsc_src_addr] [get_bd_pins xdma_0/c2h_dsc_byp_src_addr_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_ctl [get_bd_pins gen_xdma_descriptor_1/dsc_ctl] [get_bd_pins xdma_0/h2c_dsc_byp_ctl_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_dst_addr [get_bd_pins gen_xdma_descriptor_1/dsc_dst_addr] [get_bd_pins xdma_0/h2c_dsc_byp_dst_addr_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_len [get_bd_pins gen_xdma_descriptor_1/dsc_len] [get_bd_pins xdma_0/h2c_dsc_byp_len_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_load [get_bd_pins gen_xdma_descriptor_1/dsc_load] [get_bd_pins xdma_0/h2c_dsc_byp_load_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_src_addr [get_bd_pins gen_xdma_descriptor_1/dsc_src_addr] [get_bd_pins xdma_0/h2c_dsc_byp_src_addr_0]
  connect_bd_net -net pcie_clk_buf_inst_IBUF_DS_ODIV2 [get_bd_pins pcie_clk_buf_inst/IBUF_DS_ODIV2] [get_bd_pins xdma_0/sys_clk]
  connect_bd_net -net pcie_clk_buf_inst_IBUF_OUT [get_bd_pins pcie_clk_buf_inst/IBUF_OUT] [get_bd_pins xdma_0/sys_clk_gt]
  connect_bd_net -net pcie_perstn_1 [get_bd_pins pcie_perstn] [get_bd_pins xdma_0/sys_rst_n]
  connect_bd_net -net s_axis_aresetn_1 [get_bd_pins s_axis_aresetn] [get_bd_pins axi_firewall_0/aresetn] [get_bd_pins axis_data_fifo_c2h_cmd/s_axis_aresetn] [get_bd_pins axis_data_fifo_c2h_data/s_axis_aresetn] [get_bd_pins axis_data_fifo_h2c_cmd/s_axis_aresetn] [get_bd_pins axis_data_fifo_h2c_data/s_axis_aresetn] [get_bd_pins smartconnect_0/aresetn]
  connect_bd_net -net xdma_0_axi_aclk [get_bd_pins axi_aclk] [get_bd_pins axi_firewall_0/aclk] [get_bd_pins axis_data_fifo_c2h_cmd/s_axis_aclk] [get_bd_pins axis_data_fifo_c2h_data/s_axis_aclk] [get_bd_pins axis_data_fifo_h2c_cmd/s_axis_aclk] [get_bd_pins axis_data_fifo_h2c_data/s_axis_aclk] [get_bd_pins gen_xdma_descriptor_0/clk] [get_bd_pins gen_xdma_descriptor_1/clk] [get_bd_pins smartconnect_0/aclk] [get_bd_pins xdma_0/axi_aclk]
  connect_bd_net -net xdma_0_axi_aresetn [get_bd_pins axi_aresetn] [get_bd_pins xdma_0/axi_aresetn]
  connect_bd_net -net xdma_0_c2h_dsc_byp_ready_0 [get_bd_pins gen_xdma_descriptor_0/dsc_ready] [get_bd_pins xdma_0/c2h_dsc_byp_ready_0]
  connect_bd_net -net xdma_0_h2c_dsc_byp_ready_0 [get_bd_pins gen_xdma_descriptor_1/dsc_ready] [get_bd_pins xdma_0/h2c_dsc_byp_ready_0]
  connect_bd_net -net xdma_0_user_lnk_up [get_bd_pins user_lnk_up] [get_bd_pins xdma_0/user_lnk_up]

  # Restore current instance
  current_bd_instance $oldCurInst
}