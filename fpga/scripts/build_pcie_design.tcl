## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

set origin_dir [file dirname [file normalize [info script]]]

set project [create_project -force -part "xcu55c-fsvh2892-2L-e" "jfjoch_pcie" "vivado"]
current_project $project

# Get the directory path for the new project
set proj_dir [get_property directory $project]

set_property ip_repo_paths [concat [get_property ip_repo_paths [current_project]] "action/ip"] [current_project]

update_ip_catalog

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}
set source_set [get_filesets sources_1]

set hdl_files [list \
  [file normalize "action/hw/hdl/action_config.v"] \
  [file normalize "action/hw/hdl/resetn_sync.v"] \
  [file normalize "action/hw/hdl/check_eth_busy.v"] \
  [file normalize "action/hw/hdl/gen_xdma_descriptor.v"] \
]

if { [llength $hdl_files] > 0 } {
  add_files -norecurse -fileset $source_set $hdl_files
}

if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset constrs_1
}
set constraint_set [get_filesets constrs_1]

# Add constraints files
# Put target .xdc as LAST in list
set constraint_files [list \
  [file normalize "$origin_dir/../xdc/pcie_u55c.xdc"] \
  [file normalize "$origin_dir/../xdc/pcie_timing.xdc"] \
]

add_files -norecurse -fileset $constraint_set $constraint_files
set_property "target_constrs_file" [lindex $constraint_files 0] $constraint_set

source $origin_dir/network_stack.tcl
source $origin_dir/hbm_u55c.tcl
source $origin_dir/jfjoch.tcl
source $origin_dir/pcie_dma.tcl
source $origin_dir/mac_100g_pcie.tcl
source $origin_dir/jfjoch_ctrl_pcie.tcl
source $origin_dir/bd_pcie.tcl >> build_pcie.log

make_wrapper -files [get_files "vivado/jfjoch_pcie.srcs/sources_1/bd/jfjoch_pcie/jfjoch_pcie.bd"] -top >> make_wrapper.log
add_files -norecurse "vivado/jfjoch_pcie.gen/sources_1/bd/jfjoch_pcie/hdl/jfjoch_pcie_wrapper.v"
set_property top jfjoch_pcie_wrapper [current_fileset]
update_compile_order -fileset sources_1

set start_eth_elf_file [file normalize "$origin_dir/../microblaze/start_eth.elf"]
add_files -norecurse $start_eth_elf_file
add_files -fileset sim_1 -norecurse [file normalize $start_eth_elf_file]
set_property SCOPED_TO_REF jfjoch_pcie [get_files -all -of_objects [get_fileset sources_1] $start_eth_elf_file]
set_property SCOPED_TO_CELLS { jfjoch_ctrl/microblaze_0 } [get_files -all -of_objects [get_fileset sources_1] $start_eth_elf_file]
set_property SCOPED_TO_REF jfjoch_pcie [get_files -all -of_objects [get_fileset sim_1] $start_eth_elf_file]
set_property SCOPED_TO_CELLS { jfjoch_ctrl/microblaze_0 } [get_files -all -of_objects [get_fileset sim_1] $start_eth_elf_file]

set_property synth_checkpoint_mode None [get_files vivado/jfjoch_pcie.srcs/sources_1/bd/jfjoch_pcie/jfjoch_pcie.bd]
generate_target all                     [get_files vivado/jfjoch_pcie.srcs/sources_1/bd/jfjoch_pcie/jfjoch_pcie.bd] >> generate_target.log

write_hw_platform -fixed -force -file vivado/jfjoch_pcie.xsa

set_property flow {Vivado Synthesis 2022} [get_runs synth_1]
set_property strategy Flow_PerfOptimized_high [get_runs synth_1]
set_property strategy Performance_ExploreWithRemap [get_runs impl_1]
