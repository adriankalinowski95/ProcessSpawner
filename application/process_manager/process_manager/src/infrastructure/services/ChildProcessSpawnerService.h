#pragma once

#include <memory>
#include <optional>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>

#include <process_manager/src/domain/models/ProcessInstance.h>

#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>

#include <process_manager/src/application/tools/IProcessSpawner.h>
#include <process_manager/src/application/services/IChildProcessSpawnerService.h>
#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>

#include <process_manager/src/infrastructure/commands/ProcessManagerCommandFactory.h>

namespace process_manager::infrastructure::services {

class ChildProcessSpawnerService : public process_manager::application::services::IChildProcessSpawnerService {
public:
    ChildProcessSpawnerService(
        std::shared_ptr<process_manager::application::tools::IProcessSpawner> processSpawner,
        std::shared_ptr<process_manager::application::providers::ChildProcessConfigProvider> childProcessConfigProvider,
        std::shared_ptr<process_manager::infrastructure::commands::ProcessManagerCommandFactory> commandFactory,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_configProvider{ childProcessConfigProvider},
            m_commandFactory{ commandFactory },
            m_logger{ logger } 
    {
        if (!m_logger || !m_processSpawner || !m_configProvider) {
            throw std::runtime_error("Logger or process spawner or config provider is not initialized!");
        }
    }

    virtual ~ChildProcessSpawnerService() = default;

    std::optional<process_manager::domain::models::ProcessInstance> spawnChildProcess(const std::string& internalId, const std::map<std::string, std::string>& parameters) override {
        // spawn process
        const auto childConfig = m_configProvider->GetNextChildConfig(internalId);
        const auto childConfigJson = shared::application::utils::ModelsJsonConverter{}.toJson(childConfig);

        const auto pid = m_processSpawner->startProcess(environment::child_process::Process_Path.data(), { childConfigJson });
        if (!pid) {
            m_logger->log(
                shared::application::services::ILogger::LogLevel::Error, 
                "CHILD_PROCESS_SPAWNER_SERVICE",
                "Failed to spawn child process"
            );

            return std::nullopt;
        }

        // send init ping
        auto pingMessage = ::child_process_communication::ChildInitRequest{};
        pingMessage.set_internal_id(internalId.data());

        if (!m_commandFactory->createChildInitRequestCommand(childConfig)->sendRequest(pingMessage)) {
            m_logger->log(
                shared::application::services::ILogger::LogLevel::Error, 
                "CHILD_PROCESS_SPAWNER_SERVICE",
                "Failed to send init ping"
            );

            return std::nullopt;
        }

        return getProcessInstance(internalId, *pid, childConfig, parameters);
    }

private:
    std::shared_ptr<process_manager::application::tools::IProcessSpawner> m_processSpawner;
    std::shared_ptr<process_manager::application::providers::ChildProcessConfigProvider> m_configProvider;
    std::shared_ptr<process_manager::infrastructure::commands::ProcessManagerCommandFactory> m_commandFactory;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    
    process_manager::domain::models::ProcessInstance getProcessInstance(
        const std::string& internalId,
        std::uint32_t pid,
        const shared::domain::models::ProcessConfig& config,
        const std::map<std::string, std::string>& parameters) const {
        const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        return process_manager::domain::models::ProcessInstance {
            .internalId = internalId,
            .processType = "child",
            .parameters = parameters,
            .pid = pid,
            .address = config.childAddress,
            .port = config.childPort,
            .createdTimeMs = currentTime,
            .lastUpdateTimeMs = currentTime
        };
    }
};

}