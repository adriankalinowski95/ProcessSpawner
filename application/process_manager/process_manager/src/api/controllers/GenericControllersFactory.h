#pragma once

#include <memory>
#include <string>
#include <environments/environments.h>
#include <shared/src/domain/protos/core_communication.pb.h>
#include <shared/src/domain/protos/core_communication.grpc.pb.h>
#include <shared/src/api/controllers/BaseGrpcMediatorController.h>

#include <process_manager/src/infrastructure/services/ChildProcessSpawnerService.h>

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
        auto globalConfig = process_manager::application::services::ApplicationSingleton::GetInstance().GetGlobalConfigProvider();
        
        return std::make_unique<CoreQueryCommunicationController>(
            CoreQueryCommunicationController::Config {
                .address = globalConfig->GetCoreServerConfig().endpoint.ip,
                .port = globalConfig->GetCoreServerConfig().endpoint.port,
            }, logger);
    }

    static std::unique_ptr<CoreCommandCommunicationController> createCoreCommandCommunicationController(
        std::shared_ptr<shared::application::services::ILogger> logger) {
        auto globalConfig = process_manager::application::services::ApplicationSingleton::GetInstance().GetGlobalConfigProvider();

        return std::make_unique<CoreCommandCommunicationController>(
            CoreCommandCommunicationController::Config {
                .address = globalConfig->GetCoreServerConfig().endpoint.ip,
                .port = globalConfig->GetCoreServerConfig().endpoint.port
            }, logger);
    }
};

}