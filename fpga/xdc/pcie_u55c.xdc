## Define clock frequencies
create_clock -period 10.000 -name refclk100 [get_ports {ref100_clk_p[0]}]
create_clock -period 6.206  -name qsfp0_ref_clk_p [get_ports qsfp0_ref_clk_p]
create_clock -period 6.206  -name qsfp1_ref_clk_p [get_ports qsfp1_ref_clk_p]
create_clock -period 10.000 -name pcie0_clk [get_ports pcie0_ref_clk_p]
create_clock -period 10.000 -name pcie1_clk [get_ports pcie1_ref_clk_p]

## HBM cattrip
set_property PACKAGE_PIN BE45     [get_ports {hbm_cattrip}]
set_property IOSTANDARD  LVCMOS18 [get_ports {hbm_cattrip}]

## PCIe
set_property PACKAGE_PIN AR14     [get_ports {pcie1_ref_clk_n}]
set_property PACKAGE_PIN AR15     [get_ports {pcie1_ref_clk_p}]
set_property PACKAGE_PIN AL14     [get_ports {pcie0_ref_clk_n}]
set_property PACKAGE_PIN AL15     [get_ports {pcie0_ref_clk_p}]

set_property PACKAGE_PIN BF41     [get_ports {pcie_perstn}]
set_property IOSTANDARD  LVCMOS18 [get_ports {pcie_perstn}]

## Ethernet QSFP
set_property PACKAGE_PIN AD52     [get_ports {qsfp0_grx_n[0]}]
set_property PACKAGE_PIN AC54     [get_ports {qsfp0_grx_n[1]}]
set_property PACKAGE_PIN AC50     [get_ports {qsfp0_grx_n[2]}]
set_property PACKAGE_PIN AB52     [get_ports {qsfp0_grx_n[3]}]
set_property PACKAGE_PIN AD51     [get_ports {qsfp0_grx_p[0]}]
set_property PACKAGE_PIN AC53     [get_ports {qsfp0_grx_p[1]}]
set_property PACKAGE_PIN AC49     [get_ports {qsfp0_grx_p[2]}]
set_property PACKAGE_PIN AB51     [get_ports {qsfp0_grx_p[3]}]

set_property PACKAGE_PIN AD47     [get_ports {qsfp0_gtx_n[0]}]
set_property PACKAGE_PIN AC45     [get_ports {qsfp0_gtx_n[1]}]
set_property PACKAGE_PIN AB47     [get_ports {qsfp0_gtx_n[2]}]
set_property PACKAGE_PIN AA49     [get_ports {qsfp0_gtx_n[3]}]
set_property PACKAGE_PIN AD46     [get_ports {qsfp0_gtx_p[0]}]
set_property PACKAGE_PIN AC44     [get_ports {qsfp0_gtx_p[1]}]
set_property PACKAGE_PIN AB46     [get_ports {qsfp0_gtx_p[2]}]
set_property PACKAGE_PIN AA48     [get_ports {qsfp0_gtx_p[3]}]

set_property PACKAGE_PIN AA54     [get_ports {qsfp1_grx_n[0]}]
set_property PACKAGE_PIN Y52      [get_ports {qsfp1_grx_n[1]}]
set_property PACKAGE_PIN W54      [get_ports {qsfp1_grx_n[2]}]
set_property PACKAGE_PIN V52      [get_ports {qsfp1_grx_n[3]}]
set_property PACKAGE_PIN AA53     [get_ports {qsfp1_grx_p[0]}]
set_property PACKAGE_PIN Y51      [get_ports {qsfp1_grx_p[1]}]
set_property PACKAGE_PIN W53      [get_ports {qsfp1_grx_p[2]}]
set_property PACKAGE_PIN V51      [get_ports {qsfp1_grx_p[3]}]

set_property PACKAGE_PIN AA45     [get_ports {qsfp1_gtx_n[0]}]
set_property PACKAGE_PIN Y47      [get_ports {qsfp1_gtx_n[1]}]
set_property PACKAGE_PIN W49      [get_ports {qsfp1_gtx_n[2]}]
set_property PACKAGE_PIN W45      [get_ports {qsfp1_gtx_n[3]}]
set_property PACKAGE_PIN AA44     [get_ports {qsfp1_gtx_p[0]}]
set_property PACKAGE_PIN Y46      [get_ports {qsfp1_gtx_p[1]}]
set_property PACKAGE_PIN W48      [get_ports {qsfp1_gtx_p[2]}]
set_property PACKAGE_PIN W44      [get_ports {qsfp1_gtx_p[3]}]

set_property PACKAGE_PIN AD43     [get_ports {qsfp0_ref_clk_n}]
set_property PACKAGE_PIN AD42     [get_ports {qsfp0_ref_clk_p}]
set_property PACKAGE_PIN AB43     [get_ports {qsfp1_ref_clk_n}]
set_property PACKAGE_PIN AB42     [get_ports {qsfp1_ref_clk_n}]

## Reference 100 MHz clock
set_property PACKAGE_PIN BL10     [get_ports {ref100_clk_n}]
set_property IOSTANDARD LVDS      [get_ports {ref100_clk_n}]
set_property PACKAGE_PIN BK10     [get_ports {ref100_clk_p}]
set_property IOSTANDARD LVDS      [get_ports {ref100_clk_p}]

## User LEDs
set_property PACKAGE_PIN BK11     [get_ports {qsfp0_led_conn}]
set_property PACKAGE_PIN BL13     [get_ports {qsfp0_led_busy}]
set_property IOSTANDARD LVCMOS18  [get_ports {qsfp0_led_*}]

set_property PACKAGE_PIN BK15     [get_ports {qsfp1_led_conn}]
set_property PACKAGE_PIN BK14     [get_ports {qsfp1_led_busy}]
set_property IOSTANDARD LVCMOS18  [get_ports {qsfp1_led_*}]

## Satellite

set_property PACKAGE_PIN BJ42     [get_ports {satellite_uart_0_rxd}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_uart_0_rxd}]
set_property PACKAGE_PIN BH42     [get_ports {satellite_uart_0_txd}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_uart_0_txd}]

set_property PACKAGE_PIN BE46     [get_ports {satellite_gpio_0[0]}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_gpio_0[0]}]
set_property PACKAGE_PIN BH46     [get_ports {satellite_gpio_0[1]}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_gpio_0[1]}]
set_property PACKAGE_PIN BF45     [get_ports {satellite_gpio_0[2]}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_gpio_0[2]}]
set_property PACKAGE_PIN BF46     [get_ports {satellite_gpio_0[3]}]
set_property IOSTANDARD  LVCMOS18 [get_ports {satellite_gpio_0[3]}]

## HBM debug clock

set_property C_CLK_INPUT_FREQ_HZ 100000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]

connect_debug_port dbg_hub/clk [get_pins jfjoch_pcie_i/refclk_ibufds_inst/IBUF_OUT]

## BITSTREAM generation
set_property CONFIG_VOLTAGE 1.8                        [current_design]
set_property BITSTREAM.CONFIG.CONFIGFALLBACK Enable    [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE           [current_design]
set_property CONFIG_MODE SPIx4                         [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4           [current_design]
set_property BITSTREAM.CONFIG.EXTMASTERCCLK_EN disable [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 63.8          [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES        [current_design]
set_property BITSTREAM.CONFIG.UNUSEDPIN Pullup         [current_design]
set_property BITSTREAM.CONFIG.SPI_32BIT_ADDR Yes       [current_design]
