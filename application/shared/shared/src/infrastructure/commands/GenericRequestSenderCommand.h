#pragma once

#include <thread>
#include <chrono>
#include <string>
#include <cstdint>
#include <optional>
#include <shared/src/infrastructure/commands/RequestSenderCommand.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>

// UNUSED!

namespace shared::infrastructure::commands {

template <typename Input, typename Output>
class GenericRequestSenderCommand {
public:
    struct Config {
        std::string address;
        std::uint32_t port;
        std::string path;
        std::uint32_t retries = 3;
        std::uint32_t delayBetweenRetries = 1000;
        std::function<bool(Output)> validationFunction = nullptr;
    };

    GenericRequestSenderCommand(const Config& config) : m_config{ config } {}

    // @Todo make a requires to check that input/output have defined a json converter.
    std::optional<Output> sendRequest(Input inputObj) {
        const auto requestFunction = getRequestFunction();
        for (auto i = 0; i < m_config.retries; i++) {
            const auto result = requestFunction(inputObj);
            if (!result) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_config.delayBetweenRetries));

                continue;
            }

            if (!m_config.validationFunction) {
                return *result;
            }

            if (m_config.validationFunction(*result)) {
                return *result;
            }
        }

        return std::nullopt;
    }

private:
    Config m_config;

    std::function<std::optional<Output>(Input)> getRequestFunction() {
        return [this](Input input) -> std::optional<Output> {
            shared::application::utils::ModelsJsonConverter jsonConverter{};
            const auto inputJson = jsonConverter.toJson(input);
            shared::infrastructure::commands::RequestSenderCommand sender{ m_config.address, m_config.port, m_config.path };

            const auto result = sender.sendRequest(inputJson, true);
            if (!result || result->empty()) {
                return std::nullopt;
            }

            return shared::application::utils::ModelsJsonConverter{}.fromJson<Output>(*result);
        };
    }
};

}
