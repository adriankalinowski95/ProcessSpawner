#pragma once

#include <spawn.h>

#include <shared/src/application/services/ILogger.h>
#include <process_manager/src/application/tools/IProcessSpawner.h>

namespace process_manager::infrastructure::tools {

class UnixProcessSpawner : public process_manager::application::tools::IProcessSpawner {
public:
    UnixProcessSpawner(std::shared_ptr<shared::application::services::ILogger> logger) :
        process_manager::application::tools::IProcessSpawner{},
        m_logger{ logger } {}

    [[nodiscard]] std::optional<std::uint32_t> startProcess(const std::string& program, const std::vector<std::string>& args) override {
        std::vector<char*> cargs{};
        cargs.reserve(args.size() + 2);

        cargs.push_back(const_cast<char*>(program.c_str()));
        for (auto &arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);

        pid_t pid{};
        if (posix_spawn(&pid, program.data(), NULL, NULL, cargs.data(), NULL) == 0) {
            return pid;
        }

        return std::nullopt;
    }

private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;  
};

}