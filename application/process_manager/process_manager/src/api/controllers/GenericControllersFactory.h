#pragma once

#include <memory>
#include <string>
#include <environments/environments.h>
#include <shared/src/domain/protos/core_communication.pb.h>
#include <shared/src/domain/protos/core_communication.grpc.pb.h>
#include <shared/src/api/controllers/BaseGrpcMediatorController.h>

namespace process_manager::api::controllers {

class GenericControllersFactory {
public:
    using CoreQueryCommunicationController = shared::api::controllers::BaseGrpcMediatorController<
        core_communication::CoreQueryRequest,
        core_communication::CoreQueryResponse,
        core_communication::CoreQueryCommunicationService
    >;

    using CoreCommandCommunicationController = shared::api::controllers::BaseGrpcMediatorController<
        core_communication::CoreCommandRequest,
        core_communication::CoreCommandResponse,
        core_communication::CoreCommandCommunicationService
    >;

    static std::unique_ptr<CoreQueryCommunicationController> createCoreQueryCommunicationController(
        std::shared_ptr<shared::application::services::ILogger> logger) {
        
        const auto serverUrl = environment::defs::Backend_Url;
        const auto colonPosition = serverUrl.find(':');
        if (colonPosition == std::string::npos) {
            throw std::runtime_error("Invalid server url");
        }

        const auto address = serverUrl.substr(0, colonPosition);
        const auto port = std::stoi(serverUrl.substr(colonPosition + 1).data());

        return std::make_unique<CoreQueryCommunicationController>(
            CoreQueryCommunicationController::Config {
                .address = address.data(),
                .port = (std::uint32_t)port
            }, logger);
    }

    static std::unique_ptr<CoreCommandCommunicationController> createCoreCommandCommunicationController(
        std::shared_ptr<shared::application::services::ILogger> logger) {
        
        const auto serverUrl = environment::defs::Backend_Url;
        const auto colonPosition = serverUrl.find(':');
        if (colonPosition == std::string::npos) {
            throw std::runtime_error("Invalid server url");
        }

        const auto address = std::string(serverUrl.substr(0, colonPosition));
        const auto port = std::stoi(serverUrl.substr(colonPosition + 1).data());

        return std::make_unique<CoreCommandCommunicationController>(
            CoreCommandCommunicationController::Config {
                .address = address,
                .port = (std::uint32_t)port
            }, logger);
    }
};

}