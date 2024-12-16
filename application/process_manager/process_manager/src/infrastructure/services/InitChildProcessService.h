#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <thread>
#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/domain/models/ProcessConfig.h>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/services/RequestSenderService.h>
#include <shared/src/application/utils/RandomNumberGenerator.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>
#include <process_manager/src/domain/models/ProcessInstance.h> 

namespace process_manager::infrastructure::services {

class InitChildProcessService {
public:
    // Serwis, ktory zyje dluzej powinien miec przekazywane parametry przez funkcje
    // Klasa, ktora robi cos od razu (np. converter / handler / parser ) powinna miec przekazywane parametry przez konstruktor
    InitChildProcessService(
        std::uint32_t pid,
        std::string internalId,
        shared::domain::models::ProcessConfig childConfig,
        std::shared_ptr<shared::application::services::ILogger> logger) :
            m_pid{ pid },
            m_internalId{ internalId },
            m_childConfig{ childConfig },
            m_logger{ logger }
        {
            if (!m_logger) {
                throw std::runtime_error("Logger is not initialized!");
            }
        }

    std::optional<process_manager::domain::models::ProcessInstance> init(int timeoutMs = 1000, int retries = 3) {
        const auto processInstnace = getProcessInstance();

        const auto pingMessage = shared::domain::models::PingMessage {
            .processId = m_pid,
            .uniqueNumber = shared::application::utils::RandomValueGenerator{}.generateRandomValue()
        };

        const auto pingMessageJson = shared::application::utils::ModelsJsonConverter{}.toJson(pingMessage);
        for(auto i = 0; i < retries; i++) {
            shared::infrastructure::services::RequestSenderService sender{ m_childConfig.childAddress, m_childConfig.childPort, "/init" };
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
                return processInstnace;
            }
        }

        return std::nullopt;
    }

private:
    std::uint32_t m_pid;
    std::string m_internalId;
    shared::domain::models::ProcessConfig m_childConfig;
    std::shared_ptr<shared::application::services::ILogger> m_logger;

    process_manager::domain::models::ProcessInstance getProcessInstance() {
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