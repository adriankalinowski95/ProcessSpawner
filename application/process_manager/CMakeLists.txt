set(TARGET_NAME "process_manager")

set(SOURCE_FILES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} .)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   "shared"
)

# Dodanie flag kompilatora dla GCC i Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${TARGET_NAME} PRIVATE -fexceptions)
endif()

# Dodanie flag kompilatora dla MSVC
if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /EHsc)
endif()