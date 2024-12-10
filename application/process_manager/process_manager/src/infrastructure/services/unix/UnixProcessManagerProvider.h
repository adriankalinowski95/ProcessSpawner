#pragma once

#include <memory>

#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/infrastructure/services/unix/UnixPlatformIO.h>
#include <process_manager/src/infrastructure/services/unix/UnixPlatformProcessLauncher.h>
#include <process_manager/src/infrastructure/services/ProcessManager.h>
#include <process_manager/src/infrastructure/services/ProcessFactory.h>

namespace process_manager::infrastructure::services::unix {

using namespace process_manager::application::services;

class UnixProcessManagerProvider {
public:
    explicit UnixProcessManagerProvider(std::shared_ptr<shared::application::services::ILogger> logger) : 
        m_logger{ logger },
        m_platformLauncher{ nullptr },
        m_platformIO{ nullptr},
        m_factory{ nullptr },
        m_manager { std::move(m_factory) }
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    std::shared_ptr<IPlatformProcessLauncher> m_platformLauncher;
    std::shared_ptr<IPlatformIO> m_platformIO;
    std::unique_ptr<IProcessFactory> m_factory;
    
    ProcessManager m_manager;

    std::promise<void> m_shutdownPromise;
    std::future<void> m_shutdownFuture;
    std::thread m_managerThread;
};

}