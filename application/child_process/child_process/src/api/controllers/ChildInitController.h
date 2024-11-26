#pragma once

#include <environments/environments.h>
#include <shared/src/api/controllers/BaseGrpController.h>
#include <shared/src/domain/protos/child_process_communication.pb.h>
#include <shared/src/domain/protos/child_process_communication.grpc.pb.h>

#include <child_process/src/application/services/ISchedulerService.h>

namespace child_process::api::controllers {

class ChildInitController : public child_process_communication::ChildInitService::Service, public shared::api::controllers::BaseGrpController {
public:
    ChildInitController(
        std::shared_ptr<child_process::application::services::ISchedulerService> eventProviderSchedulerService,
        std::shared_ptr<child_process::application::services::ISchedulerService> pingManagerSchedulerService,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            shared::api::controllers::BaseGrpController{ logger },
            m_eventProviderSchedulerService{ eventProviderSchedulerService },
            m_pingManagerSchedulerService{ pingManagerSchedulerService }
    {
        if (!m_eventProviderSchedulerService || !m_pingManagerSchedulerService) {
            throw std::runtime_error("Scheduler services are not initialized!");
        }
    }

    virtual ::grpc::Status InitChild(::grpc::ServerContext* context, const ::child_process_communication::ChildInitRequest* request, ::child_process_communication::ChildInitResponse* response) override {
        try {
            return parse(request, response);
        } catch (std::exception& e) {
            return failed(e.what(), response);
        }
    }

private:
    std::shared_ptr<child_process::application::services::ISchedulerService> m_eventProviderSchedulerService;
    std::shared_ptr<child_process::application::services::ISchedulerService> m_pingManagerSchedulerService; 

    ::grpc::Status parse(const ::child_process_communication::ChildInitRequest* request, ::child_process_communication::ChildInitResponse* response) {
        // Start schedulers!
        m_pingManagerSchedulerService->start();
        m_eventProviderSchedulerService->start();
    
        m_logger->log(
            shared::application::services::ILogger::LogLevel::Info,
            "CHILD_INIT_CONTROLLER", 
            request->internal_id() + " initialized!"
        );
    
        return success(response);
    }
    
};

}