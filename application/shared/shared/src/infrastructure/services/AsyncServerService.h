#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <exception>
#include <atomic>

#include <shared/src/infrastructure/providers/BoostAcceptorProvider.h>
#include <shared/src/application/services/IEndpointService.h>
#include <shared/shared/src/application/services/ILogger.h>

namespace shared::infrastructure::services {

class AsyncServerService {
public:
    AsyncServerService(
        std::string address, 
        std::uint32_t port, 
        std::unique_ptr<shared::application::services::IEndpointService> endpointService,
        std::shared_ptr<shared::application::services::ILogger> logger = nullptr) :
            m_acceptorProvider{ address, port },
            m_endpointService{ std::move(endpointService) },
            m_logger{ logger } {
                if (!m_endpointService) {
                    throw std::runtime_error("Endpoint service is not initialized!");
                }
            }

    void start() {
        m_isRunning = true;
        m_serverThread = std::thread(&AsyncServerService::run, this); 
    }

    void stop() {
        m_isRunning = true;
    }

    void join() {
        m_serverThread.join();
    }

    std::unique_ptr<shared::application::services::IEndpointService>& getEndpointService() {
        return m_endpointService;
    }

    void registerController(shared::application::services::BaseController& controller) {
        m_endpointService->addHandlers(controller.getHandlers());
    }

private:
    shared::infrastructure::providers::BoostAcceptorProvider m_acceptorProvider;
    std::unique_ptr<shared::application::services::IEndpointService> m_endpointService;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    std::thread m_serverThread;
    std::atomic<bool> m_isRunning;

    void run() {
        try {
            auto acceptor = m_acceptorProvider.createAcceptor();
            if (acceptor == nullptr) {
                throw std::runtime_error("Can't create acceptor");
            }

            while (m_isRunning) {
                auto socket = acceptor->accept();

                // @Todo create pool of threads
                m_endpointService->handleRequest(std::move(socket));
            }
        } catch(std::exception& e) {
            if (m_logger) {
                m_logger->logError(e.what());
            }
        }
    }
};

}