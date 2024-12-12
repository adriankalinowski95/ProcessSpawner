#pragma once

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/IEndpointService.h>
#include <shared/src/infrastructure/services/SessionService.h>

namespace process_manager::infrastructure::services {

class EndpointService : public shared::application::services::IEndpointService {
public:
    EndpointService(std::shared_ptr<shared::application::services::ILogger> logger) : 
        m_logger{ logger } {}

    void handleRequest(boost::asio::ip::tcp::socket socket) override {
        shared::infrastructure::services::SessionService sessionService{ std::move(socket) };
        const auto data = sessionService.readMessage();
                
        m_logger->logInfo("Server recived: " + data.value_or("empty"));
        if (data) {
            sessionService.sendMessage("Hello, client!", false);
        }
    }
private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}