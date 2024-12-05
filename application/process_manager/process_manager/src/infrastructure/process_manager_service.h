#pragma once

#include <memory>
#include <string_view>
#include <communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>

namespace grpc_services {

class ProcessManagerService final : public Communication::ManagerService::Service {
public:
    ProcessManagerService(std::shared_ptr<shared::application::services::ILogger> logger) : 
        m_logger{ logger } {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        m_logger->log("SpawnProcess called!");

        response->set_success(true);
        response->set_message("Process spawned!");
        response->set_process_id("1234");

        return grpc::Status::OK;
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}