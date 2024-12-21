#pragma once

#include <vector>
#include <string_view>
#include <process_manager/src/domain/models/ProcessInfo.h>

namespace process_manager::application::services {

class IProcessTerminator {
public:
    virtual ~IProcessTerminator() = default;
    virtual bool terminateAll(const std::vector<process_manager::domain::models::ProcessInfo>& processes) const = 0;
};

}