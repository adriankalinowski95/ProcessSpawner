#pragma once

#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>
#include <shared/src/domain/protos/child_process_communication.pb.h>
#include <shared/src/domain/protos/child_process_communication.grpc.pb.h>

namespace process_manager::infrastructre::commands {

class ChildInitRequestCommand {
public:
    using Sender = shared::infrastructure::commands::GenericGrpcRequestSenderCommand<
        ::child_process_communication::ChildInitRequest,
        ::child_process_communication::ChildInitResponse,
        ::child_process_communication::ChildInitService
    >;

    ChildInitRequestCommand(const Sender::Config& config) : m_config{ config } {}

    [[nodiscard]] bool sendRequest(const ::child_process_communication::ChildInitRequest& initRequest) const {
        Sender sender{ m_config, [](auto stub, auto context, auto request, auto response) {
            return stub->InitChild(context, *request, response);
        }};
        
        return sender.sendRequest(initRequest).has_value();
    }

private:
    Sender::Config m_config;
};

}