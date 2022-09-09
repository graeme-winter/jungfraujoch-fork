ADD_SUBDIRECTORY(xgandalf)

ADD_LIBRARY(DataProcessing STATIC
        XgandalfWrapper.h XgandalfWrapper.cpp
        JFJochIndexerService.cpp JFJochIndexerService.h
        JFJochIndexer.cpp JFJochIndexer.h
        CrystalLattice.cpp CrystalLattice.h)

TARGET_LINK_LIBRARIES(DataProcessing xgandalf CommonFunctions)

ADD_EXECUTABLE(jfjoch_xgandalf main.cpp)
TARGET_LINK_LIBRARIES(jfjoch_xgandalf DataProcessing)

INSTALL(TARGETS jfjoch_xgandalf RUNTIME)