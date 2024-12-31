#pragma once

#include <memory>

#include <process_manager/environments/environments.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <process_manager/src/infrastructure/tools/UnixProcessEnumerator.h>
#include <process_manager/src/infrastructure/tools/UnixProcessSpawner.h>
#include <process_manager/src/infrastructure/tools/UnixProcessTerminator.h>
#include <shared/src/infrastructure/providers/UnixProcessInfoProvider.h>
#include <process_manager/src/application/providers/GlobalConfigProvider.h>

#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/services/ChildProcessSpawnerService.h>

namespace process_manager::application::services {

class ApplicationSingleton {
public:
    ApplicationSingleton() : 
        m_logger{ std::make_shared<shared::infrastructure::services::DefaultLogger>() },
        m_processEnumerator{ std::make_shared<process_manager::infrastructure::tools::UnixProcessEnumerator>(m_logger) },
        m_processSpawner{ std::make_shared<process_manager::infrastructure::tools::UnixProcessSpawner>(m_logger) },
        m_processTerminator{ std::make_shared<process_manager::infrastructure::tools::UnixProcessTerminator>(m_logger) },
        m_currentProcessInfoProvider{ std::make_shared<shared::infrastructure::providers::UnixProcessInfoProvider>() },
        m_globalConfigProvider{ std::make_shared<process_manager::application::providers::GlobalConfigProvider>() },
        m_childProcessConfigProvider{
            std::make_shared<process_manager::application::providers::ChildProcessConfigProvider>(
                m_globalConfigProvider->GetChildProcessConfig().endpoint.ip,
                m_globalConfigProvider->GetChildProcessConfig().endpoint.port,
                m_globalConfigProvider->GetProcessManagerConfig().endpoint.ip,
                m_globalConfigProvider->GetProcessManagerConfig().endpoint.port
            )
        },
        m_childProcessHolderService{ std::make_shared<process_manager::infrastructure::services::ChildProcessHolderService>() },
        m_childProcessSpawnerService{ 
            std::make_shared<process_manager::infrastructure::services::ChildProcessSpawnerService>(
                m_processSpawner,
                m_childProcessConfigProvider,
                m_logger
            )
        } {}
    
    static ApplicationSingleton& GetInstance() {
        static ApplicationSingleton instance;
        
        return instance;
    }

    [[nodiscard]] std::shared_ptr<shared::application::services::ILogger> GetLogger() const {
        return m_logger;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::tools::IProcessEnumerator> GetProcessEnumerator() const {
        return m_processEnumerator;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::tools::IProcessSpawner> GetProcessSpawner() const {
        return m_processSpawner;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::tools::IProcessTerminator> GetProcessTerminator() const {
        return m_processTerminator;
    }

    [[nodiscard]] std::shared_ptr<shared::application::providers::ICurrentProcessInfoProvider> GetCurrentProcessInfoProvider() const {
        return m_currentProcessInfoProvider;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> GetGlobalConfigProvider() const {
        return m_globalConfigProvider;
    }

    // @Todo process manager config!!!
    [[nodiscard]] std::shared_ptr<process_manager::application::providers::ChildProcessConfigProvider> GetChildProcessConfigProvider() const {
        return m_childProcessConfigProvider;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::services::IChildProcessHolderService> GetChildProcessHolderService() const {
        return m_childProcessHolderService;
    }

    [[nodiscard]] std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> GetChildProcessSpawnerService() const {
        return m_childProcessSpawnerService;
    }

private:
    std::shared_ptr<shared::infrastructure::services::DefaultLogger> m_logger;
    std::shared_ptr<process_manager::application::tools::IProcessEnumerator> m_processEnumerator;
    std::shared_ptr<process_manager::application::tools::IProcessSpawner> m_processSpawner;
    std::shared_ptr<process_manager::application::tools::IProcessTerminator> m_processTerminator;
    std::shared_ptr<shared::application::providers::ICurrentProcessInfoProvider> m_currentProcessInfoProvider;
    std::shared_ptr<process_manager::application::providers::GlobalConfigProvider> m_globalConfigProvider;
    std::shared_ptr<process_manager::application::providers::ChildProcessConfigProvider> m_childProcessConfigProvider;
    std::shared_ptr<process_manager::application::services::IChildProcessHolderService> m_childProcessHolderService;
    std::shared_ptr<process_manager::application::services::IChildProcessSpawnerService> m_childProcessSpawnerService;
};

}