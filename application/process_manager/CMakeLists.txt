set(TARGET_NAME "process_manager")

set(SOURCE_FILES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME}
   "shared"
)