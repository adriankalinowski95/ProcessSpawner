#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <optional>

#include <spawn.h>
#include <boost/asio.hpp>

#include <shared/src/application/services/ILogger.h>

namespace process_manager::infrastructure::tools {

using boost::asio::ip::tcp;

class ProcessSpawner {
public:
    ProcessSpawner(std::shared_ptr<shared::application::services::ILogger> logger) :
        m_logger{ logger } {}

    std::optional<std::uint32_t> startProcess(const std::string& program, const std::vector<std::string>& args) {
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