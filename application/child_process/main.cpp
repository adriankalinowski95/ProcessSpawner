#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include <grpc/grpc.h> 
#include <grpcpp/server_builder.h>

#include <child_process/src/application/utils/ChildProcessParamsParser.h>
#include <child_process/src/application/services/GlobalConfig.h>

#include <child_process/src/api/controllers/ChildInitController.h>

#include <child_process/src/infrastructure/services/PingManagerSchedulerService.h>
#include <child_process/src/infrastructure/services/EventProviderSchedulerService.h>
#include <child_process/src/infrastructure/services/CoreQueryParamsService.h>

#include <shared/src/infrastructure/commands/RequestSenderCommand.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <shared/src/infrastructure/services/EndpointService.h>

using boost::asio::ip::tcp;

void setTemporaryParams(int* argc, char*** argv) {
    std::string params = "{\"internalId\":\"abcdefg\", \"childAddress\":\"127.0.0.1\",\"childPort\":8085,\"parentAddress\":\"127.0.0.1\",\"parentPort\":50051}";

    *argc = 2;
    *argv = new char*[*argc];

    (*argv)[0] = new char[256];
    (*argv)[0] = "child_process";

    (*argv)[1] = new char[params.size()];
    memcpy((*argv)[1], params.data(), params.size());
}

void setTempData(shared::domain::models::ProcessConfig& config) {
    shared::infrastructure::commands::RequestSenderCommand sender{ config.parentAddress, config.parentPort, "/fun1" };
    const auto result = sender.sendRequest("Hello, server!" , true);
    if (result) {
            std::cout << "Hello server result: " << *result << std::endl;
    } else {
        std::cout << "Didn't recive a hello server result" << std::endl;
    }
}

void initGrpc(std::shared_ptr<shared::application::services::ILogger> logger) {
    const auto config = child_process::application::services::GlobalConfig::getInstance().getProcessConfig();
    
    grpc::ServerBuilder builder;
    const auto serverUrl = config->childAddress + ":" + std::to_string(config->childPort);
    builder.AddListeningPort(serverUrl.data(), grpc::InsecureServerCredentials());

    child_process::api::controllers::ChildInitController childInitController{ logger };

    // <START> gRPC endpoints 
    builder.RegisterService(&childInitController);
    // <END> gRPC endpoints
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    if (!server) {
        throw std::runtime_error("Can't create gRPC server");
    }
    
    if (server) {
        logger->log("Server started!");
	    server->Wait();
    }
}


int main(int argc, char** argv) {
    // setTemporaryParams(&argc, &argv);
    auto logger = std::make_shared<shared::infrastructure::services::DefaultLogger>();
    try {
        const auto params = child_process::application::utils::ChildProcessParamsParser::GetParams(argc, argv);
        if (params.empty()) {
            std::cerr << "Bad parameters" << std::endl;
            
            return 1;
        }
        
        const auto config = child_process::application::utils::ChildProcessParamsParser::ParseConfig(params);
        if (!config) {
            std::cerr << "Not correct configuration" << std::endl;
            
            return 1;
        }

        child_process::application::services::GlobalConfig::getInstance().setProcessConfig(*config);

        // Scheduler
        child_process::infrastructure::services::PingManagerSchedulerService pingManagerSchedulerService{ logger };
        pingManagerSchedulerService.start();

        child_process::infrastructure::services::EventProviderSchedulerService eventProviderSchedulerService{ logger };
        eventProviderSchedulerService.start();

        initGrpc(logger);
    }
    catch (std::exception& e) {
        logger->logError(e.what());
    }

    return 0;
}