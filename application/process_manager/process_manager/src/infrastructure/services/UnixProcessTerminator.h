#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <libproc.h>
#include <unistd.h>
#include <signal.h>

#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/domain/models/ProcessInfo.h>
#include <process_manager/src/application/services/IProcessTerminator.h>

namespace process_manager::infrastructure::services {

using namespace process_manager::domain::models;

class UnixProcessTerminator : public process_manager::application::services::IProcessTerminator {
public:
    UnixProcessTerminator(std::shared_ptr<shared::application::services::ILogger> logger) : m_logger{ logger } {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    bool terminateAll(const std::vector<ProcessInfo>& processes) const override {
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
            m_logger->logError("Failed to terminate process PID: " + std::to_string(pid) + " Error: " + std::strerror(errno));

            return false;
        }

        m_logger->log("Process PID: " + std::to_string(pid) + " terminated");

        return true;
    }
};

}