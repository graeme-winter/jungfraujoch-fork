
add_executable(CatchTest
        DiffractionExperimentTest.cpp
        RawToConvertedGeometryTest.cpp
        ../common/RawToConvertedGeometry.h
        ../common/DiffractionExperiment.h
        FPGAUnitTest.h
        FPGAIntegrationTest.cpp
        SpotFinderIntegration.cpp
        DataStreamCalibrationTest.cpp
        SpotAnalyzeUnitTest.cpp
        CommunicationBufferTest.cpp
        ZSTDCompressorTest.cpp
        FrameTransformationTest.cpp
        HDF5WritingTest.cpp PedestalCalcTest.cpp
        ZMQImagePusherTest.cpp ImageWriterTest.cpp
        CoordTest.cpp JFJochBrokerTest.cpp gRPCServerTest.cpp
        JFJochReceiverIntegrationTest.cpp
        AcquisitionDeviceFilterTest.cpp AcquisitionDeviceCountersTest.cpp
        JFJochFullIntegrationTest.cpp
        XgandalfUnitTest.cpp
        MockAcquisitionDeviceTest.cpp ThreadSafeFIFOSetTest.cpp
        FPGANetworkTest.cpp
        ZMQPreviewPublisherTest.cpp BackgroundEstimationTest.cpp HLSBitshuffleTest.cpp FrameTransformationPreviewTest.cpp ActionConfigTest.cpp)

target_link_libraries(CatchTest JFJochBroker JFJochReceiver JFJochWriter DataProcessing CommonFunctions HLSSimulation -pthread -ldl -lz)

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

ADD_EXECUTABLE(ZMQTestPuller ZMQTestPuller.cpp)
TARGET_LINK_LIBRARIES(ZMQTestPuller JFJochWriter CommonFunctions)

ADD_EXECUTABLE(ZMQTestPusher ZMQTestPusher.cpp)
TARGET_LINK_LIBRARIES(ZMQTestPusher JFJochWriter CommonFunctions)

INSTALL(TARGETS CatchTest CompressionBenchmark DataAnalysisPerfTest PreviewTest ZMQTestPuller ZMQTestPusher RUNTIME)