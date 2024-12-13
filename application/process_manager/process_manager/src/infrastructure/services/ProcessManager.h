#pragma once

#include <cstdint>
#include <spawn.h>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <optional>

#include <boost/asio.hpp>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/EndpointService.h>

#include <process_manager/src/infrastructure/services/ProcessManager.h>

#include <environments/environments.h>

namespace process_manager::infrastructure::services {

using namespace shared::application::services;
using boost::asio::ip::tcp;

class ProcessManager {
public:
    ProcessManager(std::shared_ptr<ILogger> logger) :
        m_logger{ logger } {}

    std::optional<std::int32_t> startProcess(const std::string& program, const std::vector<std::string>& args) {
        // create a parameters
        std::vector<char*> cargs;
        cargs.reserve(args.size() + 2);
        cargs.push_back(const_cast<char*>(program.c_str()));
        for (auto &arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);


        pid_t pid{};
        int status = posix_spawn(&pid, program.data(), NULL, NULL, cargs.data(), NULL);
        if (status == 0) {
            return pid;
        }

        return std::nullopt;
    }

private:
    std::shared_ptr<ILogger> m_logger;
};

}