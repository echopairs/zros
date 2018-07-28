cmake_minimum_required(VERSION 2.8)

project(zros)
# may be support windows
if(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11")
  set(PROTOBUF_PROTOC_EXECUTABLE /usr/local/bin/protoc)
  set(GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)
  set(GRPC_LIBRARIES libgrpc++.a libgrpc.a)
  set(ZEROMQ_LIBRARIES zmq)
  link_libraries(tcmalloc.a profiler.a -lpthread -lz -lssl -lcrypto)
elseif(WIN32)
# TODO
endif(UNIX)

# -I
include_directories(
  ${zros_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# now just one, may lot
set(ZROS_GRPC_PROTO ${zros_SOURCE_DIR}/src/zros_proto/zros.proto)

find_Package(Protobuf REQUIRED)

# which need package to zros lib for other use
set (ZROS_FILES
	${zros_SOURCE_DIR}/include/zros/signal_handle.h
	${zros_SOURCE_DIR}/include/zros/singleton.h
	${zros_SOURCE_DIR}/include/zros/thread_pool.h
	${zros_SOURCE_DIR}/include/zros/service_discovery.h
	${zros_SOURCE_DIR}/include/zros/service_server_interface.h
	${zros_SOURCE_DIR}/include/zros/service_server.h
	${zros_SOURCE_DIR}/include/zros/publisher_interface.h
	${zros_SOURCE_DIR}/include/zros/publisher.h
	${zros_SOURCE_DIR}/include/zros/publisher_manager.h

	${zros_SOURCE_DIR}/include/zros/node_handle.h
	${zros_SOURCE_DIR}/src/zros_cpp/node_handle.cc

	${zros_SOURCE_DIR}/include/zros/zros.h
	${zros_SOURCE_DIR}/src/zros_cpp/zros.cc

	${zros_SOURCE_DIR}/src/zros_cpp/service_discovery.cc
	${zros_SOURCE_DIR}/src/zros_cpp/impl/service_discovery_impl.cc

	${zros_SOURCE_DIR}/include/zros/service_server_manager.h
	${zros_SOURCE_DIR}/src/zros_cpp/service_server_manager.cc
	${zros_SOURCE_DIR}/include/zros/service_client_manager.h
	${zros_SOURCE_DIR}/src/zros_cpp/service_client_manager.cc
	${zros_SOURCE_DIR}/src/zros_cpp/impl/service_servers_impl.h
	${zros_SOURCE_DIR}/src/zros_cpp/impl/service_servers_impl.cc
	${zros_SOURCE_DIR}/src/zros_cpp/impl/service_clients_impl.h
	${zros_SOURCE_DIR}/src/zros_cpp/impl/service_clients_impl.cc
	${zros_SOURCE_DIR}/src/zros_cpp/service_client_interface.cc
	${zros_SOURCE_DIR}/src/zros_cpp/publisher_interface.cc
	${zros_SOURCE_DIR}/src/zros_cpp/publisher_manager.cc
	${zros_SOURCE_DIR}/src/zros_cpp/impl/publisher_impl.h
	${zros_SOURCE_DIR}/src/zros_cpp/impl/publisher_impl.cc
)


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ZROS_GRPC_PROTO})

# grpc genrate cpp
get_filename_component(ABS_PROTO ${ZROS_GRPC_PROTO} ABSOLUTE)
get_filename_component(PROTOS_WE ${ZROS_GRPC_PROTO} NAME_WE)
list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc")
list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h")

add_custom_command (
	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h"
			"${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc"
	COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
	ARGS -I ${zros_SOURCE_DIR}/src/zros_proto
			--grpc_out=${CMAKE_CURRENT_BINARY_DIR}
			--plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
	${ABS_PROTO}
	DEPENDS ${NAME_PRO} ${GRPC_CPP_PLUGIN_EXECUTABLE} ${PROTOBUF_PROTOC_EXECUTABLE}
	COMMENT "Running C++ Protocol buffer"
)
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

# zros
add_library(zros ${ZROS_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(zros ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})


# install zros
if(UNIX)
  # default is /usr/local/lib
#  install(TARGETS zros DESTINATION lib)
  install(DIRECTORY ${zros_SOURCE_DIR}/include/zros DESTINATION include)
  install(FILES
    "${CMAKE_CURRENT_BIANARY_DIR}/zros_rpc.grpc.pb.h"
    "${CMAKE_CURRENT_BIANARY_DIR}/zros_rpc.pb.h"
    DESTINATION include/zros
  )
endif(UNIX)


# zros_master
set (ZROS_MASTER_SRC
    ${zros_SOURCE_DIR}/src/zros_master/main.cc
    ${zros_SOURCE_DIR}/src/zros_master/zros_master.h
	${zros_SOURCE_DIR}/src/zros_master/zros_master.cc
    ${zros_SOURCE_DIR}/src/zros_master/signal_handle.cc
    ${zros_SOURCE_DIR}/src/zros_master/node_manager.h
	${zros_SOURCE_DIR}/src/zros_master/node_manager.cc
    ${zros_SOURCE_DIR}/src/zros_master/service_manager.cc
    ${zros_SOURCE_DIR}/src/zros_master/service_manager.h
	${zros_SOURCE_DIR}/src/zros_master/topic_manager.cc
    ${zros_SOURCE_DIR}/src/zros_master/topic_manager.h
	${zros_SOURCE_DIR}/src/zros_master/connect_task.h
	${zros_SOURCE_DIR}/src/zros_master/connect_task.cc
  )

add_executable(zros_master ${ZROS_MASTER_SRC})
target_link_libraries(zros_master zros)

# install zros master
if(UNIX)
  install(TARGETS zros_master DESTINATION bin)
endif(UNIX)

add_subdirectory(src/demos)

# example
protobuf_generate_cpp(EXAMPLE_PROTO_SRCS EXAMPLE_PROTO_HDRS ${zros_SOURCE_DIR}/examples/cpp/test_message.proto ${zros_SOURCE_DIR}/examples/cpp/test_service.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(example_service_server ${zros_SOURCE_DIR}/examples/cpp/example_service_server.cc ${EXAMPLE_PROTO_SRCS} ${EXAMPLE_PROTO_HDRS})
target_link_libraries(example_service_server zros)


add_executable(example_service_client ${zros_SOURCE_DIR}/examples/cpp/example_service_client.cc ${EXAMPLE_PROTO_SRCS} ${EXAMPLE_PROTO_HDRS})
target_link_libraries(example_service_client zros)

add_executable(example_publisher ${zros_SOURCE_DIR}/examples/cpp/example_publisher.cc ${EXAMPLE_PROTO_SRCS} ${EXAMPLE_PROTO_HDRS})
target_link_libraries(example_publisher zros)
