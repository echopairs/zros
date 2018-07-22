cmake_minimum_required(VERSION 2.8)

add_executable(test_service_discovery_impl service_discovery_demo.cc)
target_link_libraries(test_service_discovery_impl zros)

set_target_properties(test_service_discovery_impl
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${zros_SOURCE_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${zros_SOURCE_DIR}/bin/Release"
        RUNTIME_OUTPUT_DIRECTORY "${zros_SOURCE_DIR}/bin"
        )