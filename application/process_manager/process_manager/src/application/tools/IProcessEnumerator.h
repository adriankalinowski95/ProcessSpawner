#pragma once

#include <vector>
#include <process_manager/src/domain/models/ProcessInfo.h>

namespace process_manager::application::tools {

class IProcessEnumerator {
public:
    virtual ~IProcessEnumerator() = default;
    virtual std::vector<process_manager::domain::models::ProcessInfo> enumerateAll() const = 0;
    virtual std::vector<process_manager::domain::models::ProcessInfo> enumerateWhereNameEquals(std::string_view name) const = 0;
    virtual std::vector<process_manager::domain::models::ProcessInfo> enumerateWhereNameContains(std::string_view substring) const = 0;
};

}