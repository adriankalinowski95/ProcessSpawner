set(TARGET_NAME "process_manager")

set(SOURCE_FILES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   "shared"
)