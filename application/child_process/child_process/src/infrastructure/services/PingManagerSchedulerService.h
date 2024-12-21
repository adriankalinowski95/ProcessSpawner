#pragma once

#include <memory>
#include <string_view>

#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/BaseSchedulerService.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

#include <child_process/src/application/services/GlobalConfig.h>

namespace child_process::infrastructure::services {

class PingManagerSchedulerService {
    using SchedulerServiceT = shared::application::services::BaseSchedulerService;
    using RequestSenderCommandT = shared::infrastructure::commands::GenericRequestSenderCommand<shared::domain::models::PingMessage, shared::domain::models::PingMessage>;

    static constexpr std::string_view Ping_Message_Path = "/ping-message";
    static constexpr std::uint32_t Request_Retires = 5;
    static constexpr std::uint32_t Delay_Between_Retries = 1000; // ms
    static constexpr std::uint32_t Delay_Between_Sheduling = 60 * 1000; // 1 min
public:
    PingManagerSchedulerService(std::shared_ptr<shared::application::services::ILogger> logger) :
        m_logger{ logger },
        m_scheduler{ GetSchedulerConfig() } {}

    void startAndJoin() {
        m_scheduler.start();
        m_scheduler.join();
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    SchedulerServiceT m_scheduler;

    static SchedulerServiceT::Config GetSchedulerConfig() {
        return SchedulerServiceT::Config {
            .delay = Delay_Between_Sheduling,
            .periodicFunction = GetPeriodicFunction()
        };
    }

    static std::function<bool()> GetPeriodicFunction() {
        return []() -> bool {
            const auto processConfig = child_process::application::services::GlobalConfig::getInstance().getProcessConfig();
            if (!processConfig) {
                return false;
            }

            const auto pingMessage = shared::domain::models::PingMessage{
                .internalId = processConfig->internalId,
                .uniqueNumber = shared::application::utils::RandomValueGenerator{}.generateRandomValue()
            };

            auto requestSenderConfig = RequestSenderCommandT::Config{
                .address = processConfig->parentAddress,
                .port = processConfig->parentPort,
                .path = Ping_Message_Path.data(),
                .retries = Request_Retires,
                .delayBetweenRetries = Delay_Between_Retries,
                .validationFunction = [pingMessage] (const shared::domain::models::PingMessage& output) -> bool {
                    return output.uniqueNumber == pingMessage.uniqueNumber + 1;
                }
            };

            const auto result = RequestSenderCommandT{ requestSenderConfig }.sendRequest(pingMessage);
            
            return result.has_value();
        };
    }
};

}