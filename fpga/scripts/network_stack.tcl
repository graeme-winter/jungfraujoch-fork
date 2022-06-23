## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

# Hierarchical cell: network_stack
proc create_hier_cell_network_stack { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_network_stack() - Empty argument(s)!"}
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
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 M00_AXIS

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 eth_in

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 udp_addr_out

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 udp_out


  # Create pins
  create_bd_pin -dir I -type rst ap_rst_n
  create_bd_pin -dir I -type clk axiclk
  create_bd_pin -dir I -from 0 -to 0 -type data data_collection_start
  create_bd_pin -dir I -from 31 -to 0 -type data fpga_ipv4_addr
  create_bd_pin -dir I -from 47 -to 0 -type data fpga_mac_addr
  create_bd_pin -dir O -from 63 -to 0 -type data packets_eth
  create_bd_pin -dir O packets_eth_ap_vld
  create_bd_pin -dir O -from 63 -to 0 -type data packets_icmp
  create_bd_pin -dir O packets_icmp_ap_vld
  create_bd_pin -dir O -from 63 -to 0 -type data packets_udp
  create_bd_pin -dir O packets_udp_ap_vld
  create_bd_pin -dir I -type rst resetn

  # Create instance: arp_0, and set properties
  set arp_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:arp:1.0 arp_0 ]

  # Create instance: axis_data_fifo_arp_0, and set properties
  set axis_data_fifo_arp_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_arp_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.IS_ACLK_ASYNC {0} \
 ] $axis_data_fifo_arp_0

  # Create instance: axis_data_fifo_arp_1, and set properties
  set axis_data_fifo_arp_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_arp_1 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.IS_ACLK_ASYNC {0} \
 ] $axis_data_fifo_arp_1

  # Create instance: axis_data_fifo_icmp_0, and set properties
  set axis_data_fifo_icmp_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_icmp_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.IS_ACLK_ASYNC {0} \
 ] $axis_data_fifo_icmp_0

  # Create instance: axis_data_fifo_icmp_1, and set properties
  set axis_data_fifo_icmp_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_icmp_1 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.IS_ACLK_ASYNC {0} \
 ] $axis_data_fifo_icmp_1

  # Create instance: axis_data_fifo_ipv4_0, and set properties
  set axis_data_fifo_ipv4_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_ipv4_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
 ] $axis_data_fifo_ipv4_0

  # Create instance: axis_data_fifo_udp_0, and set properties
  set axis_data_fifo_udp_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_udp_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
 ] $axis_data_fifo_udp_0

  # Create instance: axis_data_fifo_udp_1, and set properties
  set axis_data_fifo_udp_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_udp_1 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
 ] $axis_data_fifo_udp_1

  # Create instance: axis_data_fifo_udp_metadata_0, and set properties
  set axis_data_fifo_udp_metadata_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_udp_metadata_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
 ] $axis_data_fifo_udp_metadata_0

  # Create instance: axis_switch_0, and set properties
  set axis_switch_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_switch:1.1 axis_switch_0 ]
  set_property -dict [ list \
   CONFIG.ARB_ON_MAX_XFERS {0} \
   CONFIG.ARB_ON_TLAST {1} \
   CONFIG.HAS_TLAST {1} \
 ] $axis_switch_0

  # Create instance: ethernet_0, and set properties
  set ethernet_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:ethernet:1.0 ethernet_0 ]

  # Create instance: icmp_0, and set properties
  set icmp_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:icmp:1.0 icmp_0 ]

  # Create instance: ipv4_0, and set properties
  set ipv4_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:ipv4:1.0 ipv4_0 ]

  # Create instance: one, and set properties
  set one [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 one ]

  # Create instance: sls_detector_0, and set properties
  set sls_detector_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:sls_detector:1.0 sls_detector_0 ]

  # Create instance: udp_0, and set properties
  set udp_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:udp:1.0 udp_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins M00_AXIS] [get_bd_intf_pins axis_switch_0/M00_AXIS]
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_pins eth_in] [get_bd_intf_pins ethernet_0/eth_in]
  connect_bd_intf_net -intf_net arp_0_eth_out [get_bd_intf_pins arp_0/eth_out] [get_bd_intf_pins axis_data_fifo_arp_1/S_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_0_M_AXIS [get_bd_intf_pins axis_data_fifo_ipv4_0/M_AXIS] [get_bd_intf_pins ipv4_0/eth_in]
  connect_bd_intf_net -intf_net axis_data_fifo_arp_0_M_AXIS [get_bd_intf_pins arp_0/arp_in] [get_bd_intf_pins axis_data_fifo_arp_0/M_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_arp_1_M_AXIS [get_bd_intf_pins axis_data_fifo_arp_1/M_AXIS] [get_bd_intf_pins axis_switch_0/S00_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_icmp_0_M_AXIS [get_bd_intf_pins axis_data_fifo_icmp_0/M_AXIS] [get_bd_intf_pins icmp_0/s_axis_icmp]
  connect_bd_intf_net -intf_net axis_data_fifo_icmp_1_M_AXIS [get_bd_intf_pins axis_data_fifo_icmp_1/M_AXIS] [get_bd_intf_pins axis_switch_0/S01_AXIS]
  connect_bd_intf_net -intf_net axis_data_fifo_udp_0_M_AXIS [get_bd_intf_pins axis_data_fifo_udp_0/M_AXIS] [get_bd_intf_pins udp_0/eth_in]
  connect_bd_intf_net -intf_net axis_data_fifo_udp_1_M_AXIS [get_bd_intf_pins axis_data_fifo_udp_1/M_AXIS] [get_bd_intf_pins sls_detector_0/udp_payload_in]
  connect_bd_intf_net -intf_net axis_data_fifo_udp_metadata_0_M_AXIS [get_bd_intf_pins axis_data_fifo_udp_metadata_0/M_AXIS] [get_bd_intf_pins sls_detector_0/udp_metadata_in]
  connect_bd_intf_net -intf_net ethernet_0_arp_out [get_bd_intf_pins axis_data_fifo_arp_0/S_AXIS] [get_bd_intf_pins ethernet_0/arp_out]
  connect_bd_intf_net -intf_net ethernet_0_ip_out [get_bd_intf_pins axis_data_fifo_ipv4_0/S_AXIS] [get_bd_intf_pins ethernet_0/ip_out]
  connect_bd_intf_net -intf_net icmp_0_m_axis_icmp [get_bd_intf_pins axis_data_fifo_icmp_1/S_AXIS] [get_bd_intf_pins icmp_0/m_axis_icmp]
  connect_bd_intf_net -intf_net ipv4_0_icmp_out [get_bd_intf_pins axis_data_fifo_icmp_0/S_AXIS] [get_bd_intf_pins ipv4_0/icmp_out]
  connect_bd_intf_net -intf_net ipv4_0_udp_out [get_bd_intf_pins axis_data_fifo_udp_0/S_AXIS] [get_bd_intf_pins ipv4_0/udp_out]
  connect_bd_intf_net -intf_net sls_detector_0_addr_out [get_bd_intf_pins udp_addr_out] [get_bd_intf_pins sls_detector_0/addr_out]
  connect_bd_intf_net -intf_net sls_detector_0_data_out [get_bd_intf_pins udp_out] [get_bd_intf_pins sls_detector_0/data_out]
  connect_bd_intf_net -intf_net udp_0_udp_metadata_out [get_bd_intf_pins axis_data_fifo_udp_metadata_0/S_AXIS] [get_bd_intf_pins udp_0/udp_metadata_out]
  connect_bd_intf_net -intf_net udp_0_udp_payload_out [get_bd_intf_pins axis_data_fifo_udp_1/S_AXIS] [get_bd_intf_pins udp_0/udp_payload_out]

  # Create port connections
  connect_bd_net -net ap_rst_n_1 [get_bd_pins ap_rst_n] [get_bd_pins arp_0/ap_rst_n] [get_bd_pins ethernet_0/ap_rst_n] [get_bd_pins icmp_0/ap_rst_n] [get_bd_pins ipv4_0/ap_rst_n] [get_bd_pins sls_detector_0/ap_rst_n] [get_bd_pins udp_0/ap_rst_n]
  connect_bd_net -net axi_clk_net [get_bd_pins axiclk] [get_bd_pins arp_0/ap_clk] [get_bd_pins axis_data_fifo_arp_0/s_axis_aclk] [get_bd_pins axis_data_fifo_arp_1/s_axis_aclk] [get_bd_pins axis_data_fifo_icmp_0/s_axis_aclk] [get_bd_pins axis_data_fifo_icmp_1/s_axis_aclk] [get_bd_pins axis_data_fifo_ipv4_0/s_axis_aclk] [get_bd_pins axis_data_fifo_udp_0/s_axis_aclk] [get_bd_pins axis_data_fifo_udp_1/s_axis_aclk] [get_bd_pins axis_data_fifo_udp_metadata_0/s_axis_aclk] [get_bd_pins axis_switch_0/aclk] [get_bd_pins ethernet_0/ap_clk] [get_bd_pins icmp_0/ap_clk] [get_bd_pins ipv4_0/ap_clk] [get_bd_pins sls_detector_0/ap_clk] [get_bd_pins udp_0/ap_clk]
  connect_bd_net -net data_collection_start_V_1 [get_bd_pins data_collection_start] [get_bd_pins arp_0/arp_start]
  connect_bd_net -net enable_V_1 [get_bd_pins arp_0/enable] [get_bd_pins one/dout]
  connect_bd_net -net ethernet_0_counter [get_bd_pins packets_eth] [get_bd_pins ethernet_0/counter]
  connect_bd_net -net ethernet_0_counter_ap_vld [get_bd_pins packets_eth_ap_vld] [get_bd_pins ethernet_0/counter_ap_vld]
  connect_bd_net -net fpga_ipv4_addr_V_1 [get_bd_pins fpga_ipv4_addr] [get_bd_pins arp_0/fpga_ipv4_addr] [get_bd_pins ipv4_0/fpga_ipv4_addr]
  connect_bd_net -net fpga_mac_addr_V_1 [get_bd_pins fpga_mac_addr] [get_bd_pins arp_0/fpga_mac_addr] [get_bd_pins ethernet_0/fpga_mac_addr]
  connect_bd_net -net icmp_0_counter [get_bd_pins packets_icmp] [get_bd_pins icmp_0/counter]
  connect_bd_net -net icmp_0_counter_ap_vld [get_bd_pins packets_icmp_ap_vld] [get_bd_pins icmp_0/counter_ap_vld]
  connect_bd_net -net resetn_1 [get_bd_pins resetn] [get_bd_pins axis_data_fifo_arp_0/s_axis_aresetn] [get_bd_pins axis_data_fifo_arp_1/s_axis_aresetn] [get_bd_pins axis_data_fifo_icmp_0/s_axis_aresetn] [get_bd_pins axis_data_fifo_icmp_1/s_axis_aresetn] [get_bd_pins axis_data_fifo_ipv4_0/s_axis_aresetn] [get_bd_pins axis_data_fifo_udp_0/s_axis_aresetn] [get_bd_pins axis_data_fifo_udp_1/s_axis_aresetn] [get_bd_pins axis_data_fifo_udp_metadata_0/s_axis_aresetn] [get_bd_pins axis_switch_0/aresetn]
  connect_bd_net -net udp_0_counter [get_bd_pins packets_udp] [get_bd_pins udp_0/counter]
  connect_bd_net -net udp_0_counter_ap_vld [get_bd_pins packets_udp_ap_vld] [get_bd_pins udp_0/counter_ap_vld]

  # Restore current instance
  current_bd_instance $oldCurInst
}