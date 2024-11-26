#pragma once

#include <vector>
#include <string>
#include <optional>
#include <process_manager/src/domain/models/InputConfig.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>

namespace process_manager::application::utils {

class ProcessManagerParamsParser {
public:
    static std::vector<std::string> GetParams(int argc, char** argv) {
        std::vector<std::string> params{};
        if (argc != 2 || argv == nullptr) {
            return params;
        }
    
        for (auto i = 0; i < argc; i++) {
            auto param = argv[i];
            if (param != nullptr) {
                params.push_back(param);
            }
        }

        return params;
    }

    static std::optional<process_manager::domain::models::InputConfig> ParseConfig(const std::vector<std::string>& params) {
        if (params.empty() || params.size() != 2) {
            return std::nullopt;
        }
        
        return shared::application::utils::ModelsJsonConverter{}.fromJson<process_manager::domain::models::InputConfig>(params[1]);
    }
};

}