#pragma once

#include <memory>
#include <string>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>

#include <process_manager/src/infrastructure/services/ApplicationSingleton.h>
#include <process_manager/src/application/services/IChildProcessHolderService.h>
#include <process_manager/src/application/services/IChildProcessSpawnerService.h>

namespace process_manager::infrastructre::commands {

class ProcessManagerInputRequestCommand {
    using Sender = shared::infrastructure::commands::GenericGrpcRequestSenderCommand<
        ::Communication::ProcessManagerInputRequest,
        ::Communication::ProcessManagerInputResponse,
        ::Communication::ProcessManagerInputService
    >;
    
public:
    ProcessManagerInputRequestCommand(
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

    [[nodiscard]] bool loadInputProcesses(const std::string& processManagerName) const {
        Communication::ProcessManagerInputRequest request{};
        request.set_managername(processManagerName);

        Sender sender{ Sender::Config{
            m_globalConfigProvider->GetCoreServerConfig().endpoint.ip,
            m_globalConfigProvider->GetCoreServerConfig().endpoint.port,
            1,
            1000,
        }, [](auto stub, auto context, auto request, auto response) {
            return stub->GetInput(context, *request, response);
        }};
        
        auto response = sender.sendRequest(request);
        if (!response || !response->success()) {
            m_logger->logError("Failed to send request to core server");

            return false;
        }

        for (const auto& process : response->processes()) {
            auto childProcessInstance = m_processSpawner->spawnChildProcess(process.internal_id());
            if (!childProcessInstance) {
                m_logger->logError("Failed to spawn process with internal id: " + process.internal_id());
                
                continue;
            }

            m_processHolderService->add(*childProcessInstance);
        }

        return response->success();
    }

private:
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_processHolderService;
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}