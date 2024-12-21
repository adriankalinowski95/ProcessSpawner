#pragma once

#include <string>

namespace process_manager::domain::models {

struct ProcessInfo {
    int pid;
    std::string name;
};

}