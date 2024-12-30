#pragma once

#include <shared/src/infrastructure/commands/GenericGrpcRequestSenderCommand.h>
#include <shared/src/domain/protos/child_process_communication.pb.h>
#include <shared/src/domain/protos/child_process_communication.grpc.pb.h>

namespace child_process::infrastructre::commands {

class CoreQueryCommunicationCommand {
public:
    using Sender = shared::infrastructure::commands::GenericGrpcRequestSenderCommand<
        ::child_process_communication::ChildPingRequest,
        ::child_process_communication::ChildPingResponse,
        ::child_process_communication::ChildPingService
    >;

    CoreQueryCommunicationCommand(const Sender::Config& config) : m_config{ config } {}

    [[nodiscard]] bool ping(const ::child_process_communication::ChildPingRequest& requestData) const {
        Sender sender{ m_config, [](auto stub, auto context, auto request, auto response) {
            return stub->Ping(context, *request, response);
        }};
        
        return sender.sendRequest(requestData).has_value();
    }

private:
    Sender::Config m_config;
};

}