#pragma once

#include <memory>
#include <libproc.h>
#include <unistd.h>
#include <signal.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/tools/IProcessEnumerator.h>

namespace process_manager::infrastructure::tools {

class UnixProcessEnumerator : public process_manager::application::tools::IProcessEnumerator {
public:
    UnixProcessEnumerator(std::shared_ptr<shared::application::services::ILogger> logger) : 
        process_manager::application::tools::IProcessEnumerator{},
        m_logger{ logger } 
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    std::vector<process_manager::domain::models::ProcessInfo> enumerateAll() const override {
        return getAllProcesses();
    }

    std::vector<process_manager::domain::models::ProcessInfo> enumerateWhereNameEquals(std::string_view name) const override {
        auto allProcesses = getAllProcesses();
        std::vector<process_manager::domain::models::ProcessInfo> result;
        result.reserve(allProcesses.size());
        
        for (auto& proc : allProcesses) {
            if (proc.name == name) {
                result.push_back(std::move(proc));
            }
        }
        return result;
    }

    std::vector<process_manager::domain::models::ProcessInfo> enumerateWhereNameContains(std::string_view substring) const override {
        auto allProcesses = getAllProcesses();
        std::vector<process_manager::domain::models::ProcessInfo> result;
        result.reserve(allProcesses.size());

        for (auto& proc : allProcesses) {
            if (proc.name.find(substring) != std::string::npos) {
                result.push_back(std::move(proc));
            }
        }
        return result;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    std::vector<process_manager::domain::models::ProcessInfo> getAllProcesses() const {
        std::vector<process_manager::domain::models::ProcessInfo> processes;
        const int maxCount = 1024;
        std::vector<pid_t> pids(maxCount, 0);

        const int pidCount = proc_listpids(PROC_ALL_PIDS, 0, pids.data(), static_cast<int>(pids.size() * sizeof(pid_t)));
        if (pidCount <= 0) {
            m_logger->log(
                shared::application::services::ILogger::LogLevel::Error,
                "UNIX_PROCESS_ENUMERATOR",
                "Error listing processes: " + std::string(std::strerror(errno))
            );

            return processes;
        }

        processes.reserve(static_cast<size_t>(pidCount));
        for (int i = 0; i < pidCount; ++i) {
            const pid_t pid = pids[i];
            if (pid <= 0) {
                continue;
            }

            char nameBuffer[PROC_PIDPATHINFO_MAXSIZE];
            std::memset(nameBuffer, 0, sizeof(nameBuffer));
            const int nameLength = proc_name(pid, nameBuffer, sizeof(nameBuffer));
            if (nameLength > 0) {
                process_manager::domain::models::ProcessInfo info{pid, std::string(nameBuffer)};
                processes.push_back(std::move(info));
            }
        }

        return processes;
    }
};


}