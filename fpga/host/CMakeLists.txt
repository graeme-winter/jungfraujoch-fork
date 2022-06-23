ADD_LIBRARY(JFJochReceiver STATIC
        JFJochReceiverService.cpp JFJochReceiverService.h
        JFJochReceiverTest.cpp JFJochReceiverTest.h
        JFJochReceiver.cpp JFJochReceiver.h
        GenerateOpenCAPIDeviceName.h
        AcquisitionDevice.cpp AcquisitionDevice.h
        AcquisitionDeviceFilter.cpp AcquisitionDeviceFilter.h
        AcquisitionDeviceCounters.cpp AcquisitionDeviceCounters.h
        MockAcquisitionDevice.cpp MockAcquisitionDevice.h
        HLSSimulatedDevice.cpp HLSSimulatedDevice.h
        Completion.cpp Completion.h ActionConfig.h)
TARGET_LINK_LIBRARIES(JFJochReceiver CommonFunctions)

ADD_EXECUTABLE(jfjoch_receiver jfjoch_receiver.cpp)
TARGET_LINK_LIBRARIES(jfjoch_receiver JFJochReceiver CommonFunctions HLSSimulation )
INSTALL(TARGETS jfjoch_receiver RUNTIME)

IF (VIVADO OR (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "ppc64le") OR VIVADO)
    SET(USE_OPENCAPI ON CACHE BOOL "Include OpenCAPI routines")
ELSE()
    SET(USE_OPENCAPI OFF CACHE BOOL "Include OpenCAPI routines")
ENDIF()

IF(USE_OPENCAPI)
    TARGET_COMPILE_DEFINITIONS(jfjoch_receiver PRIVATE -DJFJOCH_USE_OPENCAPI)
    TARGET_LINK_LIBRARIES(jfjoch_receiver JungfrajochFPGA ${OCXL} )
    ADD_LIBRARY(JungfrajochFPGA STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/software/lib/osnap.c OpenCAPIDevice.cpp OpenCAPIDevice.h)
    TARGET_INCLUDE_DIRECTORIES(JungfrajochFPGA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/actions/include/ ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/software/include/)

    ADD_EXECUTABLE(jfjoch_action_test jfjoch_action_test.cpp ../hls/hls_jfjoch.h)
    ADD_EXECUTABLE(jfjoch_oc_status jfjoch_oc_status.cpp)

    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "ppc64le")
        FIND_LIBRARY(OCXL ocxl REQUIRED)
    else()
        ADD_CUSTOM_TARGET(ocse COMMAND make WORKING_DIRECTORY ${OCSE_ROOT})
        ADD_DEPENDENCIES(JungfrajochFPGA ocse)
        TARGET_INCLUDE_DIRECTORIES(JungfrajochFPGA PUBLIC ${OCSE_ROOT}/libocxl ${OCSE_ROOT}/common)
        SET(OCXL ${OCSE_ROOT}/libocxl/libocxl.so)
    endif()

    TARGET_LINK_LIBRARIES(JungfrajochFPGA JFJochReceiver CommonFunctions ${OCXL} -pthread)
    TARGET_LINK_LIBRARIES(jfjoch_action_test JungfrajochFPGA)
    TARGET_LINK_LIBRARIES(jfjoch_oc_status JungfrajochFPGA)
    INSTALL(TARGETS jfjoch_oc_status RUNTIME)
    INSTALL(TARGETS jfjoch_action_test RUNTIME)
ENDIF()

FIND_LIBRARY(NUMA_LIBRARY NAMES numa DOC "NUMA Library")
CHECK_INCLUDE_FILE(numaif.h HAS_NUMAIF)

IF(HAS_NUMAIF AND NUMA_LIBRARY)
    TARGET_COMPILE_DEFINITIONS(JFJochReceiver PRIVATE -DJFJOCH_USE_NUMA)
    TARGET_LINK_LIBRARIES(JFJochReceiver ${NUMA_LIBRARY})
    MESSAGE(STATUS "OpenCAPI Buffer compiled with NUMA malloc")
ENDIF()



