cmake_minimum_required(VERSION 3.8)

set(TARGET_NAME "shared")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/shared/cmakedeps_macros.cmake")
    message(STATUS "Uruchamianie Conan" ${CMAKE_BINARY_DIR} "   " ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} --build missing -of=${CMAKE_BINARY_DIR}/shared
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/shared
                    RESULT_VARIABLE result)
    if(result)
        message(FATAL_ERROR "Błąd podczas uruchamiania Conan. Kod błędu: ${result}")
    endif()
endif()

include(FindProtobuf)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/src/domain/protos")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/domain/protos")

set(PROTO_FILES
    shared/src/domain/protos/communication.proto
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

add_library(${TARGET_NAME} ${PROTO_FILES})

# For communication.pb.cc files - with this, they will be able to #include "src/domain/protos/communication.pb.h"
include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} .)

# For extended project - with this, they will be able to #include <communication.pb.h> and #include "src/domain/protos/communication."
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROTO_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
    PUBLIC
        protobuf::libprotobuf
        protobuf::libprotoc
        gRPC::grpc
        gRPC::grpc++
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