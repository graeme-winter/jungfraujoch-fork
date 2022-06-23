ADD_LIBRARY(JFJochBroker STATIC
        JFJochBrokerService.cpp JFJochBrokerService.h
        JFJochStateMachine.cpp JFJochStateMachine.h
        JFJochServices.cpp JFJochServices.h)
TARGET_LINK_LIBRARIES(JFJochBroker gRPCClients CommonFunctions)

add_executable(jfjoch_broker jfjoch_broker.cpp)
target_link_libraries(jfjoch_broker JFJochBroker )

INSTALL(TARGETS jfjoch_broker RUNTIME)