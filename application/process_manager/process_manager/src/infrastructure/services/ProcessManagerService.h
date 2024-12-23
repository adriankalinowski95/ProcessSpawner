#pragma once

#include <chrono>
#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <environments/environments.h>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

#include <process_manager/src/domain/models/ProcessInstance.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/commands/ChildInitRequestCommand.h>
#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>

#include <process_manager/src/application/services/IChildProcessSpawnerService.h>

// @Todo move to api...
namespace process_manager::infrastructure::services {

class ProcessManagerService : public Communication::SpawnProcessService::Service {
public:
   ProcessManagerService(
        std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> processSpawner,
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> processHolderService,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_processHolderService{ processHolderService },
            m_logger{ logger }
    {
        if (!m_logger || !m_processSpawner || !m_processHolderService) {
            throw std::runtime_error("Logger or process spawner or child process holder service is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        auto childProcessInstance = m_processSpawner->spawnChildProcess(request->internal_id());
        if (!childProcessInstance) {
            return failedSpawnResponse("Failed to spawn process!", response);
        }

        m_processHolderService->addChildProcess(*childProcessInstance);

        return successSpawnResponse(*childProcessInstance, response);
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }
    
private:
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_processHolderService;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    grpc::Status failedSpawnResponse(const std::string& message, Communication::SpawnResponse* response) {
        m_logger->logError(message);

        response->set_success(false);
        response->set_message(message);

        return grpc::Status::CANCELLED;
    }

    grpc::Status successSpawnResponse(const process_manager::domain::models::ProcessInstance& processInstance, Communication::SpawnResponse* response) {
        response->mutable_process()->set_process_id(processInstance.pid);
        response->mutable_process()->set_internal_id(processInstance.internalId);
        response->mutable_process()->set_created_time_ms(processInstance.createdTimeMs);
        response->mutable_process()->set_last_update_time_ms(processInstance.lastUpdateTimeMs);
        response->set_success(true);
        response->set_message("OK");

        return grpc::Status::OK;
    }
};

}