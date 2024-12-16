#pragma once

#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/InitChildProcessService.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/domain/models/ProcessInstance.h>

#include <environments/environments.h>

namespace process_manager::infrastructure::services {

class ProcessManagerService : public Communication::ManagerService::Service {
public:
   ProcessManagerService(
        std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> processSpawner,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_processSpawner{ processSpawner },
            m_configProvider{ 
                environment::child_process::Address.data(), environment::child_process::Port,
                environment::parent_process::Address.data(), environment::parent_process::Port
            },
            m_logger{ logger },
            m_childProcessHolderService{}
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        const auto childConfig = m_configProvider.GetNextChildConfig();
        const auto childConfigJson = shared::application::utils::ModelsJsonConverter{}.toJson(childConfig);
        const auto pid = m_processSpawner->startProcess(environment::child_process::Process_Path.data(), { childConfigJson });
        if (!pid) {
            const std::string message = "Failed to start process!";
            m_logger->logError(message);

            response->set_success(false);
            response->set_message(message);

            return grpc::Status::CANCELLED;
        }
        
        const auto processInstance = process_manager::infrastructure::services::InitChildProcessService{
            *pid,
            request->internal_id(),
            childConfig,
            m_logger
        }.init();

        if (!processInstance) {
            const std::string message = "Failed to initialize process!";
            m_logger->logError(message);

            response->set_success(false);
            response->set_message(message);

            return grpc::Status::CANCELLED;
        }

        m_childProcessHolderService.addChildProcess(*processInstance);
        response->set_internal_id(request->internal_id());
        response->set_process_id((int)processInstance->pid);
        response->set_success(true);
        response->set_message("OK");

        return grpc::Status::OK;
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }
    
private:
    std::shared_ptr<process_manager::infrastructure::tools::ProcessSpawner> m_processSpawner;
    process_manager::application::utils::ChildProcessConfigProvider m_configProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    
    ChildProcessHolderService m_childProcessHolderService;
};

}