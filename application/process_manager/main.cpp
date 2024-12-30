#include <iostream>
#include <exception>
#include <algorithm>
#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <process_manager/environments/environments.h>
#include <process_manager/src/application/providers/ChildProcessConfigProvider.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/infrastructure/services/ChildProcessSpawnerService.h>
#include <process_manager/src/infrastructure/services/UnixProcessEnumerator.h>
#include <process_manager/src/infrastructure/services/UnixProcessTerminator.h>
#include <process_manager/src/infrastructure/commands/ProcessManagerInputRequestCommand.h>

#include <process_manager/src/api/controllers/ProcessQueryController.h>
#include <process_manager/src/api/controllers/ChildPingController.h>
#include <process_manager/src/api/controllers/ProcessManagerController.h>
#include <process_manager/src/api/controllers/GenericControllersFactory.h>

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

        auto childProcessConfigProvider = std::make_shared<process_manager::application::utils::ChildProcessConfigProvider>(
            environment::child_process::Address.data(), 
            environment::child_process::Port,
            environment::parent_process::Address.data(), 
            environment::parent_process::Port
        );
        if (!childProcessConfigProvider) {
            throw std::runtime_error("Can't create child process config provider");
        }

        auto processSpawner = std::make_shared<process_manager::infrastructure::tools::ProcessSpawner>(logger);
        if (!processSpawner) {
            throw std::runtime_error("Can't create process spawner");
        }

        auto childProcessHolderService = std::make_shared<process_manager::infrastructure::services::ChildProcessHolderService>();
        if (!childProcessHolderService) {
            throw std::runtime_error("Can't create child process holder service");
        }

        auto childProcessSpawnerService = std::make_shared<process_manager::infrastructure::services::ChildProcessSpawnerService>(
            processSpawner,
            childProcessConfigProvider,
            logger
        );
        if (!childProcessSpawnerService) {
            throw std::runtime_error("Can't create child process spawner service");
        }

        process_manager::infrastructre::commands::ProcessManagerInputRequestCommand processManagerInputRequestCommand{ 
            childProcessHolderService, 
            childProcessSpawnerService,
            logger 
        };
        if (!processManagerInputRequestCommand.loadInputProcesses(environment::parent_process::Default_Process_Manager_Name.data())) {
            throw std::runtime_error("Can't load input processes");
        }

        grpc::ServerBuilder builder;
        builder.AddListeningPort(environment::defs::Server_Url.data(), grpc::InsecureServerCredentials());

        // <START> gRPC endpoints 
        process_manager::api::controllers::ProcessManagerController managerController{
            childProcessSpawnerService,
            childProcessHolderService,
            processTerminator,
            logger 
        };

        process_manager::api::controllers::ProcessQueryController queryController{
            childProcessHolderService,
            logger
        };

        process_manager::api::controllers::ChildPingController pingController{ 
            childProcessHolderService, 
            logger 
        };

        auto coreQueryCommunicationController = process_manager::api::controllers::GenericControllersFactory::createCoreQueryCommunicationController(logger);
        auto coreCommandCommunicationController = process_manager::api::controllers::GenericControllersFactory::createCoreCommandCommunicationController(logger);

        builder.RegisterService(&managerController);
        builder.RegisterService(&queryController);
        builder.RegisterService(&pingController);
        builder.RegisterService(coreQueryCommunicationController.get());
        builder.RegisterService(coreCommandCommunicationController.get());
        // <END> gRPC endpoints

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        if (!server) {
            throw std::runtime_error("Can't create gRPC server");
        }

        logger->log("Server started!");
	    server->Wait();
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}