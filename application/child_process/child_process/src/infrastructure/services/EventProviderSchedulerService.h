#pragma once

#include <memory>
#include <string_view>

#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/BaseSchedulerService.h>

#include <child_process/src/infrastructure/commands/CoreCommandCommunicationCommand.h>
#include <child_process/src/application/services/GlobalConfig.h>

namespace child_process::infrastructure::services {

class EventProviderSchedulerService {
    using SchedulerServiceT = shared::application::services::BaseSchedulerService;

    static constexpr std::uint32_t Request_Retires = 2;
    static constexpr std::uint32_t Delay_Between_Retries = 10000; // ms
    static constexpr std::uint32_t Delay_Between_Sheduling = 10 * 1000; // 1 min
public:
    EventProviderSchedulerService(std::shared_ptr<shared::application::services::ILogger> logger) :
        m_logger{ logger },
        m_scheduler{ GetSchedulerConfig() } {}

    void start() {
        m_scheduler.start();
    }

    void startAndJoin() {
        m_scheduler.start();
        m_scheduler.join();
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    SchedulerServiceT m_scheduler;

    SchedulerServiceT::Config GetSchedulerConfig() {
        return SchedulerServiceT::Config {
            .delay = Delay_Between_Sheduling,
            .periodicFunction = GetPeriodicFunction()
        };
    }

    std::function<bool()> GetPeriodicFunction() {
        return [this]() -> bool {
            const auto processConfig = child_process::application::services::GlobalConfig::getInstance().getProcessConfig();
            if (!processConfig) {
                return false;
            }

            auto request = ::core_communication::CoreCommandRequest{};
            request.mutable_process()->set_internal_id(processConfig->internalId.data());
            request.set_event_name("temp_name");
            request.set_event_value("temp_value");

            child_process::infrastructre::commands::CoreCommandCommunicationCommand::Sender::Config config {
                processConfig->parentAddress,
                processConfig->parentPort,
                Request_Retires,
                Delay_Between_Retries,
                [] (const ::core_communication::CoreCommandResponse& output) -> bool {
                    return output.result().success();
                }
            };

            child_process::infrastructre::commands::CoreCommandCommunicationCommand sender{ config };
            auto result = sender.sendRequest(request);
            if (!result.has_value()) {
                m_logger->logError("Failed to send event provider request");

                return false;
            }

            return result->result().success();
        };
    }
};

}