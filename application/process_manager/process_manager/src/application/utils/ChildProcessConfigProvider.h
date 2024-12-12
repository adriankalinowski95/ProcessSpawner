#pragma once

#include <string>
#include <cstdint>
#include <shared/src/application/services/ModelsMapperService.h>

namespace process_manager::application::utils {

class ChildProcessConfigProvider {
public:
    ChildProcessConfigProvider( 
        std::string baseChildAddress, 
        std::uint32_t baseChildPort, 
        std::string parentAddress, 
        std::uint32_t parentPort) : 
            m_baseChildAddress{ baseChildAddress },
            m_baseChildPort{ baseChildPort },
            m_parentAddress{ parentAddress },
            m_parentPort{ parentPort },
            m_lastChildPort{ baseChildPort } {}

    [[nodiscard]] std::string GetNextChildConfigJson() {
        const auto nextChildConfig = GetNextChildConfig();

        return shared::application::services::ModelsConverter{}.toJson(nextChildConfig);    
    }

private:
    std::string m_baseChildAddress;
    std::uint32_t m_baseChildPort;
    std::string m_parentAddress;
    std::uint32_t m_parentPort;
    std::uint32_t m_lastChildPort;

    [[nodiscard]] shared::domain::models::ProcessConfig GetNextChildConfig() {
        return shared::domain::models::ProcessConfig{
            m_baseChildAddress.data(),
            m_lastChildPort++,
            m_parentAddress.data(),
            m_parentPort
        };
    }
};

}
