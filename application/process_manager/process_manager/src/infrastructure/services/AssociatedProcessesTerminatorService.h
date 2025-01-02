#pragma once

#include <memory>
#include <ranges>
#include <algorithm>
#include <string_view>

#include <process_manager/src/application/services/IAssociatedProcessesTerminatorService.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/providers/GlobalConfigProvider.h>
#include <process_manager/src/application/tools/IProcessEnumerator.h>
#include <process_manager/src/application/tools/IProcessTerminator.h>
#include <shared/src/application/providers/ICurrentProcessInfoProvider.h>

namespace process_manager::infrastructure::services {

class AssociatedProcessesTerminatorService : public process_manager::application::services::IAssociatedProcessesTerminatorService {
public:
    AssociatedProcessesTerminatorService(
        std::shared_ptr<process_manager::application::tools::IProcessEnumerator> processEnumerator,
        std::shared_ptr<process_manager::application::tools::IProcessTerminator> processTerminator,
        std::shared_ptr<shared::application::providers::ICurrentProcessInfoProvider> currentProcessInfoProvider,
        std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> globalConfig,
        std::shared_ptr<shared::application::services::ILogger> logger) 
    {
        if (!processEnumerator || !processTerminator  || !currentProcessInfoProvider || !globalConfig ||!logger) {
            throw std::runtime_error("Process enumerator or process terminator or currentProcessInfoProvider or globalConfig or logger is not initialized!");
        }
    }

    void terminate() override {
        const auto& childProcessConfig = m_globalConfig->GetChildProcessConfig();
        const auto& processManagerConfig = m_globalConfig->GetProcessManagerConfig();
    
        // kill all child processes
        if (!m_processTerminator->terminateAll(m_processEnumerator->enumerateWhereNameEquals(childProcessConfig.processName))) {
            m_logger->logError("Failed to terminate child processes");

            return;
        }

        // @Todo - could be changed depends of strategy
        // kill all process manager processes
        auto processManagerProcesses = m_processEnumerator->enumerateWhereNameEquals(processManagerConfig.processName);
        if (processManagerProcesses.size() == 0) {
            return;
        }

        const auto currentPid = m_currentProcessInfoProvider->getPid();
        const auto end = std::ranges::remove_if(processManagerProcesses, [currentPid](process_manager::domain::models::ProcessInfo& processInfo) {
            return processInfo.pid == currentPid;
        });

        processManagerProcesses.erase(end.begin(), end.end());

        if (!m_processTerminator->terminateAll(processManagerProcesses)) {
            m_logger->logError("Failed to terminate process manager processes");
        }
    }

private:
    std::shared_ptr<process_manager::application::tools::IProcessEnumerator> m_processEnumerator;
    std::shared_ptr<process_manager::application::tools::IProcessTerminator> m_processTerminator;
    std::shared_ptr<shared::application::providers::ICurrentProcessInfoProvider> m_currentProcessInfoProvider;
    std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> m_globalConfig;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}