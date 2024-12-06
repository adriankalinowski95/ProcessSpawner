#pragma once

#include <string_view>
#include <string>
#include <optional>

namespace process_manager::application::services {

class IPlatformIO {
public:
    virtual ~IPlatformIO() = default;
    virtual bool writeData(int handle, std::string_view data) = 0;
    virtual std::optional<std::string> readDataNonBlocking(int handle) = 0;
    virtual void closeHandle(int handle) = 0;
};

}