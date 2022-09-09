IF(USE_OPENCAPI)
    ADD_EXECUTABLE(jfjoch_action_test jfjoch_action_test.cpp ../hls/hls_jfjoch.h)
    TARGET_LINK_LIBRARIES(jfjoch_action_test JungfrajochFPGA JFJochReceiver)
    INSTALL(TARGETS jfjoch_action_test RUNTIME)
ENDIF()

ADD_LIBRARY(JFJochReceiver STATIC
        JFJochReceiverService.cpp JFJochReceiverService.h
        JFJochReceiverTest.cpp JFJochReceiverTest.h
        JFJochReceiver.cpp JFJochReceiver.h)

TARGET_LINK_LIBRARIES(JFJochReceiver JungfraujochHost CommonFunctions HLSSimulation)

ADD_EXECUTABLE(jfjoch_receiver jfjoch_receiver.cpp)
TARGET_LINK_LIBRARIES(jfjoch_receiver JFJochReceiver)
INSTALL(TARGETS jfjoch_receiver RUNTIME)