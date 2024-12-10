#include <iostream>

#include <shared/src/domain/protos/communication.pb.h>
#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <process_manager/environments/environments.h>
#include <process_manager/src/infrastructure/services/ProcessManagerService.h>

#include <shared/src/infrastructure/services/DefaultLogger.h>

int main(int argc, char** argv) {
    auto logger = std::make_shared<shared::infrastructure::services::DefaultLogger>();
    if (!logger) {
        std::cout << "Logger doesn't works!!!!!!!!";

        return 0;
    }

    grpc::ServerBuilder builder;
    builder.AddListeningPort(environment::defs::Server_Url.data(), grpc::InsecureServerCredentials());

    // <START> Endpoints 
    process_manager::infrastructure::services::ProcessManagerService managerService{
        std::make_unique<process_manager::infrastructure::services::ProcessManager2>(logger),
        logger 
    };

    builder.RegisterService(&managerService);
    // <END> Endpoints

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    if (server) {
        logger->log("Server started!");

	    server->Wait();
    }

    return 0;
}