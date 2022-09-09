## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

# Hierarchical cell: jungfraujoch
proc create_hier_cell_jungfraujoch { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_jungfraujoch() - Empty argument(s)!"}
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 eth_in

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 eth_out

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p0

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p1

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p10

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p11

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p2

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p3

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p4

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p5

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p6

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p7

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p8

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_d_hbm_p9

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 m_axis_c2h_data

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 m_axis_c2h_datamover_cmd

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 m_axis_h2c_datamover_cmd

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 s_axis_h2c_data


  # Create pins
  create_bd_pin -dir I -type rst ap_rst_n
  create_bd_pin -dir I apb_complete
  create_bd_pin -dir I -type clk axi_clk
  create_bd_pin -dir I -type rst axi_rst_n
  create_bd_pin -dir I eth_busy
  create_bd_pin -dir I eth_stat_rx_aligned
  create_bd_pin -dir I eth_stat_rx_packet_bad_fcs
  create_bd_pin -dir I eth_stat_rx_status
  create_bd_pin -dir I hbm_temp_trip
  create_bd_pin -dir I -from 6 -to 0 hbm_temperature
  create_bd_pin -dir I host_mem_cmd_fifo_empty
  create_bd_pin -dir I host_mem_cmd_fifo_full
  create_bd_pin -dir I host_mem_data_fifo_empty
  create_bd_pin -dir I host_mem_data_fifo_full
  create_bd_pin -dir I mm2s_error
  create_bd_pin -dir O qsfp_led_busy
  create_bd_pin -dir O qsfp_led_conn
  create_bd_pin -dir I qsfpdd_modprs
  create_bd_pin -dir I s2mm_error

  # Create instance: action_config_0, and set properties
  set block_name action_config
  set block_cell_name action_config_0
  if { [catch {set action_config_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $action_config_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }

  # Create instance: axi_bram_ctrl_0, and set properties
  set axi_bram_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_0 ]
  set_property -dict [ list \
   CONFIG.PROTOCOL {AXI4LITE} \
   CONFIG.READ_LATENCY {1} \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_0

  # Create instance: axis_addr_fifo_0, and set properties
  set axis_addr_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_addr_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {512} \
 ] $axis_addr_fifo_0

  # Create instance: axis_addr_fifo_1, and set properties
  set axis_addr_fifo_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_addr_fifo_1 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_addr_fifo_1

  # Create instance: axis_addr_fifo_2, and set properties
  set axis_addr_fifo_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_addr_fifo_2 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {512} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_addr_fifo_2

  # Create instance: axis_data_fifo_0, and set properties
  set axis_data_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {512} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
 ] $axis_data_fifo_0

  # Create instance: axis_data_fifo_1, and set properties
  set axis_data_fifo_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_1 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {512} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
 ] $axis_data_fifo_1

  # Create instance: axis_data_fifo_2, and set properties
  set axis_data_fifo_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_2 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {32768} \
   CONFIG.FIFO_MEMORY_TYPE {ultra} \
 ] $axis_data_fifo_2

  # Create instance: axis_data_fifo_3, and set properties
  set axis_data_fifo_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_3 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_3

  # Create instance: axis_data_fifo_4, and set properties
  set axis_data_fifo_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_4 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
 ] $axis_data_fifo_4

  # Create instance: axis_data_fifo_5, and set properties
  set axis_data_fifo_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_5 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {16} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
 ] $axis_data_fifo_5

  # Create instance: axis_data_fifo_6, and set properties
  set axis_data_fifo_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_6 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {32768} \
   CONFIG.FIFO_MEMORY_TYPE {ultra} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_data_fifo_6

  # Create instance: axis_register_slice_addr_0, and set properties
  set axis_register_slice_addr_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_addr_0 ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_addr_0

  # Create instance: axis_register_slice_addr_1, and set properties
  set axis_register_slice_addr_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_addr_1 ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_addr_1

  # Create instance: axis_register_slice_data_0, and set properties
  set axis_register_slice_data_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_data_0 ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_data_0

  # Create instance: axis_register_slice_data_1, and set properties
  set axis_register_slice_data_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_data_1 ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_data_1

  # Create instance: axis_register_slice_host_mem, and set properties
  set axis_register_slice_host_mem [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_host_mem ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_host_mem

  # Create instance: axis_register_slice_host_mem_cmd, and set properties
  set axis_register_slice_host_mem_cmd [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_host_mem_cmd ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_host_mem_cmd

  # Create instance: axis_register_slice_udp, and set properties
  set axis_register_slice_udp [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_udp ]
  set_property -dict [ list \
   CONFIG.REG_CONFIG {16} \
 ] $axis_register_slice_udp

  # Create instance: axis_udp_addr_fifo_0, and set properties
  set axis_udp_addr_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_udp_addr_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {64} \
 ] $axis_udp_addr_fifo_0

  # Create instance: axis_udp_fifo_0, and set properties
  set axis_udp_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_udp_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {256} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_udp_fifo_0

  # Create instance: axis_work_completion_fifo_0, and set properties
  set axis_work_completion_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_work_completion_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {1024} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
 ] $axis_work_completion_fifo_0

  # Create instance: axis_work_request_fifo_0, and set properties
  set axis_work_request_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_work_request_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {1024} \
   CONFIG.FIFO_MEMORY_TYPE {auto} \
   CONFIG.HAS_AEMPTY {1} \
   CONFIG.HAS_AFULL {1} \
   CONFIG.HAS_PROG_FULL {0} \
 ] $axis_work_request_fifo_0

  # Create instance: bitshuffle_0, and set properties
  set bitshuffle_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:bitshuffle:1.0 bitshuffle_0 ]

  # Create instance: calibration_addr_bram, and set properties
  set calibration_addr_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 calibration_addr_bram ]
  set_property -dict [ list \
   CONFIG.Assume_Synchronous_Clk {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_A_Write_Rate {50} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTB_Pin {true} \
 ] $calibration_addr_bram

  # Create instance: data_collection_fsm_0, and set properties
  set data_collection_fsm_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:data_collection_fsm:1.0 data_collection_fsm_0 ]

  # Create instance: jf_conversion_0, and set properties
  set jf_conversion_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:jf_conversion:1.0 jf_conversion_0 ]

  # Create instance: host_writer_0, and set properties
  set host_writer_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:host_writer:1.0 host_writer_0 ]

  # Create instance: internal_packet_generator_0, and set properties
  set internal_packet_generator_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:internal_packet_generator:1.0 internal_packet_generator_0 ]

  # Create instance: load_calibration_0, and set properties
  set load_calibration_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:load_calibration:1.0 load_calibration_0 ]

  # Create instance: mailbox_0, and set properties
  set mailbox_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:mailbox:2.1 mailbox_0 ]
  set_property -dict [ list \
   CONFIG.C_IMPL_STYLE {1} \
   CONFIG.C_INTERCONNECT_PORT_1 {4} \
   CONFIG.C_MAILBOX_DEPTH {256} \
 ] $mailbox_0

  # Create instance: network_stack
  create_hier_cell_network_stack $hier_obj network_stack

  # Create instance: smartconnect_0, and set properties
  set smartconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_0 ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {3} \
   CONFIG.NUM_SI {1} \
 ] $smartconnect_0

  # Create instance: timer_hbm_0, and set properties
  set timer_hbm_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:timer_hbm:1.0 timer_hbm_0 ]

  # Create instance: timer_host_0, and set properties
  set timer_host_0 [ create_bd_cell -type ip -vlnv psi.ch:hls:timer_host:1.0 timer_host_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_pins eth_out] [get_bd_intf_pins network_stack/M00_AXIS]
  connect_bd_intf_net -intf_net Conn3 [get_bd_intf_pins eth_in] [get_bd_intf_pins network_stack/eth_in]
  connect_bd_intf_net -intf_net S_AXIS_MM2S_CMD_1 [get_bd_intf_pins m_axis_h2c_datamover_cmd] [get_bd_intf_pins load_calibration_0/datamover_in_cmd]
  connect_bd_intf_net -intf_net axi_bram_ctrl_0_BRAM_PORTA [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA] [get_bd_intf_pins calibration_addr_bram/BRAM_PORTB]
  connect_bd_intf_net -intf_net aximm_host_infrastructure_M_AXIS_MM2S [get_bd_intf_pins s_axis_h2c_data] [get_bd_intf_pins load_calibration_0/host_memory_in]
  connect_bd_intf_net -intf_net axis_addr_fifo_0_M_AXIS [get_bd_intf_pins axis_addr_fifo_0/M_AXIS] [get_bd_intf_pins internal_packet_generator_0/addr_in]
  connect_bd_intf_net -intf_net axis_addr_fifo_2_M_AXIS [get_bd_intf_pins axis_addr_fifo_1/M_AXIS] [get_bd_intf_pins jf_conversion_0/addr_in]
  connect_bd_intf_net -intf_net axis_addr_fifo_3_M_AXIS [get_bd_intf_pins axis_addr_fifo_2/M_AXIS] [get_bd_intf_pins host_writer_0/addr_in]
  connect_bd_intf_net -intf_net axis_data_fifo_0_M_AXIS [get_bd_intf_pins axis_data_fifo_0/M_AXIS] [get_bd_intf_pins load_calibration_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_1_M_AXIS [get_bd_intf_pins axis_data_fifo_1/M_AXIS] [get_bd_intf_pins internal_packet_generator_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_2_M_AXIS [get_bd_intf_pins axis_data_fifo_2/M_AXIS] [get_bd_intf_pins timer_hbm_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_4_M_AXIS [get_bd_intf_pins axis_data_fifo_3/M_AXIS] [get_bd_intf_pins jf_conversion_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_6_M_AXIS [get_bd_intf_pins axis_data_fifo_5/M_AXIS] [get_bd_intf_pins timer_host_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_7_M_AXIS [get_bd_intf_pins axis_data_fifo_4/M_AXIS] [get_bd_intf_pins bitshuffle_0/data_in]
  connect_bd_intf_net -intf_net axis_data_fifo_8_M_AXIS [get_bd_intf_pins axis_data_fifo_6/M_AXIS] [get_bd_intf_pins host_writer_0/data_in]
  connect_bd_intf_net -intf_net axis_register_slice_0_M_AXIS [get_bd_intf_pins axis_addr_fifo_2/S_AXIS] [get_bd_intf_pins axis_register_slice_addr_1/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_1_M_AXIS [get_bd_intf_pins axis_data_fifo_6/S_AXIS] [get_bd_intf_pins axis_register_slice_data_1/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_2_M_AXIS [get_bd_intf_pins axis_addr_fifo_1/S_AXIS] [get_bd_intf_pins axis_register_slice_addr_0/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_4_M_AXIS [get_bd_intf_pins axis_data_fifo_3/S_AXIS] [get_bd_intf_pins axis_register_slice_data_0/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_host_mem_M_AXIS [get_bd_intf_pins m_axis_c2h_data] [get_bd_intf_pins axis_register_slice_host_mem/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_host_mem_cmd_M_AXIS [get_bd_intf_pins m_axis_c2h_datamover_cmd] [get_bd_intf_pins axis_register_slice_host_mem_cmd/M_AXIS]
  connect_bd_intf_net -intf_net axis_register_slice_udp_M_AXIS [get_bd_intf_pins axis_register_slice_udp/M_AXIS] [get_bd_intf_pins data_collection_fsm_0/eth_in]
  connect_bd_intf_net -intf_net axis_udp_addr_fifo_0_M_AXIS [get_bd_intf_pins axis_udp_addr_fifo_0/M_AXIS] [get_bd_intf_pins data_collection_fsm_0/addr_in]
  connect_bd_intf_net -intf_net axis_udp_fifo_0_M_AXIS [get_bd_intf_pins axis_register_slice_udp/S_AXIS] [get_bd_intf_pins axis_udp_fifo_0/M_AXIS]
  connect_bd_intf_net -intf_net axis_work_completion_fifo_0_M_AXIS [get_bd_intf_pins axis_work_completion_fifo_0/M_AXIS] [get_bd_intf_pins mailbox_0/S1_AXIS]
  connect_bd_intf_net -intf_net axis_work_request_fifo_0_M_AXIS [get_bd_intf_pins axis_work_request_fifo_0/M_AXIS] [get_bd_intf_pins host_writer_0/s_axis_work_request]
  connect_bd_intf_net -intf_net bitshuffle_0_data_out [get_bd_intf_pins axis_data_fifo_5/S_AXIS] [get_bd_intf_pins bitshuffle_0/data_out]
  connect_bd_intf_net -intf_net data_collection_fsm_0_addr_out [get_bd_intf_pins axis_addr_fifo_0/S_AXIS] [get_bd_intf_pins data_collection_fsm_0/addr_out]
  connect_bd_intf_net -intf_net data_collection_fsm_0_data_out [get_bd_intf_pins axis_data_fifo_0/S_AXIS] [get_bd_intf_pins data_collection_fsm_0/data_out]
  connect_bd_intf_net -intf_net jf_conversion_0_addr_out [get_bd_intf_pins axis_register_slice_addr_1/S_AXIS] [get_bd_intf_pins jf_conversion_0/addr_out]
  connect_bd_intf_net -intf_net jf_conversion_0_data_out [get_bd_intf_pins axis_data_fifo_4/S_AXIS] [get_bd_intf_pins jf_conversion_0/data_out]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p0 [get_bd_intf_pins m_axi_d_hbm_p0] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p0]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p1 [get_bd_intf_pins m_axi_d_hbm_p1] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p1]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p10 [get_bd_intf_pins m_axi_d_hbm_p10] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p10]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p11 [get_bd_intf_pins m_axi_d_hbm_p11] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p11]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p2 [get_bd_intf_pins m_axi_d_hbm_p2] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p2]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p3 [get_bd_intf_pins m_axi_d_hbm_p3] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p3]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p4 [get_bd_intf_pins m_axi_d_hbm_p4] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p4]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p5 [get_bd_intf_pins m_axi_d_hbm_p5] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p5]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p6 [get_bd_intf_pins m_axi_d_hbm_p6] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p6]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p7 [get_bd_intf_pins m_axi_d_hbm_p7] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p7]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p8 [get_bd_intf_pins m_axi_d_hbm_p8] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p8]
  connect_bd_intf_net -intf_net gain_m_axi_d_hbm_p9 [get_bd_intf_pins m_axi_d_hbm_p9] [get_bd_intf_pins jf_conversion_0/m_axi_d_hbm_p9]
  connect_bd_intf_net -intf_net host_writer_0_datamover_out_cmd [get_bd_intf_pins axis_register_slice_host_mem_cmd/S_AXIS] [get_bd_intf_pins host_writer_0/datamover_out_cmd]
  connect_bd_intf_net -intf_net host_writer_0_host_memory_out [get_bd_intf_pins axis_register_slice_host_mem/S_AXIS] [get_bd_intf_pins host_writer_0/host_memory_out]
  connect_bd_intf_net -intf_net host_writer_0_m_axis_completion [get_bd_intf_pins axis_work_completion_fifo_0/S_AXIS] [get_bd_intf_pins host_writer_0/m_axis_completion]
  connect_bd_intf_net -intf_net internal_packet_generator_0_addr_out [get_bd_intf_pins axis_register_slice_addr_0/S_AXIS] [get_bd_intf_pins internal_packet_generator_0/addr_out]
  connect_bd_intf_net -intf_net internal_packet_generator_0_data_out [get_bd_intf_pins axis_data_fifo_2/S_AXIS] [get_bd_intf_pins internal_packet_generator_0/data_out]
  connect_bd_intf_net -intf_net load_calibration_0_data_out [get_bd_intf_pins axis_data_fifo_1/S_AXIS] [get_bd_intf_pins load_calibration_0/data_out]
  connect_bd_intf_net -intf_net load_calibration_0_in_mem_location_PORTA [get_bd_intf_pins calibration_addr_bram/BRAM_PORTA] [get_bd_intf_pins load_calibration_0/in_mem_location_PORTA]
  connect_bd_intf_net -intf_net mailbox_0_M1_AXIS [get_bd_intf_pins axis_work_request_fifo_0/S_AXIS] [get_bd_intf_pins mailbox_0/M1_AXIS]
  connect_bd_intf_net -intf_net network_stack_udp_addr_out [get_bd_intf_pins axis_udp_addr_fifo_0/S_AXIS] [get_bd_intf_pins network_stack/udp_addr_out]
  connect_bd_intf_net -intf_net network_stack_udp_out [get_bd_intf_pins axis_udp_fifo_0/S_AXIS] [get_bd_intf_pins network_stack/udp_out]
  connect_bd_intf_net -intf_net s_axi_1 [get_bd_intf_pins s_axi] [get_bd_intf_pins smartconnect_0/S00_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins action_config_0/s_axi] [get_bd_intf_pins smartconnect_0/M00_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M01_AXI [get_bd_intf_pins mailbox_0/S0_AXI] [get_bd_intf_pins smartconnect_0/M01_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M02_AXI [get_bd_intf_pins axi_bram_ctrl_0/S_AXI] [get_bd_intf_pins smartconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net timer_hbm_0_data_out [get_bd_intf_pins axis_register_slice_data_0/S_AXIS] [get_bd_intf_pins timer_hbm_0/data_out]
  connect_bd_intf_net -intf_net timer_host_0_data_out [get_bd_intf_pins axis_register_slice_data_1/S_AXIS] [get_bd_intf_pins timer_host_0/data_out]

  # Create port connections
  connect_bd_net -net action_config_0_data_collection_cancel [get_bd_pins action_config_0/data_collection_cancel] [get_bd_pins data_collection_fsm_0/in_cancel] [get_bd_pins internal_packet_generator_0/in_cancel]
  connect_bd_net -net action_config_0_data_collection_fsm_start [get_bd_pins action_config_0/data_collection_start] [get_bd_pins data_collection_fsm_0/in_run] [get_bd_pins network_stack/data_collection_start]
  connect_bd_net -net action_config_0_data_collection_mode [get_bd_pins action_config_0/data_collection_mode] [get_bd_pins data_collection_fsm_0/mode]
  connect_bd_net -net action_config_0_fpga_ipv4_addr [get_bd_pins action_config_0/fpga_ipv4_addr] [get_bd_pins network_stack/fpga_ipv4_addr]
  connect_bd_net -net action_config_0_fpga_mac_addr [get_bd_pins action_config_0/fpga_mac_addr] [get_bd_pins network_stack/fpga_mac_addr]
  connect_bd_net -net action_config_0_frames_per_trigger [get_bd_pins action_config_0/frames_per_trigger] [get_bd_pins data_collection_fsm_0/frames_per_trigger]
  connect_bd_net -net action_config_0_nmodules [get_bd_pins action_config_0/nmodules] [get_bd_pins data_collection_fsm_0/nmodules]
  connect_bd_net -net action_config_0_nstorage_cells [get_bd_pins action_config_0/nstorage_cells] [get_bd_pins data_collection_fsm_0/nstorage_cells]
  connect_bd_net -net action_config_0_one_over_energy [get_bd_pins action_config_0/one_over_energy] [get_bd_pins data_collection_fsm_0/one_over_energy]
  connect_bd_net -net ap_clk_1 [get_bd_pins axi_clk] [get_bd_pins action_config_0/clk] [get_bd_pins axi_bram_ctrl_0/s_axi_aclk] [get_bd_pins axis_addr_fifo_0/s_axis_aclk] [get_bd_pins axis_addr_fifo_1/s_axis_aclk] [get_bd_pins axis_addr_fifo_2/s_axis_aclk] [get_bd_pins axis_data_fifo_0/s_axis_aclk] [get_bd_pins axis_data_fifo_1/s_axis_aclk] [get_bd_pins axis_data_fifo_2/s_axis_aclk] [get_bd_pins axis_data_fifo_3/s_axis_aclk] [get_bd_pins axis_data_fifo_4/s_axis_aclk] [get_bd_pins axis_data_fifo_5/s_axis_aclk] [get_bd_pins axis_data_fifo_6/s_axis_aclk] [get_bd_pins axis_register_slice_addr_0/aclk] [get_bd_pins axis_register_slice_addr_1/aclk] [get_bd_pins axis_register_slice_data_0/aclk] [get_bd_pins axis_register_slice_data_1/aclk] [get_bd_pins axis_register_slice_host_mem/aclk] [get_bd_pins axis_register_slice_host_mem_cmd/aclk] [get_bd_pins axis_register_slice_udp/aclk] [get_bd_pins axis_udp_addr_fifo_0/s_axis_aclk] [get_bd_pins axis_udp_fifo_0/s_axis_aclk] [get_bd_pins axis_work_completion_fifo_0/s_axis_aclk] [get_bd_pins axis_work_request_fifo_0/s_axis_aclk] [get_bd_pins bitshuffle_0/ap_clk] [get_bd_pins data_collection_fsm_0/ap_clk] [get_bd_pins jf_conversion_0/ap_clk] [get_bd_pins host_writer_0/ap_clk] [get_bd_pins internal_packet_generator_0/ap_clk] [get_bd_pins load_calibration_0/ap_clk] [get_bd_pins mailbox_0/M1_AXIS_ACLK] [get_bd_pins mailbox_0/S0_AXI_ACLK] [get_bd_pins mailbox_0/S1_AXIS_ACLK] [get_bd_pins network_stack/axiclk] [get_bd_pins smartconnect_0/aclk] [get_bd_pins timer_hbm_0/ap_clk] [get_bd_pins timer_host_0/ap_clk]
  connect_bd_net -net apb_complete_1 [get_bd_pins apb_complete] [get_bd_pins action_config_0/apb_complete_0]
  connect_bd_net -net axilite_ctrl_infrastructure_qsfp_led_busy [get_bd_pins qsfp_led_busy] [get_bd_pins action_config_0/qsfp_led_busy]
  connect_bd_net -net axilite_ctrl_infrastructure_qsfp_led_conn [get_bd_pins qsfp_led_conn] [get_bd_pins action_config_0/qsfp_led_conn]
  connect_bd_net -net aximm_host_infrastructure_err_encountered [get_bd_pins mm2s_error] [get_bd_pins action_config_0/mm2s_error]
  connect_bd_net -net aximm_host_infrastructure_err_encountered1 [get_bd_pins s2mm_error] [get_bd_pins action_config_0/s2mm_error]
  connect_bd_net -net axis_addr_fifo_2_almost_empty [get_bd_pins action_config_0/pede2gain_addr_fifo_empty] [get_bd_pins axis_addr_fifo_1/almost_empty]
  connect_bd_net -net axis_addr_fifo_2_almost_full [get_bd_pins action_config_0/pede2gain_addr_fifo_full] [get_bd_pins axis_addr_fifo_1/almost_full]
  connect_bd_net -net axis_addr_fifo_3_almost_empty [get_bd_pins action_config_0/last_addr_fifo_empty] [get_bd_pins axis_addr_fifo_2/almost_empty]
  connect_bd_net -net axis_addr_fifo_3_almost_full [get_bd_pins action_config_0/last_addr_fifo_full] [get_bd_pins axis_addr_fifo_2/almost_full]
  connect_bd_net -net axis_data_fifo_4_almost_empty [get_bd_pins action_config_0/pede2gain_data_fifo_empty] [get_bd_pins axis_data_fifo_3/almost_empty]
  connect_bd_net -net axis_data_fifo_4_almost_full [get_bd_pins action_config_0/pede2gain_data_fifo_full] [get_bd_pins axis_data_fifo_3/almost_full]
  connect_bd_net -net axis_data_fifo_8_almost_empty [get_bd_pins action_config_0/last_data_fifo_empty] [get_bd_pins axis_data_fifo_6/almost_empty]
  connect_bd_net -net axis_data_fifo_8_almost_full [get_bd_pins action_config_0/last_data_fifo_full] [get_bd_pins axis_data_fifo_6/almost_full]
  connect_bd_net -net axis_udp_fifo_0_almost_empty [get_bd_pins action_config_0/udp_fifo_empty] [get_bd_pins axis_udp_fifo_0/almost_empty]
  connect_bd_net -net axis_udp_fifo_0_almost_full [get_bd_pins action_config_0/udp_fifo_full] [get_bd_pins axis_udp_fifo_0/almost_full]
  connect_bd_net -net axis_work_completion_fifo_0_almost_empty [get_bd_pins action_config_0/work_compl_fifo_empty] [get_bd_pins axis_work_completion_fifo_0/almost_empty]
  connect_bd_net -net axis_work_completion_fifo_0_almost_full [get_bd_pins action_config_0/work_compl_fifo_full] [get_bd_pins axis_work_completion_fifo_0/almost_full]
  connect_bd_net -net axis_work_request_fifo_0_almost_empty [get_bd_pins action_config_0/work_req_fifo_empty] [get_bd_pins axis_work_request_fifo_0/almost_empty]
  connect_bd_net -net axis_work_request_fifo_0_almost_full [get_bd_pins action_config_0/work_req_fifo_full] [get_bd_pins axis_work_request_fifo_0/almost_full]
  connect_bd_net -net data_collection_fsm_0_out_idle_V [get_bd_pins action_config_0/data_collection_idle] [get_bd_pins data_collection_fsm_0/out_idle]
  connect_bd_net -net eth_busy_1 [get_bd_pins eth_busy] [get_bd_pins action_config_0/eth_busy]
  connect_bd_net -net eth_stat_rx_aligned_1 [get_bd_pins eth_stat_rx_aligned] [get_bd_pins action_config_0/eth_stat_rx_aligned]
  connect_bd_net -net eth_stat_rx_packet_bad_fcs_1 [get_bd_pins eth_stat_rx_packet_bad_fcs] [get_bd_pins action_config_0/eth_stat_rx_packet_bad_fcs]
  connect_bd_net -net eth_stat_rx_status_1 [get_bd_pins eth_stat_rx_status] [get_bd_pins action_config_0/eth_stat_rx_status]
  connect_bd_net -net hbm_temp_trip_1 [get_bd_pins hbm_temp_trip] [get_bd_pins action_config_0/hbm_temp_trip]
  connect_bd_net -net hbm_temperature_1 [get_bd_pins hbm_temperature] [get_bd_pins action_config_0/hbm_temperature]
  connect_bd_net -net host_mem_cmd_fifo_empty_1 [get_bd_pins host_mem_cmd_fifo_empty] [get_bd_pins action_config_0/host_mem_cmd_fifo_empty]
  connect_bd_net -net host_mem_cmd_fifo_full_1 [get_bd_pins host_mem_cmd_fifo_full] [get_bd_pins action_config_0/host_mem_cmd_fifo_full]
  connect_bd_net -net host_mem_data_fifo_empty_1 [get_bd_pins host_mem_data_fifo_empty] [get_bd_pins action_config_0/host_mem_data_fifo_empty]
  connect_bd_net -net host_mem_data_fifo_full_1 [get_bd_pins host_mem_data_fifo_full] [get_bd_pins action_config_0/host_mem_data_fifo_full]
  connect_bd_net -net host_writer_0_err_reg_V [get_bd_pins action_config_0/host_writer_err] [get_bd_pins host_writer_0/err_reg]
  connect_bd_net -net host_writer_0_err_reg_V_ap_vld [get_bd_pins action_config_0/host_writer_err_valid] [get_bd_pins host_writer_0/err_reg_ap_vld]
  connect_bd_net -net host_writer_0_packets_processed [get_bd_pins action_config_0/packets_processed] [get_bd_pins host_writer_0/packets_processed]
  connect_bd_net -net host_writer_0_packets_processed_ap_vld [get_bd_pins action_config_0/packets_processed_valid] [get_bd_pins host_writer_0/packets_processed_ap_vld]
  connect_bd_net -net mailbox_0_Interrupt_0 [get_bd_pins action_config_0/mailbox_interrupt_0] [get_bd_pins mailbox_0/Interrupt_0]
  connect_bd_net -net mailbox_0_Interrupt_1 [get_bd_pins action_config_0/mailbox_interrupt_1] [get_bd_pins mailbox_0/Interrupt_1]
  connect_bd_net -net network_stack_counter [get_bd_pins action_config_0/packets_eth] [get_bd_pins network_stack/packets_eth]
  connect_bd_net -net network_stack_counter_ap_vld [get_bd_pins action_config_0/packets_eth_valid] [get_bd_pins network_stack/packets_eth_ap_vld]
  connect_bd_net -net network_stack_packets_icmp [get_bd_pins action_config_0/packets_icmp] [get_bd_pins network_stack/packets_icmp]
  connect_bd_net -net network_stack_packets_icmp_ap_vld [get_bd_pins action_config_0/packets_icmp_valid] [get_bd_pins network_stack/packets_icmp_ap_vld]
  connect_bd_net -net network_stack_packets_udp [get_bd_pins action_config_0/packets_udp] [get_bd_pins network_stack/packets_udp]
  connect_bd_net -net network_stack_packets_udp_ap_vld [get_bd_pins action_config_0/packets_udp_valid] [get_bd_pins network_stack/packets_udp_ap_vld]
  connect_bd_net -net qsfpdd_modprs_1 [get_bd_pins qsfpdd_modprs] [get_bd_pins action_config_0/qsfpdd_modprs]
  connect_bd_net -net reset_axi [get_bd_pins axi_rst_n] [get_bd_pins action_config_0/resetn] [get_bd_pins axis_addr_fifo_0/s_axis_aresetn] [get_bd_pins axis_addr_fifo_1/s_axis_aresetn] [get_bd_pins axis_addr_fifo_2/s_axis_aresetn] [get_bd_pins axis_data_fifo_0/s_axis_aresetn] [get_bd_pins axis_data_fifo_1/s_axis_aresetn] [get_bd_pins axis_data_fifo_2/s_axis_aresetn] [get_bd_pins axis_data_fifo_3/s_axis_aresetn] [get_bd_pins axis_data_fifo_4/s_axis_aresetn] [get_bd_pins axis_data_fifo_5/s_axis_aresetn] [get_bd_pins axis_data_fifo_6/s_axis_aresetn] [get_bd_pins axis_register_slice_addr_0/aresetn] [get_bd_pins axis_register_slice_addr_1/aresetn] [get_bd_pins axis_register_slice_data_0/aresetn] [get_bd_pins axis_register_slice_data_1/aresetn] [get_bd_pins axis_register_slice_host_mem/aresetn] [get_bd_pins axis_register_slice_host_mem_cmd/aresetn] [get_bd_pins axis_register_slice_udp/aresetn] [get_bd_pins axis_udp_addr_fifo_0/s_axis_aresetn] [get_bd_pins axis_udp_fifo_0/s_axis_aresetn] [get_bd_pins axis_work_completion_fifo_0/s_axis_aresetn] [get_bd_pins axis_work_request_fifo_0/s_axis_aresetn] [get_bd_pins network_stack/resetn] [get_bd_pins smartconnect_0/aresetn]
  connect_bd_net -net reset_hls [get_bd_pins ap_rst_n] [get_bd_pins axi_bram_ctrl_0/s_axi_aresetn] [get_bd_pins bitshuffle_0/ap_rst_n] [get_bd_pins data_collection_fsm_0/ap_rst_n] [get_bd_pins jf_conversion_0/ap_rst_n] [get_bd_pins host_writer_0/ap_rst_n] [get_bd_pins internal_packet_generator_0/ap_rst_n] [get_bd_pins load_calibration_0/ap_rst_n] [get_bd_pins mailbox_0/S0_AXI_ARESETN] [get_bd_pins network_stack/ap_rst_n] [get_bd_pins timer_hbm_0/ap_rst_n] [get_bd_pins timer_host_0/ap_rst_n]
  connect_bd_net -net timer_hbm_0_counter [get_bd_pins action_config_0/stalls_hbm] [get_bd_pins timer_hbm_0/counter]
  connect_bd_net -net timer_hbm_0_counter_ap_vld [get_bd_pins action_config_0/stalls_hbm_valid] [get_bd_pins timer_hbm_0/counter_ap_vld]
  connect_bd_net -net timer_host_0_counter [get_bd_pins action_config_0/stalls_host] [get_bd_pins timer_host_0/counter]
  connect_bd_net -net timer_host_0_counter_ap_vld [get_bd_pins action_config_0/stalls_host_valid] [get_bd_pins timer_host_0/counter_ap_vld]

  # Restore current instance
  current_bd_instance $oldCurInst
}