## Define clock frequencies
create_clock -period 3.333  -name refclk300 [get_ports {hbm_ref_clk_p}]
create_clock -period 10.000 -name refclk100 [get_pins jfjoch_pcie_i/refclk300to100/refclk100]
create_clock -period 6.206  -name qsfp0_ref_clk_p [get_ports qsfp0_ref_clk_p]
create_clock -period 10.000 -name sys_clk [get_ports pcie0_ref_clk_p]

## AVR
set_property PACKAGE_PIN BA33    [get_ports {uc_avr_ck}]
set_property IOSTANDARD LVCMOS18 [get_ports {uc_avr_ck}]
set_property PACKAGE_PIN BF34    [get_ports {uc_avr_rx}]
set_property IOSTANDARD LVCMOS18 [get_ports {uc_avr_rx}]
set_property PACKAGE_PIN BF33    [get_ports {uc_avr_tx}]
set_property IOSTANDARD LVCMOS18 [get_ports {uc_avr_tx}]

## PCIe
set_property PACKAGE_PIN AF9 [get_ports pcie0_ref_clk_p]
set_property PACKAGE_PIN AF8 [get_ports pcie0_ref_clk_n]

set_property PACKAGE_PIN AW27 [get_ports pcie_perstn]
set_property IOSTANDARD LVCMOS18 [get_ports pcie_perstn]
set_property PULLUP true [get_ports pcie_perstn]

## Ethernet
set_property PACKAGE_PIN AU46 [get_ports {qsfp0_grx_n[0]}]    ; #QSFP0_RX0_N
set_property PACKAGE_PIN AU45 [get_ports {qsfp0_grx_p[0]}]    ; #QSFP0_RX0_P
set_property PACKAGE_PIN AT44 [get_ports {qsfp0_grx_n[1]}]    ; #QSFP0_RX1_N
set_property PACKAGE_PIN AT43 [get_ports {qsfp0_grx_p[1]}]    ; #QSFP0_RX1_P
set_property PACKAGE_PIN AR46 [get_ports {qsfp0_grx_n[2]}]    ; #QSFP0_RX2_N
set_property PACKAGE_PIN AR45 [get_ports {qsfp0_grx_p[2]}]    ; #QSFP0_RX2_P
set_property PACKAGE_PIN AP44 [get_ports {qsfp0_grx_n[3]}]    ; #QSFP0_RX3_N
set_property PACKAGE_PIN AP43 [get_ports {qsfp0_grx_p[3]}]    ; #QSFP0_RX3_P

set_property PACKAGE_PIN AH43 [get_ports {qsfp0_gtx_n[0]}]    ; #QSFP0_TX0_N
set_property PACKAGE_PIN AH42 [get_ports {qsfp0_gtx_p[0]}]    ; #QSFP0_TX0_P
set_property PACKAGE_PIN AE41 [get_ports {qsfp0_gtx_n[1]}]    ; #QSFP0_TX1_N
set_property PACKAGE_PIN AE40 [get_ports {qsfp0_gtx_p[1]}]    ; #QSFP0_TX1_P
set_property PACKAGE_PIN AF43 [get_ports {qsfp0_gtx_n[2]}]    ; #QSFP0_TX2_N
set_property PACKAGE_PIN AF42 [get_ports {qsfp0_gtx_p[2]}]    ; #QSFP0_TX2_P
set_property PACKAGE_PIN AD43 [get_ports {qsfp0_gtx_n[3]}]    ; #QSFP0_TX3_N
set_property PACKAGE_PIN AD42 [get_ports {qsfp0_gtx_p[3]}]    ; #QSFP0_TX3_P

## FL: Using QSFP-DD clock at 161.1328125 MHz
set_property PACKAGE_PIN AD39   [get_ports {qsfp0_ref_clk_n}]         ; #QSFP_CLK_PIN_N
set_property PACKAGE_PIN AD38   [get_ports {qsfp0_ref_clk_p}]         ; #QSFP_CLK_PIN_P

## QSFP-DD module present
set_property PACKAGE_PIN BA14 [get_ports {qsfpdd_modprs}] ; #QSFP_MODPRS_L
set_property IOSTANDARD LVCMOS18 [get_ports {qsfpdd_modprs}]

## Reference 300 MHz clock
set_property PACKAGE_PIN AY31 [get_ports {hbm_ref_clk_p}]
set_property PACKAGE_PIN BA31 [get_ports {hbm_ref_clk_n}]
set_property IOSTANDARD LVDS [get_ports {hbm_ref_clk_*}]
set_property DIFF_TERM_ADV TERM_100 [get_ports {hbm_ref_clk_*}]

## User LEDs
set_property PACKAGE_PIN AV31    [get_ports {qsfp0_led_conn}] ; G1
set_property PACKAGE_PIN AW32    [get_ports {qsfp0_led_busy}] ; A1
set_property IOSTANDARD LVCMOS18 [get_ports {qsfp0_led_*}]

## QSPI
set_property PACKAGE_PIN AV28 [get_ports {qspi_io0_io}]
set_property PACKAGE_PIN AW28 [get_ports {qspi_io1_io}]
set_property PACKAGE_PIN BB28 [get_ports {qspi_io2_io}]
set_property PACKAGE_PIN BC28 [get_ports {qspi_io3_io}]
set_property PACKAGE_PIN AW24 [get_ports {qspi_ss_io}]
set_property IOSTANDARD LVCMOS18 [get_ports {qspi_*_io}]

## HBM debug clock

set_property C_CLK_INPUT_FREQ_HZ 100000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]

connect_debug_port dbg_hub/clk [get_pins jfjoch_pcie_i/refclk300to100/refclk100]

## BITSTREAM generation
# Configuration from SPI Flash as per XAPP1233
# Enable bitstream compression
set_property BITSTREAM.GENERAL.COMPRESS {TRUE} [ current_design ]

set_property BITSTREAM.CONFIG.EXTMASTERCCLK_EN {DIV-1} [current_design]
set_property BITSTREAM.CONFIG.SPI_32BIT_ADDR {YES} [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH {8} [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE {YES} [current_design]
set_property CONFIG_MODE {SPIx8} [current_design]

# Don't pull unused pins up or down
set_property BITSTREAM.CONFIG.UNUSEDPIN {Pullnone} [current_design]

# Set CFGBVS to GND to match schematics
set_property CFGBVS {GND} [ current_design ]

# Set CONFIG_VOLTAGE to 1.8V to match schematics
set_property CONFIG_VOLTAGE {1.8} [ current_design ]

# Set safety trigger to power down FPGA at 125degC
set_property BITSTREAM.CONFIG.OVERTEMPSHUTDOWN {Enable} [current_design]