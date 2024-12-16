#pragma once

#include <map>
#include <string>
#include <cstdint>

namespace process_manager::domain::models {

struct ProcessInstance {
    // data from core
    std::string internalId;
    std::string processType;
    std::map<std::string, std::string> parameters;

    // dynamic data
    std::uint32_t pid;
    std::string address;
    std::uint32_t port;

    long long createdTimeMs;
    long long lastUpdateTimeMs;
};

}