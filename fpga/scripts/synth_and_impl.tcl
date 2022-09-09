## Copyright (2019-2022) Paul Scherrer Institute
## SPDX-License-Identifier: CERN-OHL-S-2.0

##
open_project $::env(VIV_PROJECT_PATH)
set flow     $::env(FLOW)

set part [get_property PART [current_project]]
if {$part eq {xcvu35p-fsvh2104-2-e}} {
    set fpga_flash_size 128
    set fpga_name "VU35P"
    set interface "SPIx8"
} elseif {$part eq {xcu55c-fsvh2892-2L-e}} {
    set fpga_flash_size 128
    set fpga_name "U55C"
    set interface "SPIx4"
} elseif {$part eq {xcu50-fsvh2104-2-e}} {
    set fpga_flash_size 128
    set fpga_name "U50"
    set interface "SPIx8"
} else {
    set fpga_flash_size 64
    set fpga_name "VU33P"
    set interface "SPIx8"
}

set bitstream_name jfjoch_${flow}_${fpga_name}_rel@RELEASE_LEVEL@_@GIT_SHA1@

puts "Part $part Flash size ${fpga_flash_size} Flow ${flow} Image name ${bitstream_name}"
set log_file synth.log
puts "Synthesis..."
synth_design -directive PerformanceOptimized -flatten_hierarchy rebuilt -bufg 12 -fanout_limit 400 -fsm_extraction one_hot -resource_sharing off -no_lc -shreg_min_size 5 -cascade_dsp auto -control_set_opt_threshold auto -gated_clock_conversion off > synth.log

puts "Opt design..."
opt_design -directive Explore > opt_design.log
xilinx::designutils::report_failfast

write_checkpoint -force jungfraujoch_synth.dcp

puts "Place design..."
place_design -directive Explore > place_design.log

set WNSPLACE [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
puts "Place WNS (before optimization): $WNSPLACE ps"

puts "Phys opt design..."
set NLOOPS 3

set WNS [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
for {set i 0} {$i < $NLOOPS} {incr i} {
    if {$WNS < 0.000} {
        phys_opt_design -directive AggressiveExplore >> phys_opt_design.log
        phys_opt_design -directive AggressiveFanoutOpt >> phys_opt_design.log
        phys_opt_design -directive AlternateReplication >> phys_opt_design.log
        set WNS [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
        puts "Iter $i WNS: $WNS ps"
    }
}

set WNSPLACE [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
puts "    ***** Place WNS: $WNSPLACE ps *****"
write_checkpoint -force jungfraujoch_place.dcp

if {$WNSPLACE > -1.000} {
    puts "Route design..."
    route_design -directive AggressiveExplore -tns_cleanup -ultrathreads > route_design.log
    set WNSROUTE [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
    puts "Route WNS (before optimization): $WNSROUTE ps"

    write_checkpoint -force jungfraujoch_route.dcp
    report_utilization -no_primitives

    if {$WNSROUTE < 0.0} {
        puts "Phys opt design after routing (1st round) ..."
        phys_opt_design -directive AggressiveExplore > phys_opt_design_post_route.log
        set WNSROUTE [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
        puts "Route WNS (1st round optimization): $WNSROUTE ps"
    }

    if {$WNSROUTE < 0.0} {
        puts "Phys opt design after routing (2nd round) ..."
        phys_opt_design -directive AggressiveExplore >> phys_opt_design_post_route.log
        set WNSROUTE [ get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup] ]
        puts "Route WNS (2nd round optimization): $WNSROUTE ps"
    }
    write_checkpoint -force jungfraujoch_route_opt.dcp
    
    puts ""
    puts "    ***** Final WNS: $WNSROUTE ps *****"
    puts ""

    if {$WNSROUTE >= -0.050} {
        write_bitstream -force -file ${bitstream_name} >> bitstream.log
        write_cfgmem -force -size ${fpga_flash_size} -format bin -interface ${interface} -loadbit "up 0x0 ${bitstream_name}.bit" ${bitstream_name} >> cfgmem.log
        puts "    ***** Bitstream ${bitstream_name}.bin written! *****"
        puts ""
    } else {
        puts "    ***** Cannot write bitstream due to negative WNS *****"
        puts ""
        close_project
        exit 1
    }
} else {
    puts "    ***** WNS place < -1 ns, no chance of success *****"
    close_project
    exit 1
}
