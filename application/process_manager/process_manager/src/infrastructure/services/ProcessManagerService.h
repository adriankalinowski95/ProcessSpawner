#pragma once

#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>
#include <shared/src/domain/models/PingMessage.h>

#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>

#include <process_manager/src/infrastructure/commands/InitChildProcessCommand.h>

#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/domain/models/ProcessInstance.h>

#include <environments/environments.h>

namespace process_manager::infrastructure::services {

class ProcessManagerService : public Communication::ManagerService::Service {
    using InitRequestSenderCommand = shared::infrastructure::commands::GenericRequestSenderCommand<shared::domain::models::PingMessage, shared::domain::models::PingMessage>;
public:
   ProcessManagerService(
        std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> processSpawner,
        std::shared_ptr<ChildProcessHolderService> childProcessHolderService,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_childProcessHolderService{ childProcessHolderService },
            m_configProvider{ 
                environment::child_process::Address.data(), environment::child_process::Port,
                environment::parent_process::Address.data(), environment::parent_process::Port
            },
            m_logger{ logger }
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        // spawn process
        const auto childConfig = m_configProvider.GetNextChildConfig(request->internal_id());
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

        InitRequestSenderCommand::Config config {
            childConfig.childAddress,
            childConfig.childPort,
            "/init",
            3,
            1000,
            [pingMessage] (const shared::domain::models::PingMessage& output) -> bool {
                return output.uniqueNumber == pingMessage.uniqueNumber + 1;
            }
        };

        if (!InitRequestSenderCommand{ config }.sendRequest(pingMessage)) {
            return failedSpawnResponse("Failed to initialize process!", response);
        }

        // update process holder
        m_childProcessHolderService->addChildProcess(getProcessInstance(request->internal_id(), *pid, childConfig));

        return successSpawnResponse(request->internal_id(), *pid, response);
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }
    
private:
    std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> m_processSpawner;
    std::shared_ptr<ChildProcessHolderService> m_childProcessHolderService;
    process_manager::application::utils::ChildProcessConfigProvider m_configProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    process_manager::domain::models::ProcessInstance getProcessInstance(
        const std::string& internalId,
        std::uint32_t pid,
        const shared::domain::models::ProcessConfig& config) {
        return process_manager::domain::models::ProcessInstance {
            .internalId = internalId,
            .processType = "child",
            .parameters = {},
            .pid = pid,
            .address = config.childAddress,
            .port = config.childPort
        };
    }

    grpc::Status failedSpawnResponse(const std::string& message, Communication::SpawnResponse* response) {
        m_logger->logError(message);

        response->set_success(false);
        response->set_message(message);

        return grpc::Status::CANCELLED;
    }

    grpc::Status successSpawnResponse(const std::string& internalId, int pid, Communication::SpawnResponse* response) {
        response->set_internal_id(internalId);
        response->set_process_id(pid);
        response->set_success(true);
        response->set_message("OK");

        return grpc::Status::OK;
    }
};

}