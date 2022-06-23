## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

################################################################
# This is a generated script based on design: jfjoch_pcie
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2022.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source jfjoch_pcie_script.tcl


# The design that will be created by this Tcl script contains the following 
# module references:
# gen_xdma_descriptor_c2h, action_config, check_eth_busy, resetn_sync

# Please add the sources of those modules before sourcing this Tcl script.

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvu35p-fsvh2104-2-e
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name jfjoch_pcie

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES:
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\
alpha-data.com:user:bci_admpcie9h3:2.0\
xilinx.com:ip:xlconstant:1.1\
xilinx.com:ip:proc_sys_reset:5.0\
xilinx.com:ip:util_ds_buf:2.2\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:axi_protocol_converter:2.1\
xilinx.com:ip:axi_register_slice:2.1\
xilinx.com:ip:hbm:1.0\
xilinx.com:ip:axi_bram_ctrl:4.1\
xilinx.com:ip:axis_data_fifo:2.0\
xilinx.com:ip:axis_register_slice:1.1\
psi.ch:hls:bitshuffle:1.0\
xilinx.com:ip:blk_mem_gen:8.4\
psi.ch:hls:data_collection_fsm:1.0\
psi.ch:hls:jf_conversion:1.0\
psi.ch:hls:host_writer:1.0\
psi.ch:hls:internal_packet_generator:1.0\
psi.ch:hls:load_calibration:1.0\
xilinx.com:ip:mailbox:2.1\
psi.ch:hls:timer_hbm:1.0\
psi.ch:hls:timer_host:1.0\
xilinx.com:ip:cmac_usplus:3.1\
xilinx.com:ip:util_vector_logic:2.0\
xilinx.com:ip:axi_firewall:1.2\
xilinx.com:ip:xdma:4.1\
psi.ch:hls:arp:1.0\
xilinx.com:ip:axis_switch:1.1\
psi.ch:hls:ethernet:1.0\
psi.ch:hls:icmp:1.0\
psi.ch:hls:ipv4:1.0\
psi.ch:hls:sls_detector:1.0\
psi.ch:hls:udp:1.0\
"

   set list_ips_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

##################################################################
# CHECK Modules
##################################################################
set bCheckModules 1
if { $bCheckModules == 1 } {
   set list_check_mods "\
gen_xdma_descriptor_c2h\
action_config\
check_eth_busy\
refclk300to100 \
resetn_sync\
"

   set list_mods_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following modules exist in the project's sources: $list_check_mods ."

   foreach mod_vlnv $list_check_mods {
      if { [can_resolve_reference $mod_vlnv] == 0 } {
         lappend list_mods_missing $mod_vlnv
      }
   }

   if { $list_mods_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following module(s) are not found in the project: $list_mods_missing" }
      common::send_msg_id "BD_TCL-008" "INFO" "Please add source files for the missing module(s) above."
      set bCheckIPsPassed 0
   }
}

if { $bCheckIPsPassed != 1 } {
  common::send_msg_id "BD_TCL-1003" "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################

# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
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


  # Create interface ports
  set hbm_ref [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 hbm_ref ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {300000000} \
   ] $hbm_ref

  set pcie0_mgt [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:pcie_7x_mgt_rtl:1.0 pcie0_mgt ]

  set pcie0_ref [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 pcie0_ref ]

  set qsfp0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 qsfp0 ]

  set qsfp0_ref [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 qsfp0_ref ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {161132812} \
   ] $qsfp0_ref

  set qspi [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:spi_rtl:1.0 qspi ]

  set uc [ create_bd_intf_port -mode Slave -vlnv alpha-data.com:user:avr_rtl:1.0 uc ]


  # Create ports
  set pcie_perstn [ create_bd_port -dir I -type rst pcie_perstn ]
  set qsfp0_led_busy [ create_bd_port -dir O qsfp0_led_busy ]
  set qsfp0_led_conn [ create_bd_port -dir O qsfp0_led_conn ]
  set qsfpdd_modprs [ create_bd_port -dir I qsfpdd_modprs ]

  # Create instance: axi_quad_spi_0, and set properties
  set axi_quad_spi_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_quad_spi:3.2 axi_quad_spi_0 ]
  set_property -dict [ list \
   CONFIG.C_DUAL_QUAD_MODE {1} \
   CONFIG.C_NUM_SS_BITS {2} \
   CONFIG.C_SCK_RATIO {2} \
   CONFIG.C_SPI_MEMORY {2} \
   CONFIG.C_SPI_MODE {2} \
   CONFIG.C_USE_STARTUP {1} \
   CONFIG.C_USE_STARTUP_INT {1} \
 ] $axi_quad_spi_0

  # Create instance: bci_admpcie9h3_0, and set properties
  set bci_admpcie9h3_0 [ create_bd_cell -type ip -vlnv alpha-data.com:user:bci_admpcie9h3:2.0 bci_admpcie9h3_0 ]

  # Create instance: hbm_infrastructure
  create_hier_cell_hbm_infrastructure [current_bd_instance .] hbm_infrastructure LEFT

  # Create instance: jungfraujoch
  create_hier_cell_jungfraujoch [current_bd_instance .] jungfraujoch

  # Create instance: mac_100g
  create_hier_cell_mac_100g [current_bd_instance .] mac_100g

  # Create instance: one, and set properties
  set one [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 one ]

  # Create instance: pcie_dma
  create_hier_cell_pcie_dma [current_bd_instance .] pcie_dma

  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]
  set_property -dict [ list \
   CONFIG.C_EXT_RST_WIDTH {1} \
 ] $proc_sys_reset_0

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

  # Create instance: refclk_ibufds_inst, and set properties
  set refclk_ibufds_inst [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 refclk_ibufds_inst ]
  set_property -dict [ list \
   CONFIG.C_BUF_TYPE {IBUFDS} \
 ] $refclk_ibufds_inst

  # Create instance: smartconnect_1, and set properties
  set smartconnect_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_1 ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {2} \
   CONFIG.NUM_MI {4} \
   CONFIG.NUM_SI {1} \
 ] $smartconnect_1

  # Create instance: zero, and set properties
  set zero [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 zero ]
  set_property -dict [ list \
   CONFIG.CONST_VAL {0} \
 ] $zero

  # Create interface connections
  connect_bd_intf_net -intf_net S_AXIS_100G_1 [get_bd_intf_pins jungfraujoch/eth_out] [get_bd_intf_pins mac_100g/s_axis_eth_out]
  connect_bd_intf_net -intf_net axi_quad_spi_0_SPI_1 [get_bd_intf_ports qspi] [get_bd_intf_pins axi_quad_spi_0/SPI_1]
  connect_bd_intf_net -intf_net hbm_ref_1 [get_bd_intf_ports hbm_ref] [get_bd_intf_pins refclk_ibufds_inst/CLK_IN_D]
  connect_bd_intf_net -intf_net jungfraujoch_m_axis_c2h_data [get_bd_intf_pins jungfraujoch/m_axis_c2h_data] [get_bd_intf_pins pcie_dma/s_axis_c2h_data]
  connect_bd_intf_net -intf_net jungfraujoch_m_axis_c2h_datamover_cmd [get_bd_intf_pins jungfraujoch/m_axis_c2h_datamover_cmd] [get_bd_intf_pins pcie_dma/s_axis_c2h_cmd]
  connect_bd_intf_net -intf_net jungfraujoch_m_axis_h2c_datamover_cmd [get_bd_intf_pins jungfraujoch/m_axis_h2c_datamover_cmd] [get_bd_intf_pins pcie_dma/s_axis_h2c_cmd]
  connect_bd_intf_net -intf_net mac_100g_1_M_AXIS_100G [get_bd_intf_pins jungfraujoch/eth_in] [get_bd_intf_pins mac_100g/m_axis_eth_in]
  connect_bd_intf_net -intf_net mac_100g_1_qsfp0 [get_bd_intf_ports qsfp0] [get_bd_intf_pins mac_100g/qsfp]
  connect_bd_intf_net -intf_net pcie0_ref_1 [get_bd_intf_ports pcie0_ref] [get_bd_intf_pins pcie_dma/pcie_refclk]
  connect_bd_intf_net -intf_net pcie_dma_M_AXI [get_bd_intf_pins pcie_dma/m_axi_ctrl] [get_bd_intf_pins smartconnect_1/S00_AXI]
  connect_bd_intf_net -intf_net pcie_dma_pcie0_mgt [get_bd_intf_ports pcie0_mgt] [get_bd_intf_pins pcie_dma/pcie_mgt]
  connect_bd_intf_net -intf_net qsfp0_ref_1 [get_bd_intf_ports qsfp0_ref] [get_bd_intf_pins mac_100g/qsfp_ref]
  connect_bd_intf_net -intf_net s_axi_1 [get_bd_intf_pins jungfraujoch/s_axi] [get_bd_intf_pins smartconnect_1/M00_AXI]
  connect_bd_intf_net -intf_net s_axi_2 [get_bd_intf_pins mac_100g/s_axi] [get_bd_intf_pins smartconnect_1/M02_AXI]
  connect_bd_intf_net -intf_net s_axi_hbm_0_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_0] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p0]
  connect_bd_intf_net -intf_net s_axi_hbm_10_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_10] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p10]
  connect_bd_intf_net -intf_net s_axi_hbm_11_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_11] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p11]
  connect_bd_intf_net -intf_net s_axi_hbm_1_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_1] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p1]
  connect_bd_intf_net -intf_net s_axi_hbm_2_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_2] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p2]
  connect_bd_intf_net -intf_net s_axi_hbm_3_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_3] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p3]
  connect_bd_intf_net -intf_net s_axi_hbm_4_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_4] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p4]
  connect_bd_intf_net -intf_net s_axi_hbm_5_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_5] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p5]
  connect_bd_intf_net -intf_net s_axi_hbm_6_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_6] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p6]
  connect_bd_intf_net -intf_net s_axi_hbm_7_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_7] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p7]
  connect_bd_intf_net -intf_net s_axi_hbm_8_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_8] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p8]
  connect_bd_intf_net -intf_net s_axi_hbm_9_1 [get_bd_intf_pins hbm_infrastructure/s_axi_hbm_9] [get_bd_intf_pins jungfraujoch/m_axi_d_hbm_p9]
  connect_bd_intf_net -intf_net s_axis_h2c_data_1 [get_bd_intf_pins jungfraujoch/s_axis_h2c_data] [get_bd_intf_pins pcie_dma/m_axis_h2c_data]
  connect_bd_intf_net -intf_net smartconnect_1_M01_AXI [get_bd_intf_pins bci_admpcie9h3_0/axil] [get_bd_intf_pins smartconnect_1/M01_AXI]
  connect_bd_intf_net -intf_net smartconnect_1_M03_AXI [get_bd_intf_pins axi_quad_spi_0/AXI_LITE] [get_bd_intf_pins smartconnect_1/M03_AXI]
  connect_bd_intf_net -intf_net uc_1 [get_bd_intf_ports uc] [get_bd_intf_pins bci_admpcie9h3_0/avr]

  # Create port connections
  connect_bd_net -net axi_clk_1 [get_bd_pins axi_quad_spi_0/s_axi_aclk] [get_bd_pins bci_admpcie9h3_0/aclk] [get_bd_pins hbm_infrastructure/clk] [get_bd_pins jungfraujoch/axi_clk] [get_bd_pins mac_100g/axiclk] [get_bd_pins pcie_dma/axi_aclk] [get_bd_pins proc_sys_reset_0/slowest_sync_clk] [get_bd_pins smartconnect_1/aclk]
  connect_bd_net -net axilite_ctrl_infrastructure_qsfp0_led_busy [get_bd_ports qsfp0_led_busy] [get_bd_pins jungfraujoch/qsfp_led_busy]
  connect_bd_net -net axilite_ctrl_infrastructure_qsfp0_led_conn [get_bd_ports qsfp0_led_conn] [get_bd_pins jungfraujoch/qsfp_led_conn]
  connect_bd_net -net hbm_infrastructure_apb_complete_0 [get_bd_pins hbm_infrastructure/apb_complete_0] [get_bd_pins jungfraujoch/apb_complete]
  connect_bd_net -net hbm_infrastructure_hbm_temp_trip [get_bd_pins hbm_infrastructure/hbm_temp_trip_0] [get_bd_pins jungfraujoch/hbm_temp_trip]
  connect_bd_net -net hbm_infrastructure_hbm_temperature [get_bd_pins hbm_infrastructure/hbm_temperature_0] [get_bd_pins jungfraujoch/hbm_temperature]
  connect_bd_net -net host_mem_cmd_fifo_empty_1 [get_bd_pins jungfraujoch/host_mem_cmd_fifo_empty] [get_bd_pins pcie_dma/host_mem_cmd_fifo_empty]
  connect_bd_net -net host_mem_cmd_fifo_full_1 [get_bd_pins jungfraujoch/host_mem_cmd_fifo_full] [get_bd_pins pcie_dma/host_mem_cmd_fifo_full]
  connect_bd_net -net host_mem_data_fifo_empty_1 [get_bd_pins jungfraujoch/host_mem_data_fifo_empty] [get_bd_pins pcie_dma/host_mem_data_fifo_empty]
  connect_bd_net -net host_mem_data_fifo_full_1 [get_bd_pins jungfraujoch/host_mem_data_fifo_full] [get_bd_pins pcie_dma/host_mem_data_fifo_full]
  connect_bd_net -net mac_100g_1_eth_busy [get_bd_pins jungfraujoch/eth_busy] [get_bd_pins mac_100g/eth_busy]
  connect_bd_net -net mac_100g_1_stat_rx_aligned [get_bd_pins jungfraujoch/eth_stat_rx_aligned] [get_bd_pins mac_100g/stat_rx_aligned]
  connect_bd_net -net mac_100g_1_stat_rx_packet_bad_fcs [get_bd_pins jungfraujoch/eth_stat_rx_packet_bad_fcs] [get_bd_pins mac_100g/stat_rx_packet_bad_fcs]
  connect_bd_net -net mac_100g_1_stat_rx_status [get_bd_pins jungfraujoch/eth_stat_rx_status] [get_bd_pins mac_100g/stat_rx_status]
  connect_bd_net -net one_dout [get_bd_pins axi_quad_spi_0/keyclearb] [get_bd_pins axi_quad_spi_0/usrdoneo] [get_bd_pins one/dout] [get_bd_pins proc_sys_reset_0/dcm_locked]
  connect_bd_net -net pcie_dma_axi_aresetn [get_bd_pins axi_quad_spi_0/s_axi_aresetn] [get_bd_pins pcie_dma/axi_aresetn] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net pcie_perstn_1 [get_bd_ports pcie_perstn] [get_bd_pins pcie_dma/pcie_perstn]
  connect_bd_net -net proc_sys_reset_0_interconnect_aresetn [get_bd_pins hbm_infrastructure/resetn] [get_bd_pins jungfraujoch/axi_rst_n] [get_bd_pins mac_100g/resetn] [get_bd_pins pcie_dma/s_axis_aresetn] [get_bd_pins proc_sys_reset_0/interconnect_aresetn] [get_bd_pins smartconnect_1/aresetn]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins bci_admpcie9h3_0/aresetn] [get_bd_pins jungfraujoch/ap_rst_n] [get_bd_pins mac_100g/ap_rst_n] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
  connect_bd_net -net qsfpdd_modprs_1 [get_bd_ports qsfpdd_modprs] [get_bd_pins jungfraujoch/qsfpdd_modprs]
  connect_bd_net -net refclk300to100_refclk100 [get_bd_pins axi_quad_spi_0/ext_spi_clk] [get_bd_pins hbm_infrastructure/refclk100] [get_bd_pins mac_100g/refclk100] [get_bd_pins refclk300to100/refclk100] [get_bd_pins smartconnect_1/aclk1]
  connect_bd_net -net refclk_ibufds_inst_IBUF_OUT [get_bd_pins hbm_infrastructure/hbm_clk] [get_bd_pins refclk300to100/refclk300] [get_bd_pins refclk_ibufds_inst/IBUF_OUT]
  connect_bd_net -net zero_dout [get_bd_pins axi_quad_spi_0/gsr] [get_bd_pins axi_quad_spi_0/gts] [get_bd_pins axi_quad_spi_0/usrcclkts] [get_bd_pins axi_quad_spi_0/usrdonets] [get_bd_pins jungfraujoch/mm2s_error] [get_bd_pins jungfraujoch/s2mm_error] [get_bd_pins zero/dout]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs jungfraujoch/action_config_0/s_axi/reg0] -force
  assign_bd_address -offset 0x00060000 -range 0x00001000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs jungfraujoch/axi_bram_ctrl_0/S_AXI/Mem0] -force
  assign_bd_address -offset 0x000F0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs pcie_dma/axi_firewall_0/S_AXI_CTL/Control] -force
  assign_bd_address -offset 0x00020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs axi_quad_spi_0/AXI_LITE/Reg] -force
  assign_bd_address -offset 0x00010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs bci_admpcie9h3_0/axil/reg0] -force
  assign_bd_address -offset 0x00040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs mac_100g/cmac_usplus_0/s_axi/Reg] -force
  assign_bd_address -offset 0x00030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces pcie_dma/xdma_0/M_AXI_LITE] [get_bd_addr_segs jungfraujoch/mailbox_0/S0_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x00008000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/eth_ctrl/microblaze_0_local_memory/dlmb_bram_if_cntlr/SLMB/Mem] -force
  assign_bd_address -offset 0x00040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/cmac_usplus_0/s_axi/Reg] -force
  assign_bd_address -offset 0x00080000 -range 0x00010000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Data] [get_bd_addr_segs mac_100g/eth_ctrl/axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x00008000 -target_address_space [get_bd_addr_spaces mac_100g/eth_ctrl/microblaze_0/Instruction] [get_bd_addr_segs mac_100g/eth_ctrl/microblaze_0_local_memory/ilmb_bram_if_cntlr/SLMB/Mem] -force
  assign_bd_address

  set_property -dict [ list \
         CONFIG.CMAC_CORE_SELECT {CMACE4_X0Y1} \
         CONFIG.GT_GROUP_SELECT {X0Y8~X0Y11} \
  ] [get_bd_cells mac_100g/cmac_usplus_0]

  set_property -dict [list \
         CONFIG.pcie_blk_locn {PCIE4C_X1Y1} \
         CONFIG.en_gt_selection {true} \
         CONFIG.select_quad {GTY_Quad_227} \
  ] [get_bd_cells pcie_dma/xdma_0]

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
  validate_bd_design
  save_bd_design
}
# End of create_root_design()

##################################################################
# MAIN FLOW
##################################################################

create_root_design ""
