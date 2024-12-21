#pragma once

#include <string>
#include <optional>
#include <process_manager/src/domain/models/ProcessInstance.h>

namespace process_manager::application::services {

class IChildProcessSpawnerService {
public:
    virtual ~IChildProcessSpawnerService() = default;

    virtual std::optional<process_manager::domain::models::ProcessInstance> spawnChildProcess(const std::string& internalId) = 0;
};

}