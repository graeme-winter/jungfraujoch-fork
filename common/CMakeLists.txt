# git header
# the commit's SHA1, and whether the building workspace was dirty or not

FIND_PACKAGE(Git)

EXECUTE_PROCESS(COMMAND
        "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=8
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA1
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
EXECUTE_PROCESS(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

MESSAGE(STATUS "Git SHA1: ${GIT_SHA1}")
MESSAGE(STATUS "Git date: ${GIT_DATE}")

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/GitInfo.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/GitInfo.cpp" @ONLY)

ADD_LIBRARY( CommonFunctions STATIC
        Logger.cpp Logger.h
        Coord.cpp Coord.h
        DiffractionExperiment.cpp DiffractionExperiment.h
        RawToConvertedGeometry.h
        JFJochException.cpp JFJochException.h
        CommunicationBuffer.cpp CommunicationBuffer.h
        Definitions.h
        ${CMAKE_CURRENT_BINARY_DIR}/GitInfo.cpp GitInfo.h
        JFJochCompressor.cpp JFJochCompressor.h
        FrameTransformation.cpp FrameTransformation.h
        PedestalCalc.cpp PedestalCalc.h
        ZMQWrappers.cpp ZMQWrappers.h
        ThreadSafeFIFO.h
        ZMQPreviewPublisher.cpp ZMQPreviewPublisher.h
        ZMQImagePuller.h ZMQImagePuller.cpp
        ZMQImagePusher.cpp ZMQImagePusher.h
        RadialIntegration.cpp RadialIntegration.h
        DiffractionSpot.cpp DiffractionSpot.h
        StrongPixelSet.cpp StrongPixelSet.h
        ZMQSpotPublisher.cpp ZMQSpotPublisher.h
        PedestalCalcCPU.cpp PedestalCalcCPU.h
        ImageMetadata.h
        Latch.cpp Latch.h
        JFPedestal.cpp JFPedestal.h
        JFCalibration.cpp JFCalibration.h RadialIntegrationMapping.cpp RadialIntegrationMapping.h StatusVector.h)

IF (CMAKE_CUDA_COMPILER)
    TARGET_SOURCES(CommonFunctions PUBLIC SpotFinder.h PedestalCalcGPU.h)
    TARGET_SOURCES(CommonFunctions PRIVATE SpotFinder.cu PedestalCalcGPU.cu)

    TARGET_COMPILE_DEFINITIONS(CommonFunctions PUBLIC -DCUDA_SPOT_FINDING)
    FIND_LIBRARY(CUDART_LIBRARY cudart_static PATHS ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES} REQUIRED)
    TARGET_LINK_LIBRARIES(CommonFunctions ${CUDART_LIBRARY} -lrt)
ELSE()
    MESSAGE(STATUS "CUDA not found")
ENDIF()

# Need to do it to include Protocol Buffer headers - although this is not the cleanest solution
TARGET_LINK_LIBRARIES(CommonFunctions Compression libzmq JFJochProtoBuf)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    TARGET_LINK_LIBRARIES(CommonFunctions -lrt)
ENDIF()

IF(HAS_NUMAIF AND NUMA_LIBRARY)
    TARGET_COMPILE_DEFINITIONS(CommonFunctions PRIVATE -DJFJOCH_USE_NUMA)
    TARGET_LINK_LIBRARIES(CommonFunctions ${NUMA_LIBRARY})
ENDIF()

SET_SOURCE_FILES_PROPERTIES(PedestalCalc.cpp PROPERTIES COMPILE_FLAGS -Ofast)
