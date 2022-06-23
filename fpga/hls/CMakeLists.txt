ADD_LIBRARY( HLSSimulation STATIC
        jf_conversion.cpp
        data_collection_fsm.cpp
        timer.cpp
        hls_jfjoch.h
        ../../common/Definitions.h
        load_calibration.cpp
        internal_packet_generator.cpp
        host_writer.cpp
        ethernet.cpp
        ipv4.cpp
        icmp.cpp arp.cpp
        ip_header_checksum.h
        udp.cpp
        sls_detector.cpp
        hls_bitshuffle.cpp)

TARGET_LINK_LIBRARIES(HLSSimulation CommonFunctions)
TARGET_COMPILE_DEFINITIONS(HLSSimulation PRIVATE -DNO_SYNTH)

IF(VIVADO_HLS)

IF(NOT HLS_SOLUTION_NAME)
SET(HLS_SOLUTION_NAME base1)
ENDIF()

FUNCTION( MAKE_HLS_MODULE SRC_FILE FUNCTION_NAME)
    ADD_CUSTOM_COMMAND(OUTPUT psi_ch_hls_${FUNCTION_NAME}_1_0.zip
            COMMAND ${CMAKE_COMMAND} -E env SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR} HLS_FILE=${SRC_FILE} HLS_TOP_FUNCTION=${FUNCTION_NAME} ${VIVADO_HLS} -f ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/synth_hls_function.tcl > hls_${FUNCTION_NAME}.log
            COMMAND ${CMAKE_COMMAND} -E env HLS_DIR=${CMAKE_CURRENT_BINARY_DIR}/${FUNCTION_NAME}/solution1 CURR_DIR=${CMAKE_CURRENT_BINARY_DIR} bash ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/check_hls.sh ${FUNCTION_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${FUNCTION_NAME}/solution1/impl/ip/psi_ch_hls_${FUNCTION_NAME}_1_0.zip .
            DEPENDS ${SRC_FILE} hls_jfjoch.h ../../common/Definitions.h)
ENDFUNCTION(MAKE_HLS_MODULE)

MAKE_HLS_MODULE(data_collection_fsm.cpp data_collection_fsm)
MAKE_HLS_MODULE(timer.cpp timer_host)
MAKE_HLS_MODULE(timer.cpp timer_hbm)
MAKE_HLS_MODULE(jf_conversion.cpp jf_conversion)
MAKE_HLS_MODULE(load_calibration.cpp load_calibration)
MAKE_HLS_MODULE(host_writer.cpp host_writer)
MAKE_HLS_MODULE(internal_packet_generator.cpp internal_packet_generator)
MAKE_HLS_MODULE(icmp.cpp icmp)
MAKE_HLS_MODULE(ipv4.cpp ipv4)
MAKE_HLS_MODULE(ethernet.cpp ethernet)
MAKE_HLS_MODULE(arp.cpp arp)
MAKE_HLS_MODULE(udp.cpp udp)
MAKE_HLS_MODULE(sls_detector.cpp sls_detector)
MAKE_HLS_MODULE(hls_bitshuffle.cpp bitshuffle)

SET (HLS_IPS psi_ch_hls_data_collection_fsm_1_0.zip
             psi_ch_hls_timer_host_1_0.zip
             psi_ch_hls_timer_hbm_1_0.zip
             psi_ch_hls_jf_conversion_1_0.zip
             psi_ch_hls_load_calibration_1_0.zip
             psi_ch_hls_internal_packet_generator_1_0.zip
             psi_ch_hls_bitshuffle_1_0.zip
             psi_ch_hls_ethernet_1_0.zip
             psi_ch_hls_ipv4_1_0.zip
             psi_ch_hls_arp_1_0.zip
             psi_ch_hls_udp_1_0.zip
             psi_ch_hls_sls_detector_1_0.zip
             psi_ch_hls_icmp_1_0.zip
             psi_ch_hls_host_writer_1_0.zip)

SET (HLS_IPS ${HLS_IPS} PARENT_SCOPE)
ADD_CUSTOM_TARGET(hls DEPENDS ${HLS_IPS})

ENDIF()