#pragma once

#include <vector>
#include <string>
#include <optional>
#include <shared/src/domain/models/ProcessConfig.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>

namespace child_process::application::utils {

class ChildProcessParamsParser {
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

    static std::optional<shared::domain::models::ProcessConfig> ParseConfig(const std::vector<std::string>& params) {
        if (params.empty() || params.size() != 2) {
            return std::nullopt;
        }
        
        return shared::application::utils::ModelsJsonConverter{}.fromJson<shared::domain::models::ProcessConfig>(params[1]);
    }
};

}