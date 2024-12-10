#pragma once

#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/application/services/IProcessFactory.h>
#include <process_manager/src/application/services/IPlatformIO.h>
#include <process_manager/src/application/services/IPlatformProcessLauncher.h>

#include <process_manager/src/infrastructure/services/LocalProcess.h>

namespace process_manager::infrastructure::services {

using namespace shared::application::services;
using namespace process_manager::domain::models;
using namespace process_manager::application::services;

class ProcessFactory final : public process_manager::application::services::IProcessFactory {
public:
    // @Add params from core
    ProcessFactory(std::unique_ptr<IPlatformProcessLauncher> launcher, std::unique_ptr<IPlatformIO> io, std::shared_ptr<ILogger> logger) :
        m_launcher{ std::move(launcher) }, 
        m_io{ std::move(io) },
        m_logger{ logger } {}
    
    std::unique_ptr<process_manager::application::services::IProcess> createProcess(const std::string& program, const std::vector<std::string>& args) const override {
        ProcessHandles handles = m_launcher->launchProcess(program, args);

        // return std::make_unique<process_manager::infrastructure::services::LocalProcess>(handles.processId, handles.writeHandle, handles.readHandle, m_io);

        return nullptr;
    }

private:
    std::unique_ptr<IPlatformProcessLauncher> m_launcher;
    std::unique_ptr<IPlatformIO> m_io;
    std::shared_ptr<ILogger> m_logger;
};

}