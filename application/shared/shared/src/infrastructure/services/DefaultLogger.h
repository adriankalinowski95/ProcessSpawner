#pragma once

#include <iostream>
#include <shared/src/application/services/ILogger.h>

namespace shared::infrastructure::services {

// @Todo change this to levels diffrent priting
// Mutex and multitreading logging must be implemented
class DefaultLogger : public shared::application::services::ILogger {
public:
    void log(std::string message) {
        std::cout << message << std::endl;
    }

    void logInfo(std::string message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    void logError(std::string message) {
        std::cout << "[ERROR] " << message << std::endl;
    }
};

}