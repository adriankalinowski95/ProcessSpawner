#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <libproc.h>
#include <unistd.h>
#include <signal.h>

#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/domain/models/ProcessInfo.h>
#include <process_manager/src/application/tools/IProcessTerminator.h>

namespace process_manager::infrastructure::tools {
    
class UnixProcessTerminator : public process_manager::application::tools::IProcessTerminator {
public:
    UnixProcessTerminator(std::shared_ptr<shared::application::services::ILogger> logger) : 
        m_logger{ logger } 
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    bool terminateAll(const std::vector<process_manager::domain::models::ProcessInfo>& processes) const override {
        auto result = true;
        for (const auto& process : processes) {
            result &= terminateByPid(process.pid);
        }

        return result;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    bool terminateByPid(pid_t pid) const {
        if (kill(pid, SIGTERM) != 0) {
            m_logger->log(
                shared::application::services::ILogger::LogLevel::Error, 
                "UNIX_PROCESS_TERMINATOR", 
                "Failed to terminate process PID: " + std::to_string(pid) + " Error: " + std::strerror(errno)
            );

            return false;
        }

        m_logger->log(
            shared::application::services::ILogger::LogLevel::Debug,
            "UNIX_PROCESS_TERMINATOR",
            "Process PID: " + std::to_string(pid) + " terminated"
        );

        return true;
    }
};

}