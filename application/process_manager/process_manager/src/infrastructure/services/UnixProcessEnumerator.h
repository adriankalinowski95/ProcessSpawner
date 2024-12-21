#pragma once

#include <memory>
#include <libproc.h>
#include <unistd.h>
#include <signal.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/services/IProcessEnumerator.h>

namespace process_manager::infrastructure::services {

using namespace process_manager::domain::models;

class UnixProcessEnumerator : public process_manager::application::services::IProcessEnumerator {
public:
    UnixProcessEnumerator(std::shared_ptr<shared::application::services::ILogger> logger) : m_logger{ logger } {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    std::vector<ProcessInfo> enumerateAll() const override {
        return getAllProcesses();
    }

    std::vector<ProcessInfo> enumerateWhereNameEquals(std::string_view name) const override {
        auto allProcesses = getAllProcesses();
        std::vector<ProcessInfo> result;
        result.reserve(allProcesses.size());
        
        for (auto& proc : allProcesses) {
            if (proc.name == name) {
                result.push_back(std::move(proc));
            }
        }
        return result;
    }

    std::vector<ProcessInfo> enumerateWhereNameContains(std::string_view substring) const override {
        auto allProcesses = getAllProcesses();
        std::vector<ProcessInfo> result;
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

    std::vector<ProcessInfo> getAllProcesses() const {
        std::vector<ProcessInfo> processes;
        const int maxCount = 1024;
        std::vector<pid_t> pids(maxCount, 0);

        const int pidCount = proc_listpids(PROC_ALL_PIDS, 0, pids.data(), static_cast<int>(pids.size() * sizeof(pid_t)));
        if (pidCount <= 0) {
            m_logger->logError("Error listing processes: " + std::string(std::strerror(errno)));

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
                ProcessInfo info{pid, std::string(nameBuffer)};
                processes.push_back(std::move(info));
            }
        }

        return processes;
    }
};


}