#pragma once

#include <string>
#include <vector>
#include <process_manager/src/domain/models/ProcessHandles.h>

namespace process_manager::application::services {

class IPlatformProcessLauncher {
public:
    public:
    virtual ~IPlatformProcessLauncher() = default;
    virtual process_manager::domain::models::ProcessHandles launchProcess(const std::string& program, const std::vector<std::string>& args) = 0;
};

}