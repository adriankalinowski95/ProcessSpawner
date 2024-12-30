#pragma once

#include <memory>
#include <cstdint>

#include <shared/src/application/services/ILogger.h>
#include <child_process/src/infrastructure/commands/CoreQueryCommunicationCommand.h>
#include <child_process/src/application/services/GlobalConfig.h>

namespace child_process::infrastructure::services {

class CoreQueryParamsService {
    static constexpr std::uint32_t Request_Retires = 1;
    static constexpr std::uint32_t Delay_Between_Retries = 1000; // ms
public:
    CoreQueryParamsService(std::shared_ptr<shared::application::services::ILogger> logger) :
        m_logger{ logger } {}

    bool loadParam(std::string paramName) {
        const auto processConfig = child_process::application::services::GlobalConfig::getInstance().getProcessConfig();
        if (!processConfig) {
            return false;
        }

        auto request = ::core_communication::CoreQueryRequest{};
        request.mutable_process()->set_internal_id(processConfig->internalId.data());
        request.set_param_name(paramName.data());

        child_process::infrastructre::commands::CoreQueryCommunicationCommand::Sender::Config config {
            processConfig->parentAddress,
            processConfig->parentPort,
            Request_Retires,
            Delay_Between_Retries,
            [] (const ::core_communication::CoreQueryResponse& output) -> bool {
                return output.result().success();
            }
        };

        child_process::infrastructre::commands::CoreQueryCommunicationCommand sender{ config };
        auto result = sender.sendRequest(request);
        if (!result.has_value()) {
            m_logger->logError("Failed to query param request");

            return false;
        }

        if (!result->result().success()) {
            m_logger->logError("Failed to query param request");
            
            return false;
        }

        m_logger->logInfo(std::string("Query param value: ") + result->param_value());

        return true;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}