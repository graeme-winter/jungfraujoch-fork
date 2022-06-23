## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

open_project $env(HLS_TOP_FUNCTION) -reset

set_top $env(HLS_TOP_FUNCTION)

add_files $env(SRC_DIR)/$env(HLS_FILE)

open_solution solution1

# Outcome will work for all Virtex US+ HBM FPGAs (this is mostly for utilization statistics)
set_part xcvu35p-fsvh2104-2-e

create_clock -period 3.2 -name default
config_interface -m_axi_addr64=true
config_schedule -enable_dsp_full_reg=true

csynth_design

config_export -vendor {psi.ch} -version 1.0 -ipname $env(HLS_TOP_FUNCTION) -format ip_catalog -rtl verilog
export_design

close_project
exit