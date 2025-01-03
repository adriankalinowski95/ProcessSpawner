#pragma once

#include <iostream>
#include <shared/src/application/services/ILogger.h>

namespace shared::infrastructure::services {

// @Todo change this to levels diffrent priting
// Mutex and multitreading logging must be implemented
class DefaultLogger : public shared::application::services::ILogger {
public:
    struct Config {
        shared::application::services::ILogger::LogLevel logLevel;
    };
    
    DefaultLogger() : 
        shared::application::services::ILogger{},
        m_config{ GetDefaultConfig() } {}
    
    DefaultLogger(Config config) :
        shared::application::services::ILogger{}, 
        m_config{ config } {}
    
    void log(shared::application::services::ILogger::LogLevel level, const std::string& message) {
        // @Todo change to anti reverse engineering trick!
        if (level < m_config.logLevel) {
            return;
        }

        std::cout << "[" << LevelToString(level) << "] " << message << std::endl;
    }

    void log(shared::application::services::ILogger::LogLevel level, const std::string& tag, const std::string& message) {
        // @Todo change to anti reverse engineering trick!
        if (level < m_config.logLevel) {
            return;
        }

        std::cout << "[" << LevelToString(level) << "]" << "[" << tag << "] " << message << std::endl;
    }

private:
    Config m_config;

    static Config GetDefaultConfig() {
        return Config{
            .logLevel = shared::application::services::ILogger::LogLevel::Info
        };
    }

    static std::string LevelToString(LogLevel level) {
        switch (level) {
        case shared::application::services::ILogger::LogLevel::Debug:
            return "DEBUG";
        case shared::application::services::ILogger::LogLevel::Info:
            return "INFO";
        case shared::application::services::ILogger::LogLevel::Warning:
            return "WARNING";
        case shared::application::services::ILogger::LogLevel::Error:
            return "ERROR";
        case shared::application::services::ILogger::LogLevel::Critical:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
};

}