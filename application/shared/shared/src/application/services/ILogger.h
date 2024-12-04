#pragma once

#include <string>

namespace shared::application::services {

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(std::string message) = 0;
    virtual void logInfo(std::string message) = 0;
    virtual void logError(std::string message) = 0;  
};

}