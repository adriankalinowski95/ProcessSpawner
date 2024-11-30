cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

project("application")

add_subdirectory("child_process")
add_subdirectory("process_manager")
add_subdirectory("shared")