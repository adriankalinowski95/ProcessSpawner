#pragma once

#include <chrono>
#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <environments/environments.h>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>

namespace process_manager::api::controllers {

class ProcessQueryController : public Communication::ProcessQueryService::Service {
public:
   ProcessQueryController(
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> childProcessHolderService,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            m_childProcessHolderService{ childProcessHolderService },
            m_logger{ logger }
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status QueryProcesses(::grpc::ServerContext* context, const ::Communication::QueryRequest* request, ::Communication::QueryResponse* response) override {
        return handleQueryByType(request->query_type(), response);
    }

private:
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_childProcessHolderService;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    grpc::Status handleQueryByType(::Communication::ProcessQueryType queryType, ::Communication::QueryResponse* response) {
        switch(queryType) {
            case ::Communication::ProcessQueryType::All:
                return handleQueryAll(response);
            case ::Communication::ProcessQueryType::Running:
                return handleQueryRunning(response);
            // @Todo add by id itp.
            default:
                return failed("Unknown query type!", response);
        }
    }

    grpc::Status failed(const std::string& message, ::Communication::QueryResponse* response) {
        m_logger->logError(message);

        response->set_success(false);
        response->set_message(message);

        return grpc::Status::CANCELLED;
    }

    grpc::Status handleQueryAll(::Communication::QueryResponse* response) {
        auto processes = m_childProcessHolderService->getChildProcesses();
        for (const auto& process : processes) {
            auto processProto = response->add_processes();
            processProto->set_internal_id(process.internalId);
            processProto->set_process_id(process.pid);
            processProto->set_created_time_ms(process.createdTimeMs);
            processProto->set_last_update_time_ms(process.lastUpdateTimeMs);
        }

        response->set_success(true);
        response->set_message("OK");

        // @Todo implement
        return grpc::Status::OK;
    }

    grpc::Status handleQueryRunning(::Communication::QueryResponse* response) {
        // @Todo implement
        return grpc::Status::CANCELLED;
    }
};

}