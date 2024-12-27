#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>
#include <shared/src/application/services/ILogger.h>

namespace shared::api::controllers {

class BaseGrpController {
public:
    BaseGrpController(std::shared_ptr<shared::application::services::ILogger> logger) : m_logger{ logger } {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    template <typename T>
    grpc::Status failed(const std::string& message, T* response) {
        m_logger->logError(message);

        response->set_success(false);
        response->set_message(message);

        return grpc::Status::CANCELLED;
    }

    template <typename T>
    grpc::Status success(T* response) {
        response->set_success(true);
        response->set_message("OK");

        return grpc::Status::OK;
    }

protected:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}