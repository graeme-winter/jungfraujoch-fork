SET(FPGACARD "AD9H335" CACHE STRING "Type of FPGA card for OpenCAPI workflow")
SET(ACTION_ROOT ${CMAKE_CURRENT_BINARY_DIR}/action)
SET(OCSE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ocse)

FIND_PROGRAM(VIVADO vivado DOC "Xilinx Vivado")
IF (VIVADO)
    MESSAGE(STATUS "Xilinx Vivado found: ${VIVADO}")
ELSE()
    MESSAGE(STATUS "Xilinx Vivado not found")
ENDIF()

FIND_PROGRAM(VIVADO_HLS NAMES vitis_hls vivado_hls DOC "Xilinx HLS")
IF (VIVADO_HLS)
    MESSAGE(STATUS "Xilinx HLS compiler found: ${VIVADO_HLS}")
ELSE()
    MESSAGE(STATUS "Xilinx HLS compiler not found")
ENDIF()

IF (VIVADO OR (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "ppc64le") OR VIVADO)
    SET(USE_OPENCAPI ON CACHE BOOL "Include OpenCAPI routines")
ELSE()
    SET(USE_OPENCAPI OFF CACHE BOOL "Include OpenCAPI routines")
ENDIF()

INCLUDE_DIRECTORIES(include)

ADD_SUBDIRECTORY(hls)
ADD_SUBDIRECTORY(host)
ADD_SUBDIRECTORY(receiver)
ADD_SUBDIRECTORY(pcie_driver)

IF(VIVADO_HLS AND VIVADO)
    ADD_CUSTOM_COMMAND(OUTPUT action/hw/hdl/action_config.v
            COMMAND ${CMAKE_COMMAND} -E env FPGACARD=${FPGACARD} SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR} HLS_IP_DIR=${CMAKE_CURRENT_BINARY_DIR}/action/ip/hls bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup_action.sh
            DEPENDS hls scripts/OC-AD9H3.hls_rx100G.nosim.defconfig  scripts/OC-AD9H3.hls_rx100G.xcelium.defconfig scripts/OC-AD9H335.hls_rx100G.nosim.defconfig  scripts/OC-AD9H335.hls_rx100G.xcelium.defconfig hdl/action_config.v hdl/check_datamover_error.v hdl/check_eth_busy.v hdl/gen_xdma_descriptor.v hdl/refclk300to100.v hdl/action_wrapper.v hdl/resetn_sync.v scripts/bd_opencapi.tcl scripts/bd_pcie.tcl scripts/jfjoch.tcl scripts/network_stack.tcl scripts/hbm.tcl scripts/mac_100g.tcl scripts/pcie_dma.tcl scripts/setup_action.sh
            )

    ADD_CUSTOM_TARGET(action DEPENDS action/hw/hdl/action_config.v hls)

    ADD_CUSTOM_TARGET(action_model
            DEPENDS action jfjoch_action_test
            COMMAND make -f ${CMAKE_CURRENT_SOURCE_DIR}/oc-accel/Makefile -C ${CMAKE_CURRENT_SOURCE_DIR}/oc-accel model)

    ADD_CUSTOM_TARGET(action_sim DEPENDS action_model WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/oc-accel/hardware/sim
            COMMAND ./run_sim -quiet -app ../../../build/fpga/receiver/jfjoch_action_test /home/filip/Jungfraujoch/tests/test_data/gainMaps_M049.bin 0 1 > sim_a.out
            )

    ADD_CUSTOM_TARGET(action_opencapi
            DEPENDS action
            COMMAND make -f ${CMAKE_CURRENT_SOURCE_DIR}/oc-accel/Makefile -C ${CMAKE_CURRENT_SOURCE_DIR}/oc-accel hw_project
            COMMAND ${CMAKE_COMMAND} -E env FLOW=opencapi VIV_PROJECT_PATH=${CMAKE_CURRENT_SOURCE_DIR}/oc-accel/hardware/viv_project/framework.xpr ${VIVADO} -notrace -mode batch -source ${CMAKE_CURRENT_BINARY_DIR}/action/scripts/synth_and_impl.tcl
            )

    ADD_CUSTOM_TARGET(action_pcie DEPENDS action/hw/hdl/action_config.v hls
            COMMAND ${VIVADO} -notrace -mode batch -source ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build_pcie_design.tcl
            COMMAND ${CMAKE_COMMAND} -E env FLOW=pcie VIV_PROJECT_PATH=${CMAKE_CURRENT_BINARY_DIR}/vivado/jfjoch_pcie.xpr ${VIVADO} -notrace -mode batch -source ${CMAKE_CURRENT_BINARY_DIR}/action/scripts/synth_and_impl.tcl
            )

ENDIF()