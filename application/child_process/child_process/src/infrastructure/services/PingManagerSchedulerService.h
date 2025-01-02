#pragma once

#include <memory>
#include <string_view>

#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/BaseSchedulerService.h>

#include <child_process/src/infrastructure/commands/ChildPingRequestCommand.h>
#include <child_process/src/application/services/GlobalConfig.h>

namespace child_process::infrastructure::services {

class PingManagerSchedulerService {
    using SchedulerServiceT = shared::application::services::BaseSchedulerService;

    static constexpr std::uint32_t Request_Retires = 5;
    static constexpr std::uint32_t Delay_Between_Retries = 1000; // ms
    static constexpr std::uint32_t Delay_Between_Sheduling = 60 * 1000; // 1 min
    static constexpr std::uint32_t Failures_To_Exit = 5;
public:
    PingManagerSchedulerService(std::shared_ptr<shared::application::services::ILogger> logger) :
        m_logger{ logger },
        m_scheduler{ GetSchedulerConfig() },
        m_failruesInRow{} {}

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
    std::uint32_t m_failruesInRow;

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

            auto pingMessage = ::child_process_communication::ChildPingRequest{};
            pingMessage.set_internal_id(processConfig->internalId.data());

            child_process::infrastructre::commands::ChildPingRequestCommand::Sender::Config config {
                processConfig->parentAddress,
                processConfig->parentPort,
                Request_Retires,
                Delay_Between_Retries,
                [pingMessage] (const ::child_process_communication::ChildPingResponse& output) -> bool {
                    return output.success();
                }
            };

            child_process::infrastructre::commands::ChildPingRequestCommand sender{ config };
        
            auto result = sender.ping(pingMessage);
            if (!result) {
                m_logger->logError("Failed to send ping message");
                m_failruesInRow++;
            } else {
                m_logger->logInfo(processConfig->internalId + " pinged successfully");
                m_failruesInRow = 0;
            }

            if (m_failruesInRow >= Failures_To_Exit) {
                m_logger->logError("Too many failures in a row. Exiting...");
                
                std::exit(EXIT_FAILURE);
            }

            return result;
        };
    }
};

}