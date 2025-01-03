#pragma once

#include <memory>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <child_process/src/application/providers/GlobalConfigProvider.h>
#include <child_process/src/infrastructure/services/CoreQueryParamsService.h>
#include <child_process/src/infrastructure/services/EventProviderSchedulerService.h>
#include <child_process/src/infrastructure/services/PingManagerSchedulerService.h>
#include <child_process/src/infrastructure/commands/ChildProcessCommandsFactory.h>

namespace child_process::application::services {

class ApplicationSingleton {
public:
    ApplicationSingleton() : 
        m_logger{ std::make_shared<shared::infrastructure::services::DefaultLogger>() },
        m_globalConfigProvider{ std::make_shared<child_process::application::providers::GlobalConfigProvider>() },
        m_childProcessCommandsFactory{ std::make_shared<child_process::infrastructure::commands::ChildProcessCommandsFactory>(m_globalConfigProvider) },
        m_coreQueryParamsService{ std::make_shared<child_process::infrastructure::services::CoreQueryParamsService>(m_childProcessCommandsFactory, m_globalConfigProvider, m_logger) },
        m_eventProviderSchedulerService{ std::make_shared<child_process::infrastructure::services::EventProviderSchedulerService>(m_childProcessCommandsFactory, m_globalConfigProvider, m_logger) },
        m_pingManagerSchedulerService{ std::make_shared<child_process::infrastructure::services::PingManagerSchedulerService>(m_childProcessCommandsFactory, m_globalConfigProvider, m_logger) } {}
    
    [[nodiscard]] static ApplicationSingleton& GetInstance() {
        static ApplicationSingleton instance{};

        return instance;
    }

    [[nodiscard]] std::shared_ptr<shared::application::services::ILogger> GetLogger() const {
        return m_logger;
    }

    [[nodiscard]] std::shared_ptr<child_process::application::providers::GlobalConfigProvider> GetGlobalConfigProvider() const {
        return m_globalConfigProvider;
    }

    [[nodiscard]] std::shared_ptr<child_process::infrastructure::services::CoreQueryParamsService> GetCoreQueryParamsService() const {
        return m_coreQueryParamsService;
    }

    [[nodiscard]] std::shared_ptr<child_process::application::services::ISchedulerService> GetEventProviderSchedulerService() const {
        return m_eventProviderSchedulerService;
    }

    [[nodiscard]] std::shared_ptr<child_process::application::services::ISchedulerService> GetPingManagerSchedulerService() const {
        return m_pingManagerSchedulerService;
    }

    [[nodiscard]] std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> GetChildProcessCommandsFactory() const {
        return m_childProcessCommandsFactory;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    std::shared_ptr<child_process::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<child_process::infrastructure::services::CoreQueryParamsService> m_coreQueryParamsService;
    std::shared_ptr<child_process::application::services::ISchedulerService> m_eventProviderSchedulerService;
    std::shared_ptr<child_process::application::services::ISchedulerService> m_pingManagerSchedulerService;
    std::shared_ptr<child_process::infrastructure::commands::ChildProcessCommandsFactory> m_childProcessCommandsFactory;
};

}