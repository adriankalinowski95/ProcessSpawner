#pragma once

#include <string_view>

#define DEBUG 1
#define STATIC_SERVER_ADDRESS 0

namespace environment {
    #if STATIC_SERVER_ADDRESS == 1
    namespace core_server {
        constexpr std::string_view Address = "192.168.1.190";
        constexpr std::uint32_t Port = 5002;
    }
    
    namespace parent_process {
        constexpr std::string_view Address = "192.168.1.190";
        constexpr std::uint32_t Port = 50051;
        
        // The name of the process after launching the application
        constexpr std::string_view Process_Name = "process_manager";
        // The name for identification on the core side
        constexpr std::string_view Core_Side_Process_Manager_Name = "default_process_manager";
    }
    
    namespace child_process {
        constexpr std::string_view Process_Path = "/Users/adriankalinowski/Desktop/learning_projects/process_spawner/application/build/child_process/child_process";
        constexpr std::string_view Address = "192.168.1.190";
        constexpr std::uint32_t Port = 8081;
        // The name of the process after launching the application
        constexpr std::string_view Process_Name = "child_process";
    }
    #else
    namespace core_server {
        constexpr std::string_view Address = "localhost";
        constexpr std::uint32_t Port = 5002;
    }

    namespace parent_process {
        constexpr std::string_view Address = "localhost";
        constexpr std::uint32_t Port = 50051;
        // The name of the process after launching the application
        constexpr std::string_view Process_Name = "process_manager";
        // The name for identification on the core side
        constexpr std::string_view Core_Side_Process_Manager_Name = "default_process_manager";
    }

    namespace child_process {
        constexpr std::string_view Process_Path = "/Users/adriankalinowski/Desktop/learning_projects/process_spawner/application/build/child_process/child_process";
        constexpr std::string_view Address = "localhost";
        constexpr std::uint32_t Port = 8081;
        // The name of the process after launching the application
        constexpr std::string_view Process_Name = "child_process";
    }
    #endif
}