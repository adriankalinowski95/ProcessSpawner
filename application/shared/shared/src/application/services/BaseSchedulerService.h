#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <cstdint>
#include <functional>
#include <exception>

namespace shared::application::services {

class BaseSchedulerService {
public:    
    struct Config {
        std::uint32_t delay;
        std::function<bool()> periodicFunction;
    };

    BaseSchedulerService(const Config& config) : 
        m_config{ config },
        m_schedulerThread{},
        m_stopRequest{} {
            if (!m_config.periodicFunction) {
                throw std::runtime_error("Periodic function doesn't exist!");
            }
        }

    void start() {
        m_schedulerThread = std::thread(&BaseSchedulerService::runThread, this);
    }

    void stop() {
        m_stopRequest = true;
    }

    void join() {
        m_schedulerThread.join();
    }

private:
    Config m_config;

    std::thread m_schedulerThread;
    std::atomic<bool> m_stopRequest;

    void runThread() {
        while (!m_stopRequest) {
            // do something
            const auto result = m_config.periodicFunction();
            if (!result) {
                // @Todo logger???
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(m_config.delay));
        }
    }
};

}