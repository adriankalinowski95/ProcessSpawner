#include <iostream>
#include <exception>
#include <algorithm>
#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <process_manager/environments/environments.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/ProcessManagerService.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/services/ProcessQueryService.h>
#include <process_manager/src/infrastructure/services/UnixProcessEnumerator.h>
#include <process_manager/src/infrastructure/services/UnixProcessTerminator.h>

#include <process_manager/src/api/controllers/ChildProcessCommunicationController.h>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/EndpointService.h>
#include <shared/src/infrastructure/providers/UnixProcessInfoProvider.h>

int main(int argc, char** argv) {
    try{ 
        // @Todo make a factory class
        auto logger = std::make_shared<shared::infrastructure::services::DefaultLogger>();
        if (!logger) {
            throw std::runtime_error("Can't create logger or process manager");
        }

        auto processEnumerator = std::make_shared<process_manager::infrastructure::services::UnixProcessEnumerator>(logger);
        if (!processEnumerator) {
            throw std::runtime_error("Can't create process enumerator");
        }

        auto processTerminator = std::make_shared<process_manager::infrastructure::services::UnixProcessTerminator>(logger);
        if (!processTerminator) {
            throw std::runtime_error("Can't create process terminator");
        }

        // @Todo move this to separated class / method

        // kill all child processes
        processTerminator->terminateAll(processEnumerator->enumerateWhereNameEquals("child_process"));

        // kill all process manager processes
        auto processManagerProcesses = processEnumerator->enumerateWhereNameEquals("process_manager");
        if (processManagerProcesses.size() > 1) {
            const auto currentPid = shared::infrastructure::providers::UnixProcessInfoProvider{}.getPid();
            const auto end = std::remove_if(processManagerProcesses.begin(), processManagerProcesses.end(), [currentPid](process_manager::domain::models::ProcessInfo& processInfo) {
                return processInfo.pid == currentPid;
            });

            processManagerProcesses.erase(end, processManagerProcesses.end());

            processTerminator->terminateAll(processEnumerator->enumerateWhereNameEquals("process_manager"));
        }

        auto processSpawner = std::make_shared<process_manager::infrastructure::tools::ProcessSpawner>(logger);
        if (!processSpawner) {
            throw std::runtime_error("Can't create process spawner");
        }

        auto childProcessHolderService = std::make_shared<process_manager::infrastructure::services::ChildProcessHolderService>();
        if (!childProcessHolderService) {
            throw std::runtime_error("Can't create child process holder service");
        }

        auto restServer = std::make_unique<shared::infrastructure::services::AsyncServerService>(
            environment::parent_process::Address.data(), 
            environment::parent_process::Port, 
            std::make_unique<shared::infrastructure::services::EndpointService>(
                environment::parent_process::Address.data(), 
                logger
            ),
            logger
        );
        if (!restServer) {
            throw std::runtime_error("Can't create rest server");
        }

        // <START> rest endpoints
        process_manager::api::controllers::ChildProcessCommunicationController childProcessCommunicationController{ childProcessHolderService, logger};
        restServer->registerController(childProcessCommunicationController);
        // <END> rest endpoints

        grpc::ServerBuilder builder;
        builder.AddListeningPort(environment::defs::Server_Url.data(), grpc::InsecureServerCredentials());

        // <START> gRPC endpoints 
        process_manager::infrastructure::services::ProcessManagerService managerService{
            processSpawner,
            childProcessHolderService,
            logger 
        };

        process_manager::infrastructure::services::ProcessQueryService queryService{
            childProcessHolderService,
            logger
        };

        builder.RegisterService(&managerService);
        builder.RegisterService(&queryService);
        // <END> gRPC endpoints

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        if (!server) {
            throw std::runtime_error("Can't create gRPC server");
        }

        if (server) {
            logger->log("Server started!");
            restServer->start();
	        server->Wait();
        }

        restServer->join();
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}