#pragma once

#include <chrono>
#include <memory>
#include <exception>

#include <environments/environments.h>

#include <shared/src/api/controllers/BaseGrpController.h>
#include <shared/src/domain/protos/child_process_communication.pb.h>
#include <shared/src/domain/protos/child_process_communication.grpc.pb.h>

#include <process_manager/src/application/services/IChildProcessHolderService.h>

namespace process_manager::api::controllers {

class ChildPingController : public child_process_communication::ChildPingService::Service, public shared::api::controllers::BaseGrpController {
public:
    ChildPingController(
        std::shared_ptr<process_manager::application::services::IChildProcessHolderService> childProcessHolderService, 
        std::shared_ptr<shared::application::services::ILogger> logger) : 
        shared::api::controllers::BaseGrpController{ logger },
        m_childProcessHolderService { childProcessHolderService } 
    {
        if (!m_childProcessHolderService) {
            throw std::runtime_error("Child process holder service is not initialized!");
        }
    }

    virtual ::grpc::Status Ping(::grpc::ServerContext* context, const ::child_process_communication::ChildPingRequest* request, ::child_process_communication::ChildPingResponse* response) override {
        try {
            return parse(request, response);
        } catch (std::exception& e) {
            return failed(e.what(), response);
        }
    }

private:
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_childProcessHolderService;

    ::grpc::Status parse(const ::child_process_communication::ChildPingRequest* request, ::child_process_communication::ChildPingResponse* response) {
        auto processInstance = m_childProcessHolderService->getById(request->internal_id());
        if (!processInstance) {
            throw std::runtime_error("[CHILD_PING_CONTROLLER] Failed to find process instance!");
        }

        processInstance->lastUpdateTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        m_childProcessHolderService->update(*processInstance);

        return success(response);
    }
};

}