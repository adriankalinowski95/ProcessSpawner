#pragma once

#include <memory>
#include <cstdint>
#include <exception>

#include <shared/src/application/services/ILogger.h>
#include <child_process/src/application/providers/GlobalConfigProvider.h>
#include <child_process/src/infrastructure/commands/ChildProcessCommandsFactory.h>

namespace child_process::infrastructure::services {

class CoreQueryParamsService {
    static constexpr std::uint32_t Request_Retires = 1;
    static constexpr std::uint32_t Delay_Between_Retries = 1000; // ms
public:
    CoreQueryParamsService(
        std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> commandsFactory,
        std::shared_ptr<child_process::application::providers::GlobalConfigProvider> globalConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_commandsFactory{ commandsFactory },
            m_globalConfigProvider{ globalConfigProvider },
            m_logger{ logger }
    {
        if (!m_commandsFactory || !m_globalConfigProvider || !m_logger) {
            throw std::runtime_error("Global config provider or logger is not initialized!");
        }
    }

    bool loadParam(std::string paramName) {
        auto request = createRequest(paramName);
        if (!request) {
            return false;
        }

        auto sender = m_commandsFactory->createCoreQueryCommunicationCommand();
        if (!sender) {
            m_logger->logError("Failed to create core query communication command");

            return false;
        }

        auto result = sender->sendRequest(*request);
        if (!result.has_value()) {
            m_logger->logError("Failed to query param request");

            return false;
        }

        if (!result->result().success()) {
            m_logger->logError("Failed to query param request");
            
            return false;
        }

        // @Todo for testing!
        // m_logger->logInfo(std::string("Query param value: ") + result->param_value());

        return true;
    }

private:
    std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> m_commandsFactory;
    std::shared_ptr<child_process::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    std::optional<::core_communication::CoreQueryRequest> createRequest(std::string paramName) {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return std::nullopt;
        }

        auto request = ::core_communication::CoreQueryRequest{};
        request.mutable_process()->set_internal_id(processConfig->internalId.data());
        request.set_param_name(paramName.data());

        return request;
    }
};

}