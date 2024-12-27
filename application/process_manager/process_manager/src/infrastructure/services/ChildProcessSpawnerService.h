#pragma once

#include <memory>
#include <optional>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>

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
        std::shared_ptr<process_manager::application::utils::ChildProcessConfigProvider> childProcessConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_configProvider{ childProcessConfigProvider},
            m_logger{ logger } 
        {
            if (!m_logger || !m_processSpawner || !m_configProvider) {
                throw std::runtime_error("Logger or process spawner or config provider is not initialized!");
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
        auto pingMessage = ::child_process_communication::ChildInitRequest{};
        pingMessage.set_internal_id(internalId.data());

        process_manager::infrastructre::commands::ChildInitRequestCommand::Sender::Config config {
            childConfig.childAddress,
            childConfig.childPort,
            3,
            1000,
            [pingMessage] (const ::child_process_communication::ChildInitResponse& output) -> bool {
                return output.success();
            }
        };

        if (!process_manager::infrastructre::commands::ChildInitRequestCommand{ config }.sendRequest(pingMessage)) {
            return std::nullopt;
        }

        return getProcessInstance(internalId, *pid, childConfig);
    }

private:
    std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> m_processSpawner;
    std::shared_ptr<process_manager::application::utils::ChildProcessConfigProvider> m_configProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    
    process_manager::domain::models::ProcessInstance getProcessInstance(
        const std::string& internalId,
        std::uint32_t pid,
        const shared::domain::models::ProcessConfig& config) {
        const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        return process_manager::domain::models::ProcessInstance {
            .internalId = internalId,
            .processType = "child",
            .parameters = {},
            .pid = pid,
            .address = config.childAddress,
            .port = config.childPort,
            .createdTimeMs = currentTime,
            .lastUpdateTimeMs = currentTime
        };
    }
};

}