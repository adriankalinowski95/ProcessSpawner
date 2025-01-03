#pragma once

#include <memory>
#include <child_process/src/application/providers/GlobalConfigProvider.h>
#include <child_process/src/infrastructure/commands/ChildPingRequestCommand.h>
#include <child_process/src/infrastructure/commands/CoreCommandCommunicationCommand.h>
#include <child_process/src/infrastructure/commands/CoreQueryCommunicationCommand.h>

namespace child_process::infrastructure::commands {

class ChildProcessCommandsFactory {
    static constexpr std::uint32_t Child_Ping_Request_Command_Request_Retires = 5;
    static constexpr std::uint32_t Child_Ping_Request_Delay_Between_Retries = 1000;
    static constexpr std::uint32_t Core_Command_Communication_Command_Request_Retires = 2;
    static constexpr std::uint32_t Core_Command_Communication_Command_Delay_Between_Retries = 10000; // ms
    static constexpr std::uint32_t Core_Query_Communication_Command_Request_Retires = 2;
    static constexpr std::uint32_t Core_Query_Communication_Command_Delay_Between_Retries = 10000; // ms

    using ChildPingRequestCommand = child_process::infrastructre::commands::ChildPingRequestCommand;
    using CoreCommandCommunicationCommand = child_process::infrastructre::commands::CoreCommandCommunicationCommand;
    using CoreQueryCommunicationCommand = child_process::infrastructre::commands::CoreQueryCommunicationCommand;
public:
    ChildProcessCommandsFactory(std::shared_ptr<child_process::application::providers::GlobalConfigProvider> globalConfigProvider) : 
        m_globalConfigProvider{ globalConfigProvider } {}

    [[nodiscard]] std::shared_ptr<ChildPingRequestCommand> createChildPingRequestCommand() const {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return nullptr;
        }

        ChildPingRequestCommand::Sender::Config config {
            processConfig->parentAddress,
            processConfig->parentPort,
            Child_Ping_Request_Command_Request_Retires,
            Child_Ping_Request_Delay_Between_Retries,
            [] (const ::child_process_communication::ChildPingResponse& output) -> bool {
                return output.success();
            }
        };

        return std::make_shared<ChildPingRequestCommand>(config);
    }

    [[nodiscard]] std::shared_ptr<CoreCommandCommunicationCommand> createCoreCommandCommunicationCommand() const {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return nullptr;
        }

        CoreCommandCommunicationCommand::Sender::Config config {
            processConfig->parentAddress,
            processConfig->parentPort,
            Core_Command_Communication_Command_Request_Retires,
            Core_Command_Communication_Command_Delay_Between_Retries,
            [] (const ::core_communication::CoreCommandResponse& output) -> bool {
                return output.result().success();
            }
        };

        return std::make_shared<CoreCommandCommunicationCommand>(config);
    }

    [[nodiscard]] std::shared_ptr<CoreQueryCommunicationCommand> createCoreQueryCommunicationCommand() const {
        const auto processConfig = m_globalConfigProvider->getProcessConfig();
        if (!processConfig) {
            return nullptr;
        }

        CoreQueryCommunicationCommand::Sender::Config config {
            processConfig->parentAddress,
            processConfig->parentPort,
            Core_Query_Communication_Command_Request_Retires,
            Core_Query_Communication_Command_Delay_Between_Retries,
            [] (const ::core_communication::CoreQueryResponse& output) -> bool {
                return output.result().success();
            }
        };

        return std::make_shared<CoreQueryCommunicationCommand>(config);
    }

private:
    std::shared_ptr<child_process::application::providers::GlobalConfigProvider> m_globalConfigProvider;
};

}