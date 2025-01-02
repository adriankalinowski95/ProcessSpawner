#pragma once

#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>

#include <environments/environments.h>
#include <process_manager/src/infrastructure/services/ApplicationSingleton.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/services/IChildProcessHolderService.h>
#include <process_manager/src/application/services/IChildProcessSpawnerService.h>

namespace process_manager::infrastructre::commands {

class ProcessManagerInputRequestCommand {
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
        Communication::ProcessManagerInputResponse response{};

        request.set_managername(processManagerName);

        // @Todo use GenericGrpcRequestSenderCommand
        auto channel = grpc::CreateChannel(m_globalConfigProvider->GetCoreServerConfig().endpoint.GetAddress(), grpc::InsecureChannelCredentials());
        std::unique_ptr<Communication::ProcessManagerInputService::Stub> stub = Communication::ProcessManagerInputService::NewStub(channel);
        grpc::ClientContext context;
        grpc::Status status = stub->GetInput(&context, request, &response);

        if (!status.ok()) {
            return false;
        }

        for (const auto& process : response.processes()) {
            auto childProcessInstance = m_processSpawner->spawnChildProcess(process.internal_id());
            if (!childProcessInstance) {
                m_logger->logError("Failed to spawn process with internal id: " + process.internal_id());
                
                continue;
            }

            m_processHolderService->add(*childProcessInstance);
        }

        return response.success();
    }

private:
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_processHolderService;
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}