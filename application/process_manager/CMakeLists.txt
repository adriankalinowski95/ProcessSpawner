set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(TARGET_NAME "process_manager")

set(SOURCE_FILES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})