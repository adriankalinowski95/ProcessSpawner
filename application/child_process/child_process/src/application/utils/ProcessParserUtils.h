#pragma once

#include <vector>
#include <string>

namespace child_process::application::utils {

class ProcessParserUtils {
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
};

}