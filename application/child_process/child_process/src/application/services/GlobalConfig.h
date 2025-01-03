#pragma once

#include <optional>
#include <shared/src/domain/models/ProcessConfig.h>

namespace child_process::application::services {

class GlobalConfig {
public:
    GlobalConfig() : 
        m_processConfig{ std::nullopt } {}
    
    [[nodiscard]] static GlobalConfig& getInstance() {
        static GlobalConfig instance{};

        return instance;
    }

    [[nodiscard]] std::optional<shared::domain::models::ProcessConfig> getProcessConfig() const {
        return m_processConfig;
    }

    void setProcessConfig(const shared::domain::models::ProcessConfig& processConfig) {
        m_processConfig.emplace(processConfig);
    }

private:
    // make not possible to modified after set.
    std::optional<shared::domain::models::ProcessConfig> m_processConfig;
};

}