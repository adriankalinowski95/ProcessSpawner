#pragma once

#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>
#include <shared/src/domain/protos/core_communication.pb.h>
#include <shared/src/domain/protos/core_communication.grpc.pb.h>

namespace child_process::infrastructre::commands {

class CoreCommandCommunicationCommand {
public:
    using Sender = shared::infrastructure::commands::GenericGrpcRequestSenderCommand<
        ::core_communication::CoreCommandRequest,
        ::core_communication::CoreCommandResponse,
        ::core_communication::CoreCommandCommunicationService
    >;

    CoreCommandCommunicationCommand(const Sender::Config& config) : m_config{ config } {}

    [[nodiscard]] std::optional<::core_communication::CoreCommandResponse> sendRequest(const ::core_communication::CoreCommandRequest& requestData) const {
        Sender sender{ m_config, [](auto stub, auto context, auto request, auto response) {
            return stub->Handle(context, *request, response);
        }};
        
        return sender.sendRequest(requestData);
    }

private:
    Sender::Config m_config;
};

}