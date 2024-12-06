#pragma once

#include <string_view>
#include <string>
#include <optional>

namespace process_manager::application::services {

class IProcess {
public:
    virtual ~IProcess() = default;
    virtual int getProcessId() const = 0;
    virtual bool sendData(std::string_view data) = 0;
    virtual std::optional<std::string> getNextOutput() = 0;
    virtual void terminate() = 0;
};

}