ADD_SUBDIRECTORY(slsDetectorPackage)

ADD_EXECUTABLE(jfjoch_detector jfjoch_detector.cpp JFJochDetector.cpp JFJochDetector.h)

ADD_LIBRARY(LibSerial STATIC
        libserial/SerialPort.cpp
        libserial/SerialStream.cpp
        libserial/SerialStreamBuf.cpp)

TARGET_LINK_LIBRARIES(jfjoch_detector LibSerial CommonFunctions slsSupportShared slsDetectorShared)

INSTALL(TARGETS sls_detector_put sls_detector_get jfjoch_detector RUNTIME)
