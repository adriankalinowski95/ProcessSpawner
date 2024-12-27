#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace shared::domain::models {

struct SendMessage {
    std::string internalId;
    std::int32_t uniqueNumber;
    std::string message;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SendMessage, internalId, uniqueNumber, message)

}