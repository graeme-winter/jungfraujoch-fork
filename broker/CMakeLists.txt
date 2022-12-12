ADD_SUBDIRECTORY(pistache)

ADD_LIBRARY(JFJochBroker STATIC
        JFJochStateMachine.cpp JFJochStateMachine.h
        JFJochServices.cpp JFJochServices.h
        JFJochBrokerHttp.cpp JFJochBrokerHttp.h
        JFJochBrokerHttp_TIFF.cpp JFJochBrokerHttp_DataProcessingSettings.cpp
        JFJochBrokerHttp_DetectorSettings.cpp JFJochBrokerHttp_Commands.cpp
        JFJochBrokerHttp_Status.cpp)

TARGET_LINK_LIBRARIES(JFJochBroker gRPCClients CommonFunctions pistache_static)

ADD_EXECUTABLE(jfjoch_broker_http jfjoch_broker_http.cpp)
TARGET_LINK_LIBRARIES(jfjoch_broker_http JFJochBroker)

INSTALL(TARGETS jfjoch_broker_http RUNTIME)


