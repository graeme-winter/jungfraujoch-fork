ADD_LIBRARY(JungfraujochHost STATIC
        GenerateOpenCAPIDeviceName.h
        AcquisitionDevice.cpp AcquisitionDevice.h
        AcquisitionOnlineCounters.cpp AcquisitionOnlineCounters.h
        MockAcquisitionDevice.cpp MockAcquisitionDevice.h
        HLSSimulatedDevice.cpp HLSSimulatedDevice.h
        Completion.cpp Completion.h ActionConfig.h
        PCIExpressDevice.cpp PCIExpressDevice.h MakeAcquisitionDevice.cpp MakeAcquisitionDevice.h AcquisitionOfflineCounters.cpp AcquisitionOfflineCounters.h)

TARGET_LINK_LIBRARIES(JungfraujochHost CommonFunctions HLSSimulation)

TARGET_INCLUDE_DIRECTORIES(JungfraujochHost PUBLIC ../../include)

FIND_LIBRARY(NUMA_LIBRARY NAMES numa DOC "NUMA Library")
CHECK_INCLUDE_FILE(numaif.h HAS_NUMAIF)

ADD_EXECUTABLE(jfjoch_pcie_status jfjoch_pcie_status.cpp)
TARGET_LINK_LIBRARIES(jfjoch_pcie_status JungfraujochHost)
INSTALL(TARGETS jfjoch_pcie_status RUNTIME)

ADD_EXECUTABLE(jfjoch_pcie_fill_buffer jfjoch_pcie_fill_buffer.cpp)
TARGET_LINK_LIBRARIES(jfjoch_pcie_fill_buffer JungfraujochHost)
INSTALL(TARGETS jfjoch_pcie_fill_buffer RUNTIME)

ADD_EXECUTABLE(jfjoch_pcie_print_dev_buf jfjoch_pcie_print_dev_buf.cpp)
TARGET_LINK_LIBRARIES(jfjoch_pcie_print_dev_buf JungfraujochHost)
INSTALL(TARGETS jfjoch_pcie_print_dev_buf RUNTIME)

IF(HAS_NUMAIF AND NUMA_LIBRARY)
    TARGET_COMPILE_DEFINITIONS(JungfraujochHost PRIVATE -DJFJOCH_USE_NUMA)
    TARGET_LINK_LIBRARIES(JungfraujochHost ${NUMA_LIBRARY})
    MESSAGE(STATUS "OpenCAPI Buffer compiled with NUMA malloc")
ENDIF()

IF(USE_OPENCAPI)
    TARGET_COMPILE_DEFINITIONS(JungfraujochHost PUBLIC -DJFJOCH_USE_OPENCAPI)
    ADD_LIBRARY(JungfrajochFPGA STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/software/lib/osnap.c OpenCAPIDevice.cpp OpenCAPIDevice.h)
    TARGET_LINK_LIBRARIES(JungfraujochHost JungfrajochFPGA)
    TARGET_INCLUDE_DIRECTORIES(JungfrajochFPGA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/actions/include/ ${CMAKE_CURRENT_SOURCE_DIR}/../oc-accel/software/include/)

    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "ppc64le")
        FIND_LIBRARY(OCXL ocxl REQUIRED)
    else()
        ADD_CUSTOM_TARGET(ocse COMMAND make WORKING_DIRECTORY ${OCSE_ROOT})
        ADD_DEPENDENCIES(JungfrajochFPGA ocse)
        TARGET_INCLUDE_DIRECTORIES(JungfrajochFPGA PUBLIC ${OCSE_ROOT}/libocxl ${OCSE_ROOT}/common)
        SET(OCXL ${OCSE_ROOT}/libocxl/libocxl.so)
    endif()

    TARGET_LINK_LIBRARIES(JungfrajochFPGA CommonFunctions ${OCXL} -pthread)

    ADD_EXECUTABLE(jfjoch_oc_status jfjoch_oc_status.cpp)
    TARGET_LINK_LIBRARIES(jfjoch_oc_status JungfraujochHost)
    INSTALL(TARGETS jfjoch_oc_status RUNTIME)
ENDIF()
