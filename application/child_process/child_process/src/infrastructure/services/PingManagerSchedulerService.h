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
        if (!m_commandsFactory || !m_globalConfigProvider || !m_logger) {
            throw std::runtime_error("Global config provider or logger is not initialized!");
        }
    }

    void start() override {
        std::scoped_lock lock(m_mutex);
        if (m_scheduler.isRunning()) {
            throw std::runtime_error("Scheduler is already running!");
        }

        m_scheduler.run();
    }

    void startAndJoin() override {
        std::scoped_lock lock(m_mutex);
        if (m_scheduler.isRunning()) {
            throw std::runtime_error("Scheduler is already running!");
        }

        m_scheduler.run();
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
            try {
                const auto processConfig = m_globalConfigProvider->getProcessConfig();
                if (!processConfig) {
                    throw std::runtime_error("Failed to get process config");
                }

                auto request = createRequest();
                if (!request) {
                    throw std::runtime_error("Failed to create request");
                }

                auto sender = m_commandsFactory->createChildPingRequestCommand();
                if (!sender) {
                    throw std::runtime_error("Failed to create sender");
                }

                auto result = sender->ping(*request);
                if (!result) {
                    // @Todo Change this - if process manager exist, it will return false,
                    // but counter shouldn't be increased
                    m_failruesInRow++;
                    
                    m_logger->log(
                        shared::application::services::ILogger::LogLevel::Debug,
                        "PING_MANAGER_SCHEDULER_SERVICE",
                        processConfig->internalId + " failed to ping"
                    );
                } else {
                    m_logger->log(
                        shared::application::services::ILogger::LogLevel::Debug,
                        "PING_MANAGER_SCHEDULER_SERVICE",
                        processConfig->internalId + " pinged successfully"
                    );

                    m_failruesInRow = 0;
                }

                if (m_failruesInRow >= Failures_To_Exit) {
                    m_logger->log(
                        shared::application::services::ILogger::LogLevel::Error, 
                        "PING_MANAGER_SCHEDULER_SERVICE",
                        "Too many failures in a row. Exiting..."
                    );

                    std::exit(EXIT_FAILURE);
                }

                return result;
            } catch(const std::exception& e) {
                m_logger->log(
                    shared::application::services::ILogger::LogLevel::Error,
                    "PING_MANAGER_SCHEDULER_SERVICE",
                    e.what()
                );

                return false;
            }
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