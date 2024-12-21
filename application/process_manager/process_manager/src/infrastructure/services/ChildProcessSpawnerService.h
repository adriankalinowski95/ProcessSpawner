#pragma once

#include <memory>
#include <optional>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/domain/models/ProcessInstance.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/commands/ChildInitRequestCommand.h>

#include <process_manager/src/application/services/IChildProcessSpawnerService.h>
#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>

namespace process_manager::infrastructure::services {

class ChildProcessSpawnerService : public process_manager::application::services::IChildProcessSpawnerService {
public:
    ChildProcessSpawnerService(
        std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> processSpawner,
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> childProcessHolderService,
        std::shared_ptr<process_manager::application::utils::ChildProcessConfigProvider> childProcessConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_childProcessHolderService{ childProcessHolderService },
            m_configProvider{ childProcessConfigProvider},
            m_logger{ logger } 
        {
            if (!m_logger || !m_processSpawner || !m_childProcessHolderService || !m_configProvider) {
                throw std::runtime_error("Logger or process spawner or child process holder service or config provider is not initialized!");
            }
        }

    virtual ~ChildProcessSpawnerService() = default;

    std::optional<process_manager::domain::models::ProcessInstance> spawnChildProcess(const std::string& internalId) {
        // spawn process
        const auto childConfig = m_configProvider->GetNextChildConfig(internalId);
        const auto childConfigJson = shared::application::utils::ModelsJsonConverter{}.toJson(childConfig);

        const auto pid = m_processSpawner->startProcess(environment::child_process::Process_Path.data(), { childConfigJson });
        if (!pid) {
            return std::nullopt;
        }

        // send init ping
        const auto pingMessage = shared::domain::models::PingMessage {
            .internalId = internalId,
            .uniqueNumber = shared::application::utils::RandomValueGenerator{}.generateRandomValue()
        };

        if (!process_manager::infrastructre::commands::ChildInitRequestCommand{ config }.sendRequest(pingMessage)) {
            return std::nullopt;
        }

        // update process holder
        const auto processInstance = getProcessInstance(internalId, *pid, childConfig);
        m_childProcessHolderService->addChildProcess(processInstance);

        return processInstance;
    }

private:
    std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> m_processSpawner;
    std::shared_ptr<ChildProcessHolderService> m_childProcessHolderService;
    std::shared_ptr<process_manager::application::utils::ChildProcessConfigProvider> m_configProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    process_manager::domain::models::ProcessInstance getProcessInstance(const std::string& internalId, std::uint32_t pid, const shared::domain::models::ProcessConfig& config) {
        return process_manager::domain::models::ProcessInstance {
            .internalId = internalId,
            .pid = pid,
            .createdTimeMs = std::chrono::system_clock::now().time_since_epoch().count(),
            .lastUpdateTimeMs = std::chrono::system_clock::now().time_since_epoch().count(),
            .config = config
        };
    }
};

}