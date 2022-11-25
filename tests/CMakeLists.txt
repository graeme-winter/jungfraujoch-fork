
add_executable(CatchTest
        DiffractionExperimentTest.cpp
        RawToConvertedGeometryTest.cpp
        ../common/RawToConvertedGeometry.h
        ../common/DiffractionExperiment.h
        FPGAUnitTest.h
        FPGAIntegrationTest.cpp
        SpotFinderIntegration.cpp
        SpotAnalyzeUnitTest.cpp
        ZSTDCompressorTest.cpp
        FrameTransformationTest.cpp
        HDF5WritingTest.cpp PedestalCalcTest.cpp
        ZMQImagePusherTest.cpp JFJochWriterTest.cpp
        CoordTest.cpp JFJochBrokerTest.cpp gRPCServerTest.cpp
        JFJochReceiverIntegrationTest.cpp
        AcquisitionCountersTest.cpp
        JFJochFullIntegrationTest.cpp
        XgandalfUnitTest.cpp
        MockAcquisitionDeviceTest.cpp ThreadSafeFIFOSetTest.cpp
        FPGANetworkTest.cpp
        ZMQPreviewPublisherTest.cpp
        HLSBitshuffleTest.cpp
        ActionConfigTest.cpp
        JFPedestalTest.cpp
        JFCalibrationTest.cpp
        RadialIntegrationTest.cpp
        StatusVectorTest.cpp
        CBORTest.cpp)

target_link_libraries(CatchTest JFJochBroker JFJochReceiver JFJochWriter DataProcessing CommonFunctions HLSSimulation -pthread -ldl -lz)
target_include_directories(CatchTest PRIVATE .)

add_executable(CompressionBenchmark CompressionBenchmark.cpp)
target_link_libraries(CompressionBenchmark JFJochWriter CommonFunctions -pthread -ldl -lz)

add_executable(HDF5DatasetWriteTest HDF5DatasetWriteTest.cpp)
target_link_libraries(HDF5DatasetWriteTest JFJochWriter CommonFunctions -pthread -ldl -lz)

ADD_EXECUTABLE(DataAnalysisPerfTest DataAnalysisPerfTest.cpp)
TARGET_LINK_LIBRARIES(DataAnalysisPerfTest DataProcessing JFJochWriter CommonFunctions)

ADD_EXECUTABLE(PedestalPerfTest PedestalPerfTest.cpp)
TARGET_LINK_LIBRARIES(PedestalPerfTest CommonFunctions)

ADD_EXECUTABLE(PreviewTest PreviewTest.cpp)
TARGET_LINK_LIBRARIES(PreviewTest JFJochWriter CommonFunctions -pthread -ldl -lz)

INSTALL(TARGETS CatchTest CompressionBenchmark DataAnalysisPerfTest PreviewTest RUNTIME)