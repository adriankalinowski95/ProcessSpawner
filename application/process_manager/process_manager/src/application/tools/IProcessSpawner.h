#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>

namespace process_manager::application::tools {

class IProcessSpawner {
public:
    [[nodiscard]] virtual std::optional<std::uint32_t> startProcess(const std::string& program, const std::vector<std::string>& args) = 0; 
};

}