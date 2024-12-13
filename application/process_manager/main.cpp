#include <iostream>
#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <process_manager/environments/environments.h>
#include <process_manager/src/infrastructure/services/ProcessManagerService.h>
#include <process_manager/src/api/controllers/ChildProcessCommunicationController.h>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/EndpointService.h>

int main(int argc, char** argv) {
    auto logger = std::make_shared<shared::infrastructure::services::DefaultLogger>();
    if (!logger) {
        std::cout << "Logger doesn't works!";

        return 0;
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

    // <START> rest endpoints
    process_manager::api::controllers::ChildProcessCommunicationController childProcessCommunicationController{};
    restServer->registerController(childProcessCommunicationController);

    // <END> rest endpoints
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(environment::defs::Server_Url.data(), grpc::InsecureServerCredentials());

    // <START> gRPC endpoints 
    process_manager::infrastructure::services::ProcessManagerService managerService{
        std::make_unique<process_manager::infrastructure::services::ProcessManager>(logger),
        logger 
    };

    builder.RegisterService(&managerService);
    // <END> gRPC endpoints

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    if (server) {
        logger->log("Server started!");

        restServer->start();
	    server->Wait();
    }

    restServer->join();

    return 0;
}