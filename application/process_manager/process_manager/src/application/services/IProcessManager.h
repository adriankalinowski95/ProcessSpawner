#pragma once

#include <string>
#include <vector>
#include <future>
#include <optional>
#include <string_view>

namespace process_manager::application::services {

class IProcessManager {
public:
    virtual ~IProcessManager() = default;

    virtual int startProcess(const std::string &program, const std::vector<std::string> &args) = 0;
    virtual bool sendToProcess(int processId, std::string_view data) = 0;
    virtual std::optional<std::string> readFromProcess(int processId) = 0;
    virtual void terminateProcess(int processId) = 0;
    virtual void run(std::future<void> shutdownSignal) = 0;
};

}