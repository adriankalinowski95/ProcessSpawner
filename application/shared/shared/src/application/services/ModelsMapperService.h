#pragma once

#include <optional>
#include <string>
#include <shared/src/domain/models/ProcessConfig.h>

namespace shared::application::services {

class ModelsConverter {
public:
    std::optional<T> fromJson(const std::string& json) {
        try {
            return nlohmann::json::parse(json).get<T>();
        } catch (std::exception& e) {
            return std::nullopt;
        }
    }

    std::string toJson(const T& model) {
        return nlohmann::json(model).dump();
    }
};

}