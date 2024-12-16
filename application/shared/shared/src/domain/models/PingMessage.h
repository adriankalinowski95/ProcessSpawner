#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace shared::domain::models {

struct PingMessage {
    std::uint32_t processId;
    std::int32_t uniqueNumber;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PingMessage, processId, uniqueNumber)

}