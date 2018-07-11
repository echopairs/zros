cmake_minimum_required(VERSION 2.8)

project(nsky)
# may be support windows
if(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11")
  set(PROTOBUF_PROTOC_EXECUTABLE /usr/local/bin/protoc)
  set(GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)
  set(GRPC_LIBRARIES libgrpc++.a libgrpc.a)
  link_libraries(tcmalloc.a profiler.a -lpthread -lz -lssl -lcrypto)
  # TODO
elseif(WIN32)
# TODO
endif(UNIX)

# -I
include_directories(
  ${nsky_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# now just one, may lot
set(NSKY_GRPC_PROTO ${nsky_SOURCE_DIR}/src/nsky_proto/nsky_rpc.proto)

find_Package(Protobuf REQUIRED)


# which need package to nsky lib for other use
set(NSKY_HEAD
 	${nsky_SOURCE_DIR}/include/nsky/signal_handle.h
	${nsky_SOURCE_DIR}/include/nsky/singleton.h
	${nsky_SOURCE_DIR}/include/nsky/common/thread_pool.h
)
set(NSKY_SRC
  ${nsky_SOURCE_DIR}/src/nsky_cpp/nsky.cc
  ${nsky_SOURCE_DIR}/src/nsky_master/signal_handle.cc
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${NSKY_GRPC_PROTO})

# grpc genrate cpp
get_filename_component(ABS_PROTO ${NSKY_GRPC_PROTO} ABSOLUTE)
get_filename_component(PROTOS_WE ${NSKY_GRPC_PROTO} NAME_WE)
list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc")
list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h")

add_custom_command (
	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h"
			"${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc"
	COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
	ARGS -I ${nsky_SOURCE_DIR}/src/nsky_proto
			--grpc_out=${CMAKE_CURRENT_BINARY_DIR}
			--plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
	${ABS_PROTO}
	DEPENDS ${NAME_PRO} ${GRPC_CPP_PLUGIN_EXECUTABLE} ${PROTOBUF_PROTOC_EXECUTABLE}
	COMMENT "Running C++ Protocol buffer"
)
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

# nsky
add_library(nsky ${NSKY_SRC} ${NSKY_HDRS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(nsky ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES})


# install nsky
if(UNIX)
  # default is /usr/local/lib
#  install(TARGETS nsky DESTINATION lib)
  install(DIRECTORY ${nsky_SOURCE_DIR}/include/nsky DESTINATION include)
  install(FILES
    "${CMAKE_CURRENT_BIANARY_DIR}/nsky_rpc.grpc.pb.h"
    "${CMAKE_CURRENT_BIANARY_DIR}/nsky_rpc.pb.h"
    DESTINATION include/nsky
  )
endif(UNIX)


# nsky_master
set (NSKY_MASTER_SRC
    ${nsky_SOURCE_DIR}/src/nsky_master/main.cc
    ${nsky_SOURCE_DIR}/src/nsky_master/nsky_master.cc
    ${nsky_SOURCE_DIR}/src/nsky_master/nsky_master.h
    ${nsky_SOURCE_DIR}/src/nsky_master/node_mgr.cc
    ${nsky_SOURCE_DIR}/src/nsky_master/node_mgr.h
    ${nsky_SOURCE_DIR}/src/nsky_master/service_mgr.cc
    ${nsky_SOURCE_DIR}/src/nsky_master/service_mgr.h
  )

add_executable(nsky_master ${NSKY_MASTER_SRC})
target_link_libraries(nsky_master nsky)

# install nsky master
if(UNIX)
  install(TARGETS nsky_master DESTINATION bin)
endif(UNIX)
