# PCIe reset
set_false_path -from [get_ports pcie_perstn]

# Reset synchronizers
set_false_path -to [get_pins jfjoch_pcie_i/*/resetn_sync_0/U0/q?_reg/CLR]

set_false_path -from [get_pins jfjoch_pcie_i/*/cmac_usplus_0/*/i_jfjoch_pcie_cmac_usplus_0_0_top/*/RX_CLK] -to [get_pins jfjoch_pcie_i/*/action_config_0/*/reg_eth_stat_rx_*_1_reg/D]
set_false_path -from [get_pins jfjoch_pcie_i/*/check_eth_busy_0/*/eth_busy_reg/C] -to [get_pins jfjoch_pcie_i/*/action_config_0/*/reg_eth_busy_1_reg/D]

# This affects HBM Temperature & HBM status + 100G status/align (anyway readout from host is very slow in comparison to FPGA clock)
set_false_path -to [get_pins jfjoch_pcie_i/*/action_config_0/U0/*/D]

# From 100G example design
set_false_path -to [get_pins -leaf -of_objects [get_cells -hier *cdc_to* -filter {is_sequential}] -filter {NAME=~*cmac_cdc*/*/D}]

# This affects HBM Temperature & status
set_false_path -from [get_clocks refclk100] -to [get_clocks clock_afu]

set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -to [get_clocks refclk100] -datapath_only 3.103
set_max_delay -from [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -to [get_clocks refclk100] -datapath_only 3.103
set_max_delay -from [get_clocks refclk100] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/RXOUTCLK}]] -datapath_only 10.000
set_max_delay -from [get_clocks refclk100] -to [get_clocks -of_objects [get_pins -hierarchical -filter {NAME =~ */mac_100g*/*/channel_inst/*_CHANNEL_PRIM_INST/TXOUTCLK}]] -datapath_only 10.000

create_waiver -type CDC -id {CDC-11} -user "cmac" -tags "10930"\
-desc "The CDC-11 warning is waived as fan-out is expected for this stat signal"\
-to [get_pins -of [get_cells -hier -filter {name =~ */*stat_rx_aligned/*cdc_to*}] -filter {name =~ *D}]
