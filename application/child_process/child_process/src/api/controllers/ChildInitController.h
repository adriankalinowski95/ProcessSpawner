#pragma once

#include <shared/src/infrastructure/api/controllers/BaseGrpController.h>

#include <environments/environments.h>
#include <shared/src/domain/protos/child_process_communication.pb.h>
#include <shared/src/domain/protos/child_process_communication.grpc.pb.h>

namespace child_process::api::controllers {

class ChildInitController : public child_process_communication::ChildInitService::Service, public shared::infrastructure::api::controllers::BaseGrpController {
public:
    ChildInitController(std::shared_ptr<shared::application::services::ILogger> logger) : 
        shared::infrastructure::api::controllers::BaseGrpController{ logger } {}

    virtual ::grpc::Status InitChild(::grpc::ServerContext* context, const ::child_process_communication::ChildInitRequest* request, ::child_process_communication::ChildInitResponse* response) override {
        m_logger->logInfo("InitChild request received");
        
        return success(response);
    }
};

}