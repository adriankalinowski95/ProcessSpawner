#pragma once

#include <memory>
#include <optional>
#include <exception>
#include <string_view>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/BaseSchedulerService.h>
#include <child_process/src/application/services/ISchedulerService.h>
#include <child_process/src/application/providers/GlobalConfigProvider.h>
#include <child_process/src/infrastructure/commands/ChildProcessCommandsFactory.h>

namespace child_process::infrastructure::services {

class PingManagerSchedulerService : public child_process::application::services::ISchedulerService{
    using SchedulerServiceT = shared::application::services::BaseSchedulerService;
    static constexpr std::uint32_t Delay_Between_Sheduling = 60 * 1000; // 1 min
    static constexpr std::uint32_t Failures_To_Exit = 5;
public:
    PingManagerSchedulerService(
        std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> commandsFactory,
        std::shared_ptr<child_process::application::providers::GlobalConfigProvider> globalConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_commandsFactory{ commandsFactory },
            m_globalConfigProvider{ globalConfigProvider },
            m_logger{ logger },
            m_scheduler{ GetSchedulerConfig() },
            m_failruesInRow{},
            m_mutex{}
    {
        if (!m_globalConfigProvider || !m_logger) {
            throw std::runtime_error("Global config provider or logger is not initialized!");
        }
    }

    void start() override {
        std::scoped_lock lock(m_mutex);
        if (m_scheduler.isRunning()) {
            throw std::runtime_error("Scheduler is already running!");
        }

        m_scheduler.start();
    }

    void startAndJoin() override {
        std::scoped_lock lock(m_mutex);
        if (m_scheduler.isRunning()) {
            throw std::runtime_error("Scheduler is already running!");
        }

        m_scheduler.start();
        m_scheduler.join();
    }

    [[nodiscard]] bool isRunning() override {
        return m_scheduler.isRunning();
    }

private:
    std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> m_commandsFactory;
    std::shared_ptr<child_process::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    SchedulerServiceT m_scheduler;
    std::uint32_t m_failruesInRow;
    std::mutex m_mutex;

    SchedulerServiceT::Config GetSchedulerConfig() {
        return SchedulerServiceT::Config {
            .delay = Delay_Between_Sheduling,
            .periodicFunction = GetPeriodicFunction()
        };
    }

    std::function<bool()> GetPeriodicFunction() {
        return [this]() -> bool {
            const auto processConfig = m_globalConfigProvider->getProcessConfig();
            if (!processConfig) {
                m_logger->logError("Failed to get process config");

                return false;
            }

            auto request = createRequest();
            if (!request) {
                m_logger->logError("Failed to create request");

                return false;
            }

            auto sender = m_commandsFactory->createChildPingRequestCommand();
            if (!sender) {
                m_logger->logError("Failed to create child ping request command");

                return false;
            }

            auto result = sender->ping(*request);
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

    std::optional<::child_process_communication::ChildPingRequest> createRequest() {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return std::nullopt;
        }

        auto request = ::child_process_communication::ChildPingRequest{};
        request.set_internal_id(processConfig->internalId.data());

        return request;
    }
};

}