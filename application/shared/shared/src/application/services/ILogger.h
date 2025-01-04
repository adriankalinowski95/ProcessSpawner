#pragma once

#include <string>

namespace shared::application::services {

class ILogger {
public:
    // Enum to represent log levels
    enum LogLevel { 
        Debug, 
        Info, 
        Warning, 
        Error, 
        Critical 
    };

    virtual ~ILogger() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual void log(LogLevel level, const std::string& tag, const std::string& message) = 0;
};

}