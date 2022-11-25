## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

# Inspired on Apache License based SNAP/OC-Accel design from IBM

# Hierarchical cell: hbm_infrastructure
proc create_hier_cell_hbm_infrastructure { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_msg_id "BD_TCL-102" "ERROR" "create_hier_cell_hbm_infrastructure() - Empty argument(s)!"}
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

  create_bd_pin -dir I -type clk axi_clk
  create_bd_pin -dir I -type rst axi_resetn
  create_bd_pin -dir I -type clk refclk100

  set hbm_temperature_0 [ create_bd_pin -dir O -from 6 -to 0 hbm_temperature_0 ]
  set hbm_temperature_1 [ create_bd_pin -dir O -from 6 -to 0 hbm_temperature_1 ]
  set hbm_temp_trip_0 [ create_bd_pin -dir O hbm_temp_trip_0 ]
  set hbm_temp_trip_1 [ create_bd_pin -dir O hbm_temp_trip_1 ]
  set apb_complete_0 [ create_bd_pin -dir O apb_complete_0 ]
  set apb_complete_1 [ create_bd_pin -dir O apb_complete_1 ]

  # Create instance: resetn_sync_0, and set properties
  create_bd_cell -type module -reference {resetn_sync} {resetn_sync_0}

  # Create instance: hbm, and set properties
  set hbm [ create_bd_cell -type ip -vlnv xilinx.com:ip:hbm:1.0 hbm ]
  set_property -dict [ list \
   CONFIG.HBM_MMCM1_DIVCLK_DIV0 {1} \
   CONFIG.HBM_MMCM1_FBOUT_MULT0 {9} \
   CONFIG.HBM_MMCM_DIVCLK_DIV0 {1} \
   CONFIG.HBM_MMCM_FBOUT_MULT0 {9} \
   CONFIG.USER_APB_EN {false} \
   CONFIG.USER_APB_PCLK_0 {100} \
   CONFIG.USER_APB_PCLK_PERIOD_0 {10.0} \
   CONFIG.USER_AUTO_POPULATE {yes} \
   CONFIG.USER_CLK_SEL_LIST0 {AXI_05_ACLK} \
   CONFIG.USER_CLK_SEL_LIST1 {AXI_16_ACLK} \
   CONFIG.USER_DFI_CLK0_FREQ {450.000} \
   CONFIG.USER_HBM_CP_0 {6} \
   CONFIG.USER_HBM_CP_1 {6} \
   CONFIG.USER_HBM_DENSITY {16GB} \
   CONFIG.USER_HBM_FBDIV_0 {36} \
   CONFIG.USER_HBM_FBDIV_1 {36} \
   CONFIG.USER_HBM_HEX_CP_RES_0 {0x0000A600} \
   CONFIG.USER_HBM_HEX_CP_RES_1 {0x0000A600} \
   CONFIG.USER_HBM_HEX_FBDIV_CLKOUTDIV_0 {0x00000902} \
   CONFIG.USER_HBM_HEX_FBDIV_CLKOUTDIV_1 {0x00000902} \
   CONFIG.USER_HBM_HEX_LOCK_FB_REF_DLY_0 {0x00001f1f} \
   CONFIG.USER_HBM_HEX_LOCK_FB_REF_DLY_1 {0x00001f1f} \
   CONFIG.USER_HBM_LOCK_FB_DLY_0 {31} \
   CONFIG.USER_HBM_LOCK_FB_DLY_1 {31} \
   CONFIG.USER_HBM_LOCK_REF_DLY_0 {31} \
   CONFIG.USER_HBM_LOCK_REF_DLY_1 {31} \
   CONFIG.USER_HBM_REF_CLK_0 {100} \
   CONFIG.USER_HBM_REF_CLK_PS_0 {5000.00} \
   CONFIG.USER_HBM_REF_CLK_XDC_0 {10.00} \
   CONFIG.USER_HBM_REF_OUT_CLK_0 {1800} \
   CONFIG.USER_HBM_RES_0 {10} \
   CONFIG.USER_HBM_RES_1 {10} \
   CONFIG.USER_HBM_STACK {2} \
   CONFIG.USER_HBM_TCK_0 {900} \
   CONFIG.USER_HBM_TCK_0_PERIOD {1.1111111111111112} \
   CONFIG.USER_MC0_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC0_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC0_EN_DATA_MASK {false} \
   CONFIG.USER_MC0_EN_SBREF {true} \
   CONFIG.USER_MC0_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC0_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC0_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC0_REF_TEMP_COMP {false} \
   CONFIG.USER_MC0_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC10_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC10_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC10_EN_DATA_MASK {false} \
   CONFIG.USER_MC10_EN_SBREF {true} \
   CONFIG.USER_MC10_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC10_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC10_REF_TEMP_COMP {false} \
   CONFIG.USER_MC10_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC11_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC11_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC11_EN_DATA_MASK {false} \
   CONFIG.USER_MC11_EN_SBREF {true} \
   CONFIG.USER_MC11_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC11_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC11_REF_TEMP_COMP {false} \
   CONFIG.USER_MC11_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC12_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC12_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC12_EN_DATA_MASK {false} \
   CONFIG.USER_MC12_EN_SBREF {true} \
   CONFIG.USER_MC12_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC12_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC12_REF_TEMP_COMP {false} \
   CONFIG.USER_MC12_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC13_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC13_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC13_EN_DATA_MASK {false} \
   CONFIG.USER_MC13_EN_SBREF {true} \
   CONFIG.USER_MC13_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC13_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC13_REF_TEMP_COMP {false} \
   CONFIG.USER_MC13_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC14_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC14_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC14_EN_DATA_MASK {false} \
   CONFIG.USER_MC14_EN_SBREF {true} \
   CONFIG.USER_MC14_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC14_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC14_REF_TEMP_COMP {false} \
   CONFIG.USER_MC14_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC15_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC15_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC15_EN_DATA_MASK {false} \
   CONFIG.USER_MC15_EN_SBREF {true} \
   CONFIG.USER_MC15_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC15_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC15_REF_TEMP_COMP {false} \
   CONFIG.USER_MC15_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC1_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC1_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC1_EN_DATA_MASK {false} \
   CONFIG.USER_MC1_EN_SBREF {true} \
   CONFIG.USER_MC1_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC1_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC1_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC1_REF_TEMP_COMP {false} \
   CONFIG.USER_MC1_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC2_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC2_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC2_EN_DATA_MASK {false} \
   CONFIG.USER_MC2_EN_SBREF {true} \
   CONFIG.USER_MC2_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC2_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC2_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC2_REF_TEMP_COMP {false} \
   CONFIG.USER_MC2_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC3_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC3_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC3_EN_DATA_MASK {false} \
   CONFIG.USER_MC3_EN_SBREF {true} \
   CONFIG.USER_MC3_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC3_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC3_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC3_REF_TEMP_COMP {false} \
   CONFIG.USER_MC3_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC4_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC4_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC4_EN_DATA_MASK {false} \
   CONFIG.USER_MC4_EN_SBREF {true} \
   CONFIG.USER_MC4_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC4_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC4_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC4_REF_TEMP_COMP {false} \
   CONFIG.USER_MC4_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC5_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC5_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC5_EN_DATA_MASK {false} \
   CONFIG.USER_MC5_EN_SBREF {true} \
   CONFIG.USER_MC5_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC5_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC5_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC5_REF_TEMP_COMP {false} \
   CONFIG.USER_MC5_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC6_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC6_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC6_EN_DATA_MASK {false} \
   CONFIG.USER_MC6_EN_SBREF {true} \
   CONFIG.USER_MC6_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC6_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC6_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC6_REF_TEMP_COMP {false} \
   CONFIG.USER_MC6_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC7_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC7_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC7_EN_DATA_MASK {false} \
   CONFIG.USER_MC7_EN_SBREF {true} \
   CONFIG.USER_MC7_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC7_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC7_REF_CMD_PERIOD {0x0DB6} \
   CONFIG.USER_MC7_REF_TEMP_COMP {false} \
   CONFIG.USER_MC7_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC8_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC8_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC8_EN_DATA_MASK {false} \
   CONFIG.USER_MC8_EN_SBREF {true} \
   CONFIG.USER_MC8_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC8_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC8_REF_TEMP_COMP {false} \
   CONFIG.USER_MC8_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC9_ENABLE_ECC_CORRECTION {true} \
   CONFIG.USER_MC9_ENABLE_ECC_SCRUBBING {true} \
   CONFIG.USER_MC9_EN_DATA_MASK {false} \
   CONFIG.USER_MC9_EN_SBREF {true} \
   CONFIG.USER_MC9_INITILIZE_MEM_USING_ECC_SCRUB {true} \
   CONFIG.USER_MC9_LOOKAHEAD_SBRF {true} \
   CONFIG.USER_MC9_REF_TEMP_COMP {false} \
   CONFIG.USER_MC9_TRAFFIC_OPTION {Linear} \
   CONFIG.USER_MC_ENABLE_06 {FALSE} \
   CONFIG.USER_MC_ENABLE_07 {FALSE} \
   CONFIG.USER_MC_ENABLE_08 {FALSE} \
   CONFIG.USER_MC_ENABLE_09 {FALSE} \
   CONFIG.USER_MC_ENABLE_10 {FALSE} \
   CONFIG.USER_MC_ENABLE_11 {FALSE} \
   CONFIG.USER_MC_ENABLE_12 {FALSE} \
   CONFIG.USER_MC_ENABLE_13 {FALSE} \
   CONFIG.USER_MC_ENABLE_14 {FALSE} \
   CONFIG.USER_MC_ENABLE_15 {FALSE} \
   CONFIG.USER_MC_ENABLE_APB_01 {FALSE} \
   CONFIG.USER_MEMORY_DISPLAY {16384} \
   CONFIG.USER_PHY_ENABLE_08 {TRUE} \
   CONFIG.USER_PHY_ENABLE_09 {TRUE} \
   CONFIG.USER_PHY_ENABLE_10 {TRUE} \
   CONFIG.USER_PHY_ENABLE_11 {TRUE} \
   CONFIG.USER_PHY_ENABLE_12 {TRUE} \
   CONFIG.USER_PHY_ENABLE_13 {TRUE} \
   CONFIG.USER_PHY_ENABLE_14 {TRUE} \
   CONFIG.USER_PHY_ENABLE_15 {TRUE} \
   CONFIG.USER_SAXI_31 {false} \
   CONFIG.USER_SINGLE_STACK_SELECTION {LEFT} \
   CONFIG.USER_SWITCH_ENABLE_00 {FALSE} \
   CONFIG.USER_SWITCH_ENABLE_01 {FALSE} \
   CONFIG.USER_TEMP_POLL_CNT_0 {100000} \
   CONFIG.USER_XSDB_INTF_EN {TRUE} \
   CONFIG.USER_tFAW_0 {0x00F} \
   CONFIG.USER_tRAS_0 {0x1E} \
   CONFIG.USER_tRCDRD_0 {0x0D} \
   CONFIG.USER_tRCDWR_0 {0x09} \
   CONFIG.USER_tRC_0 {0x2B} \
   CONFIG.USER_tRFCSB_0 {0x90} \
   CONFIG.USER_tRFC_0 {0x0EA} \
   CONFIG.USER_tRP_0 {0x0D} \
   CONFIG.USER_tRRDL_0 {0x04} \
   CONFIG.USER_tRRDS_0 {0x04} \
   CONFIG.USER_tRREFD_0 {0x8} \
   CONFIG.USER_tWR_0 {0x0F} \
   CONFIG.USER_tXP_0 {0x07} \
   ] $hbm
  connect_bd_net [get_bd_pins hbm_temperature_0] [get_bd_pins hbm/DRAM_0_STAT_TEMP]
  connect_bd_net [get_bd_pins hbm_temperature_1] [get_bd_pins hbm/DRAM_1_STAT_TEMP]
  connect_bd_net [get_bd_pins hbm_temp_trip_0] [get_bd_pins hbm/DRAM_0_STAT_CATTRIP]
  connect_bd_net [get_bd_pins hbm_temp_trip_1] [get_bd_pins hbm/DRAM_1_STAT_CATTRIP]
  connect_bd_net [get_bd_pins apb_complete_0] [get_bd_pins hbm/apb_complete_0]
  connect_bd_net [get_bd_pins apb_complete_1] [get_bd_pins hbm/apb_complete_1]

  connect_bd_net [get_bd_pins axi_resetn] [get_bd_pins resetn_sync_0/in_resetn]
  connect_bd_net [get_bd_pins refclk100] [get_bd_pins hbm/APB_0_PCLK] [get_bd_pins hbm/APB_1_PCLK] [get_bd_pins resetn_sync_0/clk] [get_bd_pins hbm/HBM_REF_CLK_0] [get_bd_pins hbm/HBM_REF_CLK_1]
  connect_bd_net [get_bd_pins hbm/APB_0_PRESET_N] [get_bd_pins hbm/APB_1_PRESET_N] [get_bd_pins resetn_sync_0/out_resetn]

  for {set i 0} {$i < 12} {incr i} {
      create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi_hbm_$i

      set cell [create_bd_cell -type ip -vlnv {xilinx.com:ip:axi_register_slice:*} axi_register_slice_$i ]
      set_property -dict [ list \
          CONFIG.REG_AW {7}                  \
          CONFIG.REG_AR {7}                  \
          CONFIG.REG_W {1}                   \
          CONFIG.REG_R {1}                   \
          CONFIG.REG_B {7}                   \
          ] $cell

      set axi_prot_conv [create_bd_cell -type ip -vlnv {xilinx.com:ip:axi_protocol_converter:*} axi4_to_axi3_$i]
      set_property -dict [ list \
          CONFIG.TRANSLATION_MODE {0} \
          ] $axi_prot_conv

      connect_bd_net [get_bd_pins axi_clk] [get_bd_pins axi4_to_axi3_$i/aclk] [get_bd_pins axi_register_slice_$i/aclk]

      connect_bd_net [get_bd_pins axi_resetn] [get_bd_pins axi_register_slice_$i/aresetn]
      connect_bd_net [get_bd_pins axi_resetn] [get_bd_pins axi4_to_axi3_$i/aresetn]
      connect_bd_intf_net [get_bd_intf_pins axi4_to_axi3_$i/M_AXI] [get_bd_intf_pins axi_register_slice_$i/S_AXI]
      connect_bd_intf_net [get_bd_intf_pins axi4_to_axi3_$i/S_AXI] [get_bd_intf_pins s_axi_hbm_$i]
      if {$i < 10} {
        connect_bd_net [get_bd_pins axi_resetn] [get_bd_pins hbm/AXI_0${i}_ARESET_N]
        connect_bd_net [get_bd_pins axi_clk] [get_bd_pins hbm/AXI_0${i}_ACLK]
        connect_bd_intf_net [get_bd_intf_pins axi_register_slice_$i/M_AXI] [get_bd_intf_pins hbm/SAXI_0${i}*]
      } else {
        connect_bd_net [get_bd_pins axi_resetn] [get_bd_pins hbm/AXI_${i}_ARESET_N]
        connect_bd_net [get_bd_pins axi_clk] [get_bd_pins hbm/AXI_${i}_ACLK]
        connect_bd_intf_net [get_bd_intf_pins axi_register_slice_$i/M_AXI] [get_bd_intf_pins hbm/SAXI_${i}*]
      }
  }

  # Restore current instance
  current_bd_instance $oldCurInst
}
