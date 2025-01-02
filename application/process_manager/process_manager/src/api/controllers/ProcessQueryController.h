#pragma once

#include <chrono>
#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <environments/environments.h>

#include <shared/src/api/controllers/BaseGrpController.h>
#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

#include <process_manager/src/application/services/IChildProcessHolderService.h>

namespace process_manager::api::controllers {

class ProcessQueryController : public Communication::ProcessQueryService::Service, public shared::api::controllers::BaseGrpController {
public:
   ProcessQueryController(
        std::shared_ptr<process_manager::application::services::IChildProcessHolderService> childProcessHolderService,
        std::shared_ptr<shared::application::services::ILogger> logger) : 
            shared::api::controllers::BaseGrpController{ logger },
            m_childProcessHolderService{ childProcessHolderService }
    {
        if (!m_childProcessHolderService) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status QueryProcesses(::grpc::ServerContext* context, const ::Communication::QueryRequest* request, ::Communication::QueryResponse* response) override {
        try {
            return parse(request, response);
        } catch (std::exception& e) {
            return failed(e.what(), response);
        }
    }

private:
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_childProcessHolderService;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    grpc::Status parse(const ::Communication::QueryRequest* request, ::Communication::QueryResponse* response) {
        switch(request->query_type()) {
            case ::Communication::ProcessQueryType::All:
                return handleQueryAll(response);
            case ::Communication::ProcessQueryType::Running:
                return handleQueryRunning(response);
            // @Todo add by id itp.
            default:
                return failed("Unknown query type!", response);
        }
    }

    grpc::Status handleQueryAll(::Communication::QueryResponse* response) {
        auto processes = m_childProcessHolderService->getAll();
        for (const auto& process : processes) {
            auto processProto = response->add_processes();
            processProto->set_internal_id(process.internalId);
            processProto->set_process_id(process.pid);
            processProto->set_created_time_ms(process.createdTimeMs);
            processProto->set_last_update_time_ms(process.lastUpdateTimeMs);
        }

        return success(response);
    }

    grpc::Status handleQueryRunning(::Communication::QueryResponse* response) {
        // @Todo implement
        return grpc::Status::CANCELLED;
    }
};

}