#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <thread>
#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/domain/models/ProcessConfig.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/services/RequestSenderCommand.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>

namespace process_manager::infrastructure::commands {

class SendPingMessageCommand {
public:
    SendPingMessageCommand(std::string internalId, std::shared_ptr<shared::application::services::ILogger> logger) :
            m_internalId{ internalId },
            m_logger{ logger }
    {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    std::optional<process_manager::domain::models::ProcessInstance> sendPingMessage(int timeoutMs = 1000, int retries = 3) {
        auto processInstnace = getProcessInstance();

        const auto pingMessage = shared::domain::models::PingMessage {
            .internalId = m_internalId,
            .uniqueNumber = shared::application::utils::RandomValueGenerator{}.generateRandomValue()
        };

        const auto pingMessageJson = shared::application::utils::ModelsJsonConverter{}.toJson(pingMessage);
        for (auto i = 0; i < retries; i++) {
            shared::infrastructure::commands::RequestSenderCommand sender{ m_childConfig.childAddress, m_childConfig.childPort, "/init" };
            const auto result = sender.sendRequest(pingMessageJson, true);
            if (!result || result->empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs));

                continue;
            }

            const auto response = shared::application::utils::ModelsJsonConverter{}.fromJson<shared::domain::models::PingMessage>(*result);
            if (!response) {
                continue;
            }

            if (response->uniqueNumber == pingMessage.uniqueNumber + 1) {
                const auto start = std::chrono::steady_clock::now();
                const auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
                processInstnace.createdTimeMs = timeInMs;
                processInstnace.lastUpdateTimeMs = timeInMs;

                return processInstnace;
            }
        }

        return std::nullopt;
    }

private:
    std::string m_internalId;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    process_manager::domain::models::ProcessInstance getPingMessage() {
        return process_manager::domain::models::ProcessInstance {
            .internalId = m_internalId,
            .processType = "child",
            .parameters = {},
            .pid = m_pid,
            .address = m_childConfig.childAddress,
            .port = m_childConfig.childPort
        };
    }
};

}