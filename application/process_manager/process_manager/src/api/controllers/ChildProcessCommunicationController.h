#pragma once

#include <memory>
#include <shared/src/application/services/PathMacro.h>
#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>

namespace process_manager::api::controllers {

class ChildProcessCommunicationController : public BaseController {
public:
    ChildProcessCommunicationController(
        std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> childProcessHolderService, 
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_childProcessHolderService{ childProcessHolderService },
            m_logger{ logger }  
        {
            if (!m_childProcessHolderService || !m_logger) {
                throw std::runtime_error("ChildProcessHolderService is not initialized!");
            }
        }

    PATH("/ping-message", pingMessageHandler)
    void pingMessageHandler(boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req) {
        try {
            const std::string body = req.body().c_str();
            auto message = shared::application::utils::ModelsJsonConverter{}.fromJson<shared::domain::models::PingMessage>(body);
            if (!message) {
                throw std::runtime_error("Failed to parse message!");
            }

            auto processInstance = m_childProcessHolderService->getChildProcessByInternalId(message->internalId);
            if (!processInstance) {
                throw std::runtime_error("Failed to find process instance!");
            }

            processInstance->lastUpdateTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            m_childProcessHolderService->updateChildProcess(*processInstance);
            
            message->uniqueNumber = message->uniqueNumber + 1;
            auto response = getResponseWithData(shared::application::utils::ModelsJsonConverter{}.toJson(*message), req);
            http::write(socket, response);
        } catch (std::exception e) {
            failedResponse(e.what(), socket, req);
        }
    }

private:
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_childProcessHolderService;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    void failedResponse(std::string message, boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req) {
        auto baseResponse = this->getBaseResponse(req);
        baseResponse.body() = message;
        http::write(socket, baseResponse);
    }
};

}