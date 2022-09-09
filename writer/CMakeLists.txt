SET(HDF5_USE_STATIC_LIBRARIES ON)

FIND_PACKAGE(HDF5 1.10 REQUIRED)

ADD_EXECUTABLE(jfjoch_writer jfjoch_writer.cpp)

ADD_LIBRARY(JFJochWriter STATIC
        JFJochWriterService.cpp JFJochWriterService.h
        JFJochWriter.cpp        JFJochWriter.h
        HDF5DataFile.h    HDF5DataFile.cpp
        HDF5MasterFile.h  HDF5MasterFile.cpp
        HDF5Objects.cpp   HDF5Objects.h
        HDF5NXmx.cpp      HDF5NXmx.h
        HDF5Writer.cpp    HDF5Writer.h
        ../compression/bitshuffle/bshuf_h5filter.c ZMQHDF5FileStatusPublisher.cpp ZMQHDF5FileStatusPublisher.h)

TARGET_LINK_LIBRARIES(JFJochWriter CommonFunctions ${HDF5_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(JFJochWriter PUBLIC ${HDF5_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(jfjoch_writer JFJochWriter)

INSTALL(TARGETS jfjoch_writer RUNTIME)
