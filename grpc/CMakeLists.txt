SET(USE_SYSTEM_GRPC ON CACHE BOOL "Use system installed GRPC if available")
SET(ABSL_PROPAGATE_CXX_STD ON)
SET(protobuf_MODULE_COMPATIBLE TRUE)

IF(USE_SYSTEM_GRPC)
    FIND_PACKAGE(Protobuf CONFIG)
    FIND_PACKAGE(gRPC CONFIG)
ENDIF()

IF (gRPC_FOUND)
    MESSAGE(STATUS "Using preinstalled protobuf ${Protobuf_VERSION}")
    MESSAGE(STATUS "Using preinstalled gRPC ${gRPC_VERSION}")
    SET(_GRPC_GRPCPP gRPC::grpc++)
    SET(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
    SET(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
    SET(_GRPC_PYTHON_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_python_plugin>)
ELSE()
    INCLUDE(FetchContent)

    FetchContent_Declare(
            gRPC
            GIT_REPOSITORY https://github.com/grpc/grpc
            GIT_TAG        v1.46.5
    )
    SET(FETCHCONTENT_QUIET OFF)
    SET(gRPC_ZLIB_PROVIDER "package")
    SET(gRPC_INSTALL OFF)
    FetchContent_MakeAvailable(gRPC)
    SET(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
    SET(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
    SET(_GRPC_PYTHON_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_python_plugin>)
    SET(_GRPC_GRPCPP grpc++)
ENDIF()

SET(rg_proto "jfjoch.proto")
SET(rg_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/jfjoch.pb.cc")
SET(rg_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/jfjoch.pb.h")
SET(rg_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/jfjoch.grpc.pb.cc")
SET(rg_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/jfjoch.grpc.pb.h" )

ADD_CUSTOM_COMMAND(
        OUTPUT "${rg_proto_srcs}" "${rg_proto_hdrs}" "${rg_grpc_srcs}" "${rg_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_SOURCE_DIR}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${rg_proto}"
        DEPENDS "${rg_proto}")

ADD_LIBRARY(JFJochProtoBuf ${rg_grpc_srcs} ${rg_grpc_hdrs} ${rg_proto_srcs} ${rg_proto_hdrs})

TARGET_INCLUDE_DIRECTORIES(JFJochProtoBuf PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
TARGET_LINK_LIBRARIES(JFJochProtoBuf ${_GRPC_GRPCPP})

ADD_LIBRARY(gRPCClients STATIC
        JFJochReceiverClient.cpp JFJochReceiverClient.h
        JFJochWriterClient.cpp JFJochWriterClient.h
        JFJochDetectorClient.cpp JFJochDetectorClient.h
        JFJochIndexerClient.cpp JFJochIndexerClient.h
        JFJochWriterGroupClient.cpp JFJochWriterGroupClient.h)

TARGET_LINK_LIBRARIES(gRPCClients CommonFunctions JFJochProtoBuf)