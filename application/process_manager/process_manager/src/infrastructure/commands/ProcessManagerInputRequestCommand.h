#pragma once

#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/infrastructure/commands/GenericRequestSenderCommand.h>

namespace process_manager::infrastructre::commands {

class ProcessManagerInputRequestCommand {
public:
    ProcessManagerInputRequestCommand() {

    }

    [[nodiscard]] bool loadInputProcesses(const shared::domain::models::PingMessage& pingMessage) const {
        Sender sender{ m_config };
        
        return sender.sendRequest(pingMessage).has_value();
    }
};

}