set(TARGET_NAME "child_process")

set(SOURCE_FILES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   "shared"
)