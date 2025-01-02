#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace shared::domain::models {

struct ProcessConfig {
    // @Todo change "childAddress" to childEndpoint
    std::string internalId;
    std::string childAddress;
    std::uint32_t childPort;
    std::string parentAddress;
    std::uint32_t parentPort;

    [[nodiscard]] std::string GetChildAddress() const {
        return childAddress + ":" + std::to_string(childPort);
    }

    [[nodiscard]] std::string GetParentAddress() const {
        return parentAddress + ":" + std::to_string(parentPort);
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProcessConfig, internalId, childAddress, childPort, parentAddress, parentPort)

}