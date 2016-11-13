cmake_minmum_required(VERSION 3.2)

project(nsky)

# may be support windows
if(UNIX)
  set(CMAKE_CPP_FLAGS "-std=c++11")
  set(PROTOBUF_PROTOC_EXECUTABLE /usr/local/bin/protoc)
  set(GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)
  set(GRPC_LIBRARIES libgrpc++.a libgrpc.a)
  // TODO
elseif(WIN32)
// TODO
endif(UNIX)

# -I
include_directories(
  $(nsky_SOURCE_DIR)/include
  $(nsky_SOURCE_DIR)/include/nsky/common/slog/spdlog/include
)

# now just one, may lot
set(NSKY_GRPC_PROTO ${nsky_SOURCE_CODE/src/nsky_proto/nsky_rpc.proto})

find_Package(Protobuf REQUIRED)


// which need package to nsky lib for other use
set(NSKY_HEAD
  // TODO
)
set(NSKY_SRC
  // TODO
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${NSKY_GRPC_PROTO})

# grpc genrate cpp
get_filename_componet(ABS_PROTO ${NSKY_GRPC_PROTO} ABSOLUTE)
get_filename_componet(PROTOS_WE ${NSKY_GRPC_PROTO} NAME_WE)
list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc")
list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h")
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.cc"
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTOS_WE}.grpc.pb.h"
  COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
  ARGS -I ${nsky_SOURCE_CODE_DIR/src/nsky_proto}
    --grpc_out=${CMAKE_DURRENT_BINANRY_DIR}
    --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
    ${ABS_PROTO}
    DEPENDS ${ABS_PROTO} ${PROTOBUF_PROTOC_EXECUTABLE} ${GRPC_CPP_PLUGIN_EXECUTABLE}
    COMMENT "Running C++ protocal buffer compiler with grpc plugin on ${PROTOS_WE}"
)

set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

# nsky
add_library(nsky ${NSKY_SRC} ${NSKY_HDRS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(nsky ${GRPC_LIBRARIES} ${PROTOBUF_LIBRARIES})


# install nsky
if (UNIX)
  # default is /usr/local/lib
  install(TARGETS nsky DESTINATION lib)
  install(DIRECTORY ${nsky_SOURCE_DIR}/include/nsky DESTINATION include)
  install(FILES
    "${CMAKE_CURRENT_BIANARY_DIR}/nsky_rpc.grpc.pb.h"
    "${CMAKE_CURRENT_BIANARY_DIR}/nsky_rpc.pb.h"
    DESTINATION include/nsky
  )
#endif(UNIX)


# nsky_master
set (NSKY_MATER_SRC
  // TODO
  )
add_library(nsky_master ${NSKY_MASTER_SRC})
target_link_libraries(nsky_master nsky)
add_executable(nsky_master ${nsky_SOURCE_DIR}/src/nsky_master/main.cpp)

# install nsky master
if(UNIX)
  install(TARGETS nsky_master DESTINATION bin)
endif(UNIX)
