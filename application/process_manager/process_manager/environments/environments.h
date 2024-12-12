#pragma once

#include <string_view>

#define DEBUG 1

namespace environment {
    namespace defs {
        constexpr std::string_view Server_Url = "192.168.1.190:50051";
        constexpr std::string_view Backend_Url = "192.168.1.190:5002";
    }

    namespace child_process {
        constexpr std::string_view Process_Path = "/Users/adriankalinowski/Desktop/learning_projects/process_spawner/application/build/child_process/child_process";
    }
}