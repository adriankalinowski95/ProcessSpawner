#include <iostream>
#include <exception>
#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <process_manager/environments/environments.h>
#include <process_manager/src/infrastructure/tools/ProcessSpawner.h>
#include <process_manager/src/infrastructure/services/ProcessManagerService.h>
#include <process_manager/src/infrastructure/services/ChildProcessHolderService.h>
#include <process_manager/src/api/controllers/ChildProcessCommunicationController.h>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/EndpointService.h>

int main(int argc, char** argv) {
    try{ 
        auto logger = std::make_shared<shared::infrastructure::services::DefaultLogger>();
        if (!logger) {
            throw std::runtime_error("Can't create logger or process manager");
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
        builder.RegisterService(&managerService);
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