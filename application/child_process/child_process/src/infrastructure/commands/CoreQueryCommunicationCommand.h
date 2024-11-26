#pragma once

#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>
#include <shared/src/domain/protos/core_communication.pb.h>
#include <shared/src/domain/protos/core_communication.grpc.pb.h>

namespace child_process::infrastructre::commands {

class CoreQueryCommunicationCommand {
public:
    using Sender = shared::infrastructure::commands::GenericGrpcRequestSenderCommand<
        ::core_communication::CoreQueryRequest,
        ::core_communication::CoreQueryResponse,
        ::core_communication::CoreQueryCommunicationService
    >;

    CoreQueryCommunicationCommand(const Sender::Config& config) : m_config{ config } {}

    [[nodiscard]] std::optional<::core_communication::CoreQueryResponse> sendRequest(const ::core_communication::CoreQueryRequest& requestData) const {
        Sender sender{ m_config, [](auto stub, auto context, auto request, auto response) {
            return stub->Handle(context, *request, response);
        }};
        
        return sender.sendRequest(requestData);
    }

private:
    Sender::Config m_config;
};

}