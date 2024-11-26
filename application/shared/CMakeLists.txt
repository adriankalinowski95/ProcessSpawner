cmake_minimum_required(VERSION 3.8)

set(TARGET_NAME "shared")

set(absl_DIR ${PROTOBUF}/lib/cmake/absl)
set(utf8_range_DIR ${PROTOBUF}/lib/cmake/utf8_range)

find_package(absl REQUIRED CONFIG)
find_package(protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED CONFIG)
find_package(boost REQUIRED CONFIG)
find_package(nlohmann_json CONFIG REQUIRED)

function(printVeriables) 
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        string(TOLOWER "absl" MY_STRING_LOWER)
        string(TOLOWER "${_variableName}" SEARCH_SUBSTRING_LOWER)

        string(FIND "${SEARCH_SUBSTRING_LOWER}" "${MY_STRING_LOWER}" HAS_BOOST)
        #if (HAS_BOOST GREATER -1)
            message("${_variableName}=${${_variableName}}")
        #endif()
    endforeach()
endfunction()

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/src/domain/protos")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/domain/protos")

set(PROTO_FILES
    shared/src/domain/protos/common.proto
    shared/src/domain/protos/communication.proto
    shared/src/domain/protos/child_process_communication.proto
    shared/src/domain/protos/core_communication.proto
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

add_library(${TARGET_NAME} ${PROTO_FILES})

# For communication.pb.cc files - with this, they will be able to #include "src/domain/protos/communication.pb.h"
include_directories(${TARGET_NAME} 
    PUBLIC 
    . 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${protobuf_INCLUDE_DIR} 
    ${gRPC_INCLUDE_DIR}
)

# For extended project - with this, they will be able to #include <communication.pb.h> and #include "src/domain/protos/communication."
target_include_directories(${TARGET_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${PROTO_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${protobuf_INCLUDE_DIR} 
    ${gRPC_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        absl::log_internal_message
        absl::log_internal_check_op
        protobuf::libprotobuf
        protobuf::libprotoc
        gRPC::grpc
        gRPC::grpc++
        Boost::boost
        Boost::system
        nlohmann_json::nlohmann_json
)

function(print_included_files target)
    get_target_property(include_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    if(NOT include_dirs)
        message(WARNING "No include directories found for target ${target}")
        return()
    endif()

    message("Include directories for target ${target}: ${include_dirs}")

    foreach(dir ${include_dirs})
        if(EXISTS ${dir})
            message("Checking directory: ${dir}")
            file(GLOB_RECURSE files "${dir}/*.h")
            foreach(file ${files})
                message("Found file: ${file}")
            endforeach()
        else()
            message(WARNING "Directory does not exist: ${dir}")
        endif()
    endforeach()
endfunction()


# print_included_files(${TARGET_NAME})

protobuf_generate(
    TARGET ${TARGET_NAME} 
    LANGUAGE cpp
)

protobuf_generate(
    TARGET ${TARGET_NAME} 
    LANGUAGE grpc 
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)