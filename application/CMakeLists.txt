cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE debug)

project("application")

add_subdirectory("child_process")
add_subdirectory("process_manager")
add_subdirectory("shared")