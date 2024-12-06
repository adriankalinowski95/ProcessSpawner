#pragma once

#include <string.h>

namespace process_manager::domain::models {

struct ProcessDetails {
    std::string id;
    std::string name;
    std::string type;
}

}