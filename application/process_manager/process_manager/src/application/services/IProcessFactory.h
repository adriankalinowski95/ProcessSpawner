#pragma once

#include <memory>
#include <string>
#include <vector>

#include "IProcess.h"

namespace process_manager::application::services {

class IProcessFactory {
public:
    virtual ~IProcessFactory() = default;
    virtual std::unique_ptr<process_manager::application::services::IProcess> createProcess(const std::string& program, const std::vector<std::string>& args) const = 0;
};

}