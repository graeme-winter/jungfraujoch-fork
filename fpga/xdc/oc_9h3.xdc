# set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
# connect_debug_port dbg_hub/clk [get_nets */APB_0_PCLK]

#create_pblock pblock_opencapi
#resize_pblock pblock_opencapi -add CLOCKREGION_X0Y0:CLOCKREGION_X7Y3
#add_cells_to_pblock pblock_opencapi [get_cells [list bsp oc_func cfg ]]

# This affects reset synchronizers
set_false_path -to [get_pins oc_func/fw_afu/action_w/action_i/*/resetn_sync_0/U0/q?_reg/CLR]

# From 100G example design
set_false_path -to [get_pins -leaf -of_objects [get_cells -hier *cdc_to* -filter {is_sequential}] -filter {NAME=~*cmac_cdc*/*/D}]

create_clock -period 10.000 -name refclk100 [get_pins oc_func/fw_afu/action_w/action_i/refclk300to100/refclk100]

set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -to [get_clocks refclk100] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -to [get_clocks refclk100] -datapath_only 3.103
set_max_delay -from [get_clocks refclk100] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -datapath_only 10.000
set_max_delay -from [get_clocks refclk100] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -datapath_only 10.000

create_waiver -type CDC -id {CDC-11} -user "cmac" -tags "10930"\
-desc "The CDC-11 warning is waived as fan-out is expected for this stat signal"\
-to [get_pins -of [get_cells -hier -filter {name =~ */*stat_rx_aligned/*cdc_to*}] -filter {name =~ *D}]

set_property C_CLK_INPUT_FREQ_HZ 100000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]

connect_debug_port dbg_hub/clk [get_pins oc_func/fw_afu/action_w/action_i/refclk300to100/refclk100]

#set_max_delay -from [get_pins oc_func/fw_afu/action_w/action_i/hbm_infrastructure/hbm/inst/ONE_STACK.u_hbm_top/ONE_STACK_HBM.hbm_one_stack_intf/HBM_SNGLBLI_INTF_APB_INST/PCLK] -to [get_pins oc_func/fw_afu/action_w/action_i/hbm_infrastructure/hbm/inst/ONE_STACK.u_hbm_top/ONE_STACK_HBM.u_xsdb_top_0/xsdb2adb_u0/*/*] 4.0

# This affects HBM Temperature & HBM status (anyway readout from host is very slow in comparison to FPGA clock)
set_false_path -to [get_pins oc_func/fw_afu/action_w/action_i/*/action_config_0/U0/*/D]

set_false_path -from [get_pins oc_func/fw_afu/action_w/action_i/*/cmac_usplus_0/inst/i_action_cmac_usplus_0_0_top/*/RX_CLK] -to [get_pins oc_func/fw_afu/action_w/action_i/*/action_config_0/U0/reg_eth_stat_rx_*_1_reg/D]
set_false_path -from [get_pins oc_func/fw_afu/action_w/action_i/*/check_eth_busy_0/U0/eth_busy_reg/C] -to [get_pins oc_func/fw_afu/action_w/action_i/*/action_config_0/U0/reg_eth_busy_1_reg/D]
