#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <future>
#include <string_view>

#include <environments/environments.h>

#include <shared/src/api/controllers/BaseGrpController.h>
#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>

#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/domain/models/ProcessInstance.h>
#include <process_manager/src/application/tools/IProcessTerminator.h>
#include <process_manager/src/application/services/IChildProcessHolderService.h>
#include <process_manager/src/application/services/IChildProcessSpawnerService.h>

namespace process_manager::api::controllers {

class ProcessManagerController : public Communication::SpawnProcessService::Service, public shared::api::controllers::BaseGrpController {
public:
   ProcessManagerController(
        std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> processSpawner,
        std::shared_ptr<process_manager::application::services::IChildProcessHolderService> processHolderService,
        std::shared_ptr<process_manager::application::tools::IProcessTerminator> processTerminator,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            shared::api::controllers::BaseGrpController{ logger },
            m_processSpawner{ processSpawner },
            m_processHolderService{ processHolderService },
            m_processTerminator{ processTerminator }
    {
        if (!m_processSpawner || !m_processHolderService || !m_processTerminator) {
            throw std::runtime_error("Process spawner or process holder service or process terminator is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        try {
            return parseSpawnProcess(request, response);
        } catch (std::exception& e) {
            return failed(e.what(), response);
        }
    }

    virtual ::grpc::Status FinishProcess(::grpc::ServerContext* context, const Communication::FinishRequest* request, Communication::FinishResponse* response) override {
        try {
            return parseFinishProcess(request, response);
        } catch (std::exception& e) {
            return failed(e.what(), response);
        }
    }
    
private:
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_processSpawner;
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_processHolderService;
    std::shared_ptr<process_manager::application::tools::IProcessTerminator> m_processTerminator;

    ::grpc::Status parseSpawnProcess(const Communication::SpawnRequest* request, Communication::SpawnResponse* response) {
        auto childProcessInstance = m_processSpawner->spawnChildProcess(request->internal_id());
        if (!childProcessInstance) {
            return failed("[PROCESS_MANAGER_CONTROLLER] Failed to spawn process!", response);
        }

        m_processHolderService->add(*childProcessInstance);
        response->mutable_process()->set_process_id(childProcessInstance->pid);
        response->mutable_process()->set_internal_id(childProcessInstance->internalId);
        response->mutable_process()->set_created_time_ms(childProcessInstance->createdTimeMs);
        response->mutable_process()->set_last_update_time_ms(childProcessInstance->lastUpdateTimeMs);

        return success(response);
    }

    ::grpc::Status parseFinishProcess(const Communication::FinishRequest* request, Communication::FinishResponse* response) {
        const auto internalProcess = m_processHolderService->getById(request->internal_id());
        if (!internalProcess) {
            return failed("[PROCESS_MANAGER_CONTROLLER] Failed to find process with internal id: " + request->internal_id(), response);
        }

        if (!m_processTerminator->terminateAll({process_manager::domain::models::ProcessInfo { 
            .pid = static_cast<int>(internalProcess->pid),
            .name = "child_process"
        }})) {
            return failed("[PROCESS_MANAGER_CONTROLLER] Failed to terminate process with internal id: " + request->internal_id(), response);
        }

        m_processHolderService->removeById(request->internal_id());

        return success(response);
    }
};

}