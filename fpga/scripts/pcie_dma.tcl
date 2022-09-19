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

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi_dma_ctrl

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_c2h_data

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_c2h_datamover_cmd

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_h2c_datamover_cmd


  # Create pins
  create_bd_pin -dir O -type clk axi_aclk
  create_bd_pin -dir O -type rst axi_aresetn
  create_bd_pin -dir I -type rst pcie_perstn
  create_bd_pin -dir I -type rst s_axis_aresetn
  create_bd_pin -dir I usr_irq_req

  # Create instance: axi_firewall_0, and set properties
  set axi_firewall_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_firewall:1.2 axi_firewall_0 ]

  # Create instance: axi_gpio_0, and set properties
  set axi_gpio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0 ]
  set_property -dict [ list \
   CONFIG.C_ALL_INPUTS {1} \
   CONFIG.C_ALL_OUTPUTS {0} \
 ] $axi_gpio_0

  # Create instance: gen_xdma_descriptor_c2h, and set properties
  set block_name gen_xdma_descriptor_c2h
  set block_cell_name gen_xdma_descriptor_c2h
  if { [catch {set gen_xdma_descriptor_c2h [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $gen_xdma_descriptor_c2h eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: gen_xdma_descriptor_h2c, and set properties
  set block_name gen_xdma_descriptor_h2c
  set block_cell_name gen_xdma_descriptor_h2c
  if { [catch {set gen_xdma_descriptor_h2c [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $gen_xdma_descriptor_h2c eq "" } {
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

  # Create instance: smartconnect_1, and set properties
  set smartconnect_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_1 ]
  set_property -dict [ list \
   CONFIG.NUM_MI {2} \
   CONFIG.NUM_SI {1} \
 ] $smartconnect_1

  # Create instance: xdma_0, and set properties
  set xdma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xdma:4.1 xdma_0 ]
  set_property -dict [ list \
   CONFIG.INS_LOSS_NYQ {5} \
   CONFIG.PF0_DEVICE_ID_mqdma {903F} \
   CONFIG.PF0_SRIOV_VF_DEVICE_ID {A03F} \
   CONFIG.PF1_SRIOV_VF_DEVICE_ID {A13F} \
   CONFIG.PF2_DEVICE_ID_mqdma {923F} \
   CONFIG.PF2_SRIOV_VF_DEVICE_ID {A23F} \
   CONFIG.PF3_DEVICE_ID_mqdma {933F} \
   CONFIG.PF3_SRIOV_VF_DEVICE_ID {A33F} \
   CONFIG.axi_data_width {512_bit} \
   CONFIG.axi_id_width {2} \
   CONFIG.axil_master_64bit_en {false} \
   CONFIG.axilite_master_en {true} \
   CONFIG.axilite_master_size {4} \
   CONFIG.axisten_freq {250} \
   CONFIG.cfg_mgmt_if {false} \
   CONFIG.copy_pf0 {true} \
   CONFIG.coreclk_freq {500} \
   CONFIG.dsc_bypass_rd {0001} \
   CONFIG.dsc_bypass_wr {0001} \
   CONFIG.en_gt_selection {true} \
   CONFIG.ins_loss_profile {Chip-to-Chip} \
   CONFIG.mode_selection {Advanced} \
   CONFIG.pcie_blk_locn {PCIE4C_X1Y0} \
   CONFIG.pf0_base_class_menu {Processing_accelerators} \
   CONFIG.pf0_class_code {120000} \
   CONFIG.pf0_class_code_base {12} \
   CONFIG.pf0_class_code_interface {00} \
   CONFIG.pf0_class_code_sub {00} \
   CONFIG.pf0_device_id {903F} \
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
   CONFIG.pl_link_cap_max_link_speed {8.0_GT/s} \
   CONFIG.pl_link_cap_max_link_width {X16} \
   CONFIG.plltype {QPLL1} \
   CONFIG.runbit_fix {false} \
   CONFIG.select_quad {GTY_Quad_227} \
   CONFIG.vendor_id {10EE} \
   CONFIG.xdma_axi_intf_mm {AXI_Stream} \
   CONFIG.xdma_axilite_slave {true} \
   CONFIG.xdma_wnum_chnl {1} \
 ] $xdma_0

  # Create interface connections
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_pins m_axi_ctrl] [get_bd_intf_pins axi_firewall_0/M_AXI]
  connect_bd_intf_net -intf_net Conn3 [get_bd_intf_pins pcie_mgt] [get_bd_intf_pins xdma_0/pcie_mgt]
  connect_bd_intf_net -intf_net Conn5 [get_bd_intf_pins pcie_refclk] [get_bd_intf_pins pcie_clk_buf_inst/CLK_IN_D]
  connect_bd_intf_net -intf_net S_AXIS1_1 [get_bd_intf_pins s_axis_c2h_datamover_cmd] [get_bd_intf_pins gen_xdma_descriptor_c2h/S_AXIS]
  connect_bd_intf_net -intf_net S_AXIS_1 [get_bd_intf_pins s_axis_h2c_datamover_cmd] [get_bd_intf_pins gen_xdma_descriptor_h2c/S_AXIS]
  connect_bd_intf_net -intf_net S_AXIS_C2H_0_1 [get_bd_intf_pins s_axis_c2h_data] [get_bd_intf_pins xdma_0/S_AXIS_C2H_0]
  connect_bd_intf_net -intf_net s_axi_dma_ctrl_1 [get_bd_intf_pins s_axi_dma_ctrl] [get_bd_intf_pins smartconnect_1/S00_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins axi_firewall_0/S_AXI] [get_bd_intf_pins smartconnect_0/M00_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M01_AXI [get_bd_intf_pins axi_firewall_0/S_AXI_CTL] [get_bd_intf_pins smartconnect_0/M01_AXI]
  connect_bd_intf_net -intf_net smartconnect_1_M00_AXI [get_bd_intf_pins axi_gpio_0/S_AXI] [get_bd_intf_pins smartconnect_1/M00_AXI]
  connect_bd_intf_net -intf_net smartconnect_1_M01_AXI [get_bd_intf_pins smartconnect_1/M01_AXI] [get_bd_intf_pins xdma_0/S_AXI_LITE]
  connect_bd_intf_net -intf_net xdma_0_M_AXIS_H2C_0 [get_bd_intf_pins m_axis_h2c_data] [get_bd_intf_pins xdma_0/M_AXIS_H2C_0]
  connect_bd_intf_net -intf_net xdma_0_M_AXI_LITE [get_bd_intf_pins smartconnect_0/S00_AXI] [get_bd_intf_pins xdma_0/M_AXI_LITE]

  # Create port connections
  connect_bd_net -net gen_xdma_descriptor_0_dsc_ctl [get_bd_pins gen_xdma_descriptor_c2h/dsc_ctl] [get_bd_pins xdma_0/c2h_dsc_byp_ctl_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_dst_addr [get_bd_pins gen_xdma_descriptor_c2h/dsc_dst_addr] [get_bd_pins xdma_0/c2h_dsc_byp_dst_addr_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_len [get_bd_pins gen_xdma_descriptor_c2h/dsc_len] [get_bd_pins xdma_0/c2h_dsc_byp_len_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_load [get_bd_pins gen_xdma_descriptor_c2h/dsc_load] [get_bd_pins xdma_0/c2h_dsc_byp_load_0]
  connect_bd_net -net gen_xdma_descriptor_0_dsc_src_addr [get_bd_pins gen_xdma_descriptor_c2h/dsc_src_addr] [get_bd_pins xdma_0/c2h_dsc_byp_src_addr_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_ctl [get_bd_pins gen_xdma_descriptor_h2c/dsc_ctl] [get_bd_pins xdma_0/h2c_dsc_byp_ctl_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_dst_addr [get_bd_pins gen_xdma_descriptor_h2c/dsc_dst_addr] [get_bd_pins xdma_0/h2c_dsc_byp_dst_addr_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_len [get_bd_pins gen_xdma_descriptor_h2c/dsc_len] [get_bd_pins xdma_0/h2c_dsc_byp_len_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_load [get_bd_pins gen_xdma_descriptor_h2c/dsc_load] [get_bd_pins xdma_0/h2c_dsc_byp_load_0]
  connect_bd_net -net gen_xdma_descriptor_1_dsc_src_addr [get_bd_pins gen_xdma_descriptor_h2c/dsc_src_addr] [get_bd_pins xdma_0/h2c_dsc_byp_src_addr_0]
  connect_bd_net -net gen_xdma_descriptor_c2h_counter [get_bd_pins axi_gpio_0/gpio_io_i] [get_bd_pins gen_xdma_descriptor_c2h/counter]
  connect_bd_net -net pcie_clk_buf_inst_IBUF_DS_ODIV2 [get_bd_pins pcie_clk_buf_inst/IBUF_DS_ODIV2] [get_bd_pins xdma_0/sys_clk]
  connect_bd_net -net pcie_clk_buf_inst_IBUF_OUT [get_bd_pins pcie_clk_buf_inst/IBUF_OUT] [get_bd_pins xdma_0/sys_clk_gt]
  connect_bd_net -net pcie_perstn_1 [get_bd_pins pcie_perstn] [get_bd_pins xdma_0/sys_rst_n]
  connect_bd_net -net s_axis_aresetn_1 [get_bd_pins s_axis_aresetn] [get_bd_pins axi_firewall_0/aresetn] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins gen_xdma_descriptor_c2h/resetn] [get_bd_pins smartconnect_0/aresetn] [get_bd_pins smartconnect_1/aresetn]
  connect_bd_net -net usr_irq_req_1 [get_bd_pins usr_irq_req] [get_bd_pins xdma_0/usr_irq_req]
  connect_bd_net -net xdma_0_axi_aclk [get_bd_pins axi_aclk] [get_bd_pins axi_firewall_0/aclk] [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins gen_xdma_descriptor_c2h/clk] [get_bd_pins gen_xdma_descriptor_h2c/clk] [get_bd_pins smartconnect_0/aclk] [get_bd_pins smartconnect_1/aclk] [get_bd_pins xdma_0/axi_aclk]
  connect_bd_net -net xdma_0_axi_aresetn [get_bd_pins axi_aresetn] [get_bd_pins xdma_0/axi_aresetn]
  connect_bd_net -net xdma_0_c2h_dsc_byp_ready_0 [get_bd_pins gen_xdma_descriptor_c2h/dsc_ready] [get_bd_pins xdma_0/c2h_dsc_byp_ready_0]
  connect_bd_net -net xdma_0_h2c_dsc_byp_ready_0 [get_bd_pins gen_xdma_descriptor_h2c/dsc_ready] [get_bd_pins xdma_0/h2c_dsc_byp_ready_0]

  # Restore current instance
  current_bd_instance $oldCurInst
}