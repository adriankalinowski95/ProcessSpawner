#pragma once

#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>
#include <process_manager/src/application/services/IChildProcessHolderService.h>
#include <process_manager/src/application/services/IChildProcessSpawnerService.h>

#include <process_manager/src/infrastructure/commands/ChildInitRequestCommand.h>
#include <process_manager/src/infrastructure/commands/ProcessManagerInputRequestCommand.h>

namespace process_manager::infrastructure::commands {

class ProcessManagerCommandFactory {
    using ChildInitRequestCommand = process_manager::infrastructre::commands::ChildInitRequestCommand;
    using ProcessManagerInputRequestCommand = process_manager::infrastructre::commands::ProcessManagerInputRequestCommand;
public:
    ProcessManagerCommandFactory(
        std::shared_ptr<process_manager::application::services::IChildProcessHolderService> processHolderService,
        std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> processSpawner,
        std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> globalConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_processHolderService{ processHolderService },
            m_processSpawner{ processSpawner },
            m_globalConfigProvider{ globalConfigProvider },
            m_logger{ logger } 
    {
        if (!m_logger || !m_processHolderService || !m_globalConfigProvider || !m_processSpawner) {
            throw std::runtime_error("Logger or processHolderService or globalConfigProvider or processSpawner is not initialized!");
        }
    }

    [[nodiscard]] std::shared_ptr<ProcessManagerInputRequestCommand> createProcessManagerInputRequestCommand() const {
        ProcessManagerInputRequestCommand::Sender::Config config {
            m_globalConfigProvider->GetCoreServerConfig().endpoint.ip,
            m_globalConfigProvider->GetCoreServerConfig().endpoint.port,
            1,
            1000,
        };

        return std::make_shared<ProcessManagerInputRequestCommand>(config, m_processHolderService, m_processSpawner, m_logger);
    }

    [[nodiscard]] std::shared_ptr<ChildInitRequestCommand> createChildInitRequestCommand(shared::domain::models::ProcessConfig childProcessConfig) const {
        ChildInitRequestCommand::Sender::Config config {
            childProcessConfig.childAddress,
            childProcessConfig.childPort,
            3,
            1000,
            [] (const ::child_process_communication::ChildInitResponse& output) -> bool {
                return output.success();
            }
        };

        return std::make_shared<ChildInitRequestCommand>(config);
    }

private:
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_processHolderService;
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}