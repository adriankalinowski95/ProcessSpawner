#pragma once

#include <memory>
#include <shared/src/application/services/PathMacro.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>

namespace process_manager::api::controllers {

class ChildProcessCommunicationController : public BaseController {
public:
    ChildProcessCommunicationController(std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> childProcessHolderService) :
        m_childProcessHolderService{ childProcessHolderService } {
            if (!m_childProcessHolderService) {
                throw std::runtime_error("ChildProcessHolderService is not initialized!");
            }
        }

    PATH("/ping-message", pingMessageHandler)
    void pingMessageHandler(boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req) {
        const std::string body = req.body().c_str();
        auto message = shared::application::utils::ModelsJsonConverter{}.fromJson<shared::domain::models::PingMessage>(body);
        if (!message) {
            auto baseResponse = this->getBaseResponse(req);
            baseResponse.body() = "Failed to parse message!";
            http::write(socket, baseResponse);

            return;
        }

        auto processInstance = m_childProcessHolderService->getChildProcessByInternalId(message->internalId);
        if (!processInstance) {
            auto baseResponse = this->getBaseResponse(req);
            baseResponse.body() = "Failed to find process instance!";
            http::write(socket, baseResponse);

            return;
        }

        processInstance->lastUpdateTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        auto baseResponse = this->getBaseResponse(req);
        baseResponse.body() = "Ok";

        http::write(socket, baseResponse);
    }

private:
    std::shared_ptr<process_manager::infrastructure::services::ChildProcessHolderService> m_childProcessHolderService;
};

}