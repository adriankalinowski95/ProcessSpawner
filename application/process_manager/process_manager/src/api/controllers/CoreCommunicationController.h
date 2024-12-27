#pragma once

#include <chrono>
#include <memory>
#include <exception>

#include <environments/environments.h>

#include <shared/src/api/controllers/BaseGrpController.h>
#include <shared/src/domain/protos/core_communication.pb.h>
#include <shared/src/domain/protos/core_communication.grpc.pb.h>

#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>

namespace process_manager::api::controllers {

class CoreCommunicationController : public core_communication::CoreCommunicationService::Service, public shared::api::controllers::BaseGrpController {
    using RequestT = ::core_communication::CoreQueryRequest;
    using ResponseT = ::core_communication::CoreQueryResponse;
public:
    CoreCommunicationController(
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> childProcessHolderService, 
        std::shared_ptr<shared::application::services::ILogger> logger) : 
        shared::api::controllers::BaseGrpController{ logger } {}

    virtual ::grpc::Status Ping(::grpc::ServerContext* context, const RequestT* request, ResponseT* response) override {
        try {
            return parse(request, response);
        } catch (std::exception e) {
            return failed(e.what(), response);
        }
    }

private:
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_childProcessHolderService;

    ::grpc::Status parse(const RequestT* request, ResponseT* response) {
        auto processInstance = m_childProcessHolderService->getChildProcessByInternalId(request->internal_id());
        if (!processInstance) {
            throw std::runtime_error("Failed to find process instance!");
        }

        processInstance->lastUpdateTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        m_childProcessHolderService->updateChildProcess(*processInstance);

        return success(response);
    }
};

}