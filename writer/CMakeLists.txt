SET(HDF5_USE_STATIC_LIBRARIES ON)

FIND_PACKAGE(HDF5 1.10 REQUIRED)

ADD_LIBRARY(HDF5Wrappers STATIC HDF5Objects.cpp HDF5Objects.h HDF5DataFile.h    HDF5DataFile.cpp  ../compression/bitshuffle/bshuf_h5filter.c)
TARGET_LINK_LIBRARIES(HDF5Wrappers Compression ${HDF5_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(HDF5Wrappers PUBLIC ${HDF5_INCLUDE_DIRS})

ADD_EXECUTABLE(jfjoch_writer jfjoch_writer.cpp)

ADD_LIBRARY(JFJochWriter STATIC
        JFJochWriterService.cpp JFJochWriterService.h
        JFJochWriter.cpp        JFJochWriter.h
        HDF5MasterFile.h  HDF5MasterFile.cpp
        HDF5NXmx.cpp      HDF5NXmx.h
        HDF5Writer.cpp    HDF5Writer.h
        ZMQImagePuller.cpp ZMQImagePuller.h
        )

TARGET_LINK_LIBRARIES(JFJochWriter HDF5Wrappers CommonFunctions)
TARGET_LINK_LIBRARIES(jfjoch_writer JFJochWriter)

ADD_EXECUTABLE(jfjoch_writer_test jfjoch_writer_test.cpp)
TARGET_LINK_LIBRARIES(jfjoch_writer_test JFJochWriter gRPCClients CommonFunctions)

INSTALL(TARGETS jfjoch_writer jfjoch_writer_test RUNTIME)

ADD_EXECUTABLE(HDF5Sum HDF5Sum.cpp)
TARGET_LINK_LIBRARIES(HDF5Sum HDF5Wrappers)