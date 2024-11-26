#pragma once

#include <mutex>
#include <memory>
#include <exception>
#include <string_view>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/BaseSchedulerService.h>
#include <child_process/src/application/services/ISchedulerService.h>
#include <child_process/src/application/providers/GlobalConfigProvider.h>
#include <child_process/src/infrastructure/services/CoreQueryParamsService.h>
#include <child_process/src/infrastructure/commands/ChildProcessCommandsFactory.h>

namespace child_process::infrastructure::services {

class EventProviderSchedulerService : public child_process::application::services::ISchedulerService {
    using SchedulerServiceT = shared::application::services::BaseSchedulerService;

    static constexpr std::uint32_t Request_Retires = 2;
    static constexpr std::uint32_t Delay_Between_Retries = 10000; // ms
    static constexpr std::uint32_t Delay_Between_Sheduling = 10 * 1000; // 1 min
public:
    EventProviderSchedulerService(
        std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> commandsFactory,
        std::shared_ptr<child_process::application::providers::GlobalConfigProvider> globalConfigProvider,
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_commandsFactory{ commandsFactory },
            m_globalConfigProvider{ globalConfigProvider },
            m_logger{ logger },
            m_scheduler{ GetSchedulerConfig() },
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
    std::mutex m_mutex;

    SchedulerServiceT::Config GetSchedulerConfig() {
        return SchedulerServiceT::Config {
            .delay = Delay_Between_Sheduling,
            .periodicFunction = GetPeriodicFunction()
        };
    }

    // @Todo - remove this function
    // Its only for test!
    void load() {
        /*
        child_process::infrastructure::services::CoreQueryParamsService coreQueryParamsService{ 
            m_globalConfigProvider,
            m_logger
        };
        const auto result = coreQueryParamsService.loadParam("param1");
        */
    }

    std::function<bool()> GetPeriodicFunction() {
        return [this]() -> bool {
            try {
                auto request = createRequest();
                if (!request.has_value()) {
                    throw std::runtime_error("Failed to create request");
                }

                auto sender = m_commandsFactory->createCoreCommandCommunicationCommand();
                if (!sender) {
                    throw std::runtime_error("Failed to create sender");
                }     

                auto result = sender->sendRequest(*request);
                if (!result.has_value()) {
                    throw std::runtime_error("Failed to send request");
                } 

                return result->result().success();
             }
            catch (std::exception& e) {
                m_logger->log(
                    shared::application::services::ILogger::LogLevel::Error,
                    "EVENT_PROVIDER_SCHEDULER_SERVICE",
                    e.what()
                );

                return false;
            }
        };
    }

    std::optional<::core_communication::CoreCommandRequest> createRequest() {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return std::nullopt;
        }

        // EXAMPLE
        auto request = ::core_communication::CoreCommandRequest{};
        request.mutable_process()->set_internal_id(processConfig->internalId.data());
        request.set_event_name("temp_name");
        request.set_event_value("temp_value");

        return request;
    }
};

}