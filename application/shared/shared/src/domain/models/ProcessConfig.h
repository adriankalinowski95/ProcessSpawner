#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace shared::domain::models {

struct ProcessConfig {
    std::string internalId;
    std::string childAddress;
    std::uint32_t childPort;
    std::string parentAddress;
    std::uint32_t parentPort;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProcessConfig, internalId, childAddress, childPort, parentAddress, parentPort)

}