cmake_minimum_required(VERSION 2.8)

project(test)

if(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11 -g")
  set(PROTOBUF_PROTOC_EXECUTABLE /usr/local/bin/protoc)
  set(GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)
  set(GRPC_LIBRARIES libgrpc++.a libgrpc.a)
endif(UNIX)

find_package(Protobuf REQUIRED)
set(TEST_GRPC_PROTO ../../nsky_proto/nsky_rpc.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${TEST_GRPC_PROTO})

# gen

get_filename_component(ABS_PRO ${TEST_GRPC_PROTO} ABSOLUTE)
get_filename_component(NAME_PRO ${TEST_GRPC_PROTO} NAME_WE)

add_custom_command (
	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${NAME_PRO}.grpc.pb.h"
			"${CMAKE_CURRENT_BINARY_DIR}/${NAME_PRO}.grpc.pb.cc"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS -I ${test_SOURCE_DIR}/proto
            --grpc_out=${test_SOURCE_DIR}
    	    --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
    DEPENDS ${NAME_PRO} ${GRPC_CPP_PLUGIN_EXECUTABLE} ${PROTOBUF_PROTOC_EXECUTABLE}
    COMMENT "Running C++ Protocol buffer"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)
add_executable(demo ${test_SOURCE_DIR}/test.cc ${PROTOS_SRCS} ${PROTO_HDRS})
target_link_libraries(demo ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES})



