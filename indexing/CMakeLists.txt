ADD_SUBDIRECTORY(xgandalf)

ADD_LIBRARY(DataProcessing STATIC
        XgandalfWrapper.h XgandalfWrapper.cpp
        JFJochIndexerService.cpp JFJochIndexerService.h
        JFJochIndexer.cpp JFJochIndexer.h
        CrystalLattice.cpp CrystalLattice.h
        IndexerWrapper.cpp IndexerWrapper.h)

IF (CMAKE_CUDA_COMPILER)
    FIND_PACKAGE(Eigen3 3.3 REQUIRED NO_MODULE)
    ADD_LIBRARY(FastFeedbackIndexer STATIC
            fast-feedback-indexer/indexer/src/indexer.cpp
            fast-feedback-indexer/indexer/src/indexer.h
            fast-feedback-indexer/indexer/src/indexer_gpu.cu
            fast-feedback-indexer/indexer/src/indexer_gpu.h
            fast-feedback-indexer/indexer/src/log.cpp
            fast-feedback-indexer/indexer/src/log.h
            fast-feedback-indexer/indexer/src/exception.h)
    TARGET_SOURCES(DataProcessing PUBLIC FastFeedbackIndexerWrapper.h)
    TARGET_SOURCES(DataProcessing PRIVATE FastFeedbackIndexerWrapper.cpp)
    TARGET_LINK_LIBRARIES(DataProcessing FastFeedbackIndexer Eigen3::Eigen)
endif()

TARGET_LINK_LIBRARIES(DataProcessing xgandalf CommonFunctions)

ADD_EXECUTABLE(jfjoch_xgandalf main.cpp)
TARGET_LINK_LIBRARIES(jfjoch_xgandalf DataProcessing)

INSTALL(TARGETS jfjoch_xgandalf RUNTIME)