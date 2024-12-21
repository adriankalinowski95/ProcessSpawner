#pragma once

#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

namespace process_manager::infrastructre::commands {

class ChildInitRequestCommand {
public:
    using Sender = shared::infrastructure::commands::GenericRequestSenderCommand<shared::domain::models::PingMessage, shared::domain::models::PingMessage>;

    ChildInitRequestCommand(const Sender::Config& config) : m_config{ config } {}

    [[nodiscard]] bool sendRequest(const shared::domain::models::PingMessage& pingMessage) const {
        Sender sender{ m_config };
        
        return sender.sendRequest(pingMessage).has_value();
    }

private:
    Sender::Config m_config;
};

}