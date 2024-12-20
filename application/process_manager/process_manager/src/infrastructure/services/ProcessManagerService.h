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

// @Todo move to api...
namespace process_manager::infrastructure::services {

class ProcessManagerService : public Communication::SpawnProcessService::Service {
public:
   ProcessManagerService(
        std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> processSpawner,
        std::shared_ptr<ChildProcessHolderService> childProcessHolderService,
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

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        // @Todo push to separated class
        // spawn process
        const auto childConfig = m_configProvider->GetNextChildConfig(request->internal_id());
        const auto childConfigJson = shared::application::utils::ModelsJsonConverter{}.toJson(childConfig);

        const auto pid = m_processSpawner->startProcess(environment::child_process::Process_Path.data(), { childConfigJson });
        if (!pid) {
            return failedSpawnResponse("Failed to start process!", response);
        }

        // @Todo move this to separated class / method
        // send init ping
        const auto pingMessage = shared::domain::models::PingMessage {
            .internalId = request->internal_id(),
            .uniqueNumber = shared::application::utils::RandomValueGenerator{}.generateRandomValue()
        };

        process_manager::infrastructre::commands::ChildInitRequestCommand::Sender::Config config {
            childConfig.childAddress,
            childConfig.childPort,
            "/init",
            3,
            1000,
            [pingMessage] (const shared::domain::models::PingMessage& output) -> bool {
                return output.uniqueNumber == pingMessage.uniqueNumber + 1;
            }
        };

        if (!process_manager::infrastructre::commands::ChildInitRequestCommand{ config }.sendRequest(pingMessage)) {
            return failedSpawnResponse("Failed to initialize process!", response);
        }

        // update process holder
        const auto processInstance = getProcessInstance(request->internal_id(), *pid, childConfig);
        m_childProcessHolderService->addChildProcess(processInstance);

        return successSpawnResponse(processInstance, response);
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }
    
private:
    std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> m_processSpawner;
    std::shared_ptr<ChildProcessHolderService> m_childProcessHolderService;
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