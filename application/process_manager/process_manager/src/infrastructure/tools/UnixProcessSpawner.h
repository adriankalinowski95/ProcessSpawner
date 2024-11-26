#pragma once

#include <spawn.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/tools/IProcessSpawner.h>

namespace process_manager::infrastructure::tools {

class UnixProcessSpawner : public process_manager::application::tools::IProcessSpawner {
public:
    UnixProcessSpawner(std::shared_ptr<shared::application::services::ILogger> logger) :
        process_manager::application::tools::IProcessSpawner{},
        m_logger{ logger } 
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    [[nodiscard]] std::optional<std::uint32_t> startProcess(const std::string& program, const std::vector<std::string>& args) override {
        std::vector<char*> cargs{};
        cargs.reserve(args.size() + 2);

        // @Todo change this - old code
        cargs.push_back(const_cast<char*>(program.c_str()));
        for (auto &arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);

        pid_t pid{};
        if (posix_spawn(&pid, program.data(), NULL, NULL, cargs.data(), NULL) == 0) {
            m_logger->log(
                shared::application::services::ILogger::LogLevel::Info,
                "UNIX_PROCESS_SPAWNER", 
                "Process was spawned correctly! Pid: " + std::to_string(pid)
            );

            return pid;
        }

        m_logger->log(
            shared::application::services::ILogger::LogLevel::Error,
            "UNIX_PROCESS_SPAWNER",
            "Can't spawn process" + program
        );

        return std::nullopt;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;  
};

}