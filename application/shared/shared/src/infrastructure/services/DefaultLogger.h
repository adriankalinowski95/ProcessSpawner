#pragma once

#include <iostream>
#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>
#include <chrono>

#include <shared/src/application/services/ILogger.h>

namespace shared::infrastructure::services {

// @Todo change this to levels diffrent priting
// Mutex and multitreading logging must be implemented
class DefaultLogger : public shared::application::services::ILogger {
    static constexpr std::uint32_t Waiting_Timeout = 1000;
public:
    struct Config {
        shared::application::services::ILogger::LogLevel logLevel;
    };
    
    DefaultLogger() : 
        shared::application::services::ILogger{},
        m_config{ GetDefaultConfig() },
        m_thread{},
        m_mutex{},
        m_stopRequest{},
        m_messages{},
        m_condition{} {}
    
    DefaultLogger(Config config) :
        shared::application::services::ILogger{}, 
        m_config{ config },
        m_thread{},
        m_mutex{},
        m_stopRequest{},
        m_messages{},
        m_condition{} {}

    ~DefaultLogger() override {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void start() override {
        m_stopRequest = false;
        run();
    }

    void stop() override {
        m_stopRequest = true;
    }
    
    void log(shared::application::services::ILogger::LogLevel level, const std::string& message) override {
        std::scoped_lock lock(m_mutex);
        
        // @Todo change to anti reverse engineering trick!
        if (level < m_config.logLevel) {
            return;
        }

        const auto fullMesage = "[" + LevelToString(level) + "] " + message;
        m_messages.push_back(fullMesage);
    }

    void log(shared::application::services::ILogger::LogLevel level, const std::string& tag, const std::string& message) override {
        std::scoped_lock lock(m_mutex);

        // @Todo change to anti reverse engineering trick!
        if (level < m_config.logLevel) {
            return;
        }

        const auto fullMesage = "[" + LevelToString(level) + "]" + "[" + tag+  "] " + message;
        m_messages.push_back(fullMesage);
    }

private:
    Config m_config;
    std::thread m_thread;
    std::mutex m_mutex;
    std::atomic<bool> m_stopRequest;
    std::deque<std::string> m_messages;
    std::condition_variable m_condition;

    void run() {
        m_thread = std::thread(&DefaultLogger::threadFun, this);
    }

    void threadFun() {
        while (!m_stopRequest) {
            const auto result = waitAndPopAll(Waiting_Timeout);
            if (!result) {
                continue;
            }

            printMessages(*result);
        }
    }

    void printMessages(const std::vector<std::string>& messages) {
        for (const auto& message : messages) {
            std::cout << message << std::endl;
        }
    }

    std::optional<std::string> waitAndPop(std::uint32_t timeoutMs) {
		std::unique_lock<std::mutex> lock(m_mutex);
		auto result = m_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this]() {
			return !m_messages.empty();
		});

		if (!result) {
			return std::nullopt;
		}

		auto message = m_messages.front();
		m_messages.pop_front();

		return message;
	}

    std::optional<std::vector<std::string>> waitAndPopAll(std::uint32_t timeoutMs) {
		std::unique_lock<std::mutex> lock(m_mutex);
		auto result = m_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this]() {
			return !m_messages.empty();
		});

		if (!result) {
			return std::nullopt;
		}

		std::vector<std::string> dataOut{};
		std::ranges::transform(m_messages, std::back_inserter(dataOut), [](std::string& v) {
			return v;
		});

		m_messages.clear();

		return dataOut;
	}

	bool isEmpty() {
		std::scoped_lock lock{ m_mutex };

		return m_messages.empty();
	}

    static Config GetDefaultConfig() {
        return Config{
            .logLevel = shared::application::services::ILogger::LogLevel::Debug
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