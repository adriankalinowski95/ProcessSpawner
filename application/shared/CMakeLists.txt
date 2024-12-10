cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME "shared")

# conan_basic_setup()

# its overwritting some 
# include(FindProtobuf)
include(FindBoost)

find_package(absl REQUIRED)
find_package(Protobuf CONFIG REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)
find_package(gRPC CONFIG REQUIRED)
# find_package(Boost 1.86.0 REQUIRED COMPONENTS system)

#if(Protobuf_FOUND)
#    message(STATUS "protobuf found.")
#else()
#    message(FATAL_ERROR "Boost nie został znaleziony.")
#endif()

# FOR DEBUGGING
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     string(TOLOWER "boost" MY_STRING_LOWER)
#     string(TOLOWER "${_variableName}" SEARCH_SUBSTRING_LOWER)
# 
#     string(FIND "${SEARCH_SUBSTRING_LOWER}" "${MY_STRING_LOWER}" HAS_BOOST)
#     if (HAS_BOOST GREATER -1)
#         message("${_variableName}=${${_variableName}}")
#     endif()
# endforeach()

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/src/domain/protos")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/domain/protos")

set(PROTO_FILES
    shared/src/domain/protos/communication.proto
)

set(protobuf_ABSL_USED_TARGETS
    absl::log_internal_message
    absl::log_internal_check_op
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

add_library(${TARGET_NAME} ${PROTO_FILES})

# For communication.pb.cc files - with this, they will be able to #include "src/domain/protos/communication.pb.h"
include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR} . ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR})

# For extended project - with this, they will be able to #include <communication.pb.h> and #include "src/domain/protos/communication."
target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR} ${PROTO_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}  ${Boost_INCLUDE_DIR})

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${protobuf_ABSL_USED_TARGETS}
        protobuf::libprotobuf
        protobuf::libprotoc
        gRPC::grpc
        gRPC::grpc++
        Boost::boost
        Boost::system
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
            file(GLOB_RECURSE files "${dir}/*.hpp")
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
    PLUGIN "protoc-gen-grpc=${protobuf_PACKAGE_FOLDER_DEBUG}/bin/protoc"
)

protobuf_generate(
    TARGET ${TARGET_NAME} 
    LANGUAGE grpc 
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
    # PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
    PLUGIN "protoc-gen-grpc=${grpc_PACKAGE_FOLDER_DEBUG}/bin/grpc_cpp_plugin"
)