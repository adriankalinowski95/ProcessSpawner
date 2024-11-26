#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace shared::domain::models {

struct PingMessage {
    std::string internalId;
    std::int32_t uniqueNumber;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PingMessage, internalId, uniqueNumber)

}