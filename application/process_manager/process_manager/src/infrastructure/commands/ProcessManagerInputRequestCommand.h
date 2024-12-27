#pragma once

#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>

#include <environments/environments.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/services/ChildProcessSpawnerService.h>

namespace process_manager::infrastructre::commands {

class ProcessManagerInputRequestCommand {
public:
    ProcessManagerInputRequestCommand(
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> processHolderService,
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessSpawnerService> processSpawner,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processHolderService{ processHolderService },
            m_processSpawner{ processSpawner },
            m_logger{ logger } 
    {
        if (!m_logger || !m_processHolderService || !m_processSpawner) {
            throw std::runtime_error("Logger or process holder or process spawner is not initialized!");
        }
    }

    [[nodiscard]] bool loadInputProcesses(const std::string& processManagerName) const {
        Communication::ProcessManagerInputRequest request{};
        Communication::ProcessManagerInputResponse response{};

        request.set_managername(processManagerName);

        // @Todo change to Input process config and make a singleton
        auto channel = grpc::CreateChannel(environment::defs::Backend_Url.data(), grpc::InsecureChannelCredentials());
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

            m_processHolderService->addChildProcess(*childProcessInstance);
        }

        return response.success();
    }

private:
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_processHolderService;
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}