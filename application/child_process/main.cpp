#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include <child_process/src/application/utils/ChildProcessParamsParser.h>
#include <child_process/src/application/services/GlobalConfig.h>
#include <child_process/src/api/controllers/ProcessManagerCommunicationController.h>
#include <child_process/src/infrastructure/services/PingManagerSchedulerService.h>

#include <shared/src/infrastructure/commands/RequestSenderCommand.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>
#include <shared/src/infrastructure/services/DefaultLogger.h>
#include <shared/src/infrastructure/services/EndpointService.h>

using boost::asio::ip::tcp;

void setTemporaryParams(int* argc, char*** argv) {
    std::string params = "{\"childAddress\":\"127.0.0.1\",\"childPort\":8085,\"parentAddress\":\"127.0.0.1\",\"parentPort\":8080}";

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

        shared::infrastructure::services::AsyncServerService restServer { 
            config->childAddress, 
            config->childPort, 
            std::make_unique<shared::infrastructure::services::EndpointService>(config->childAddress.data(), logger),
            logger
        };

        // <START> rest endpoints
        child_process::api::controllers::ProcessManagerCommunicationController processManagerCommunicationController{};
        restServer.registerController(processManagerCommunicationController);
        // <END> rest endpoints

        restServer.start();
        
        // Scheduler
        child_process::infrastructure::services::PingManagerSchedulerService pingManagerSchedulerService{ logger };
        pingManagerSchedulerService.startAndJoin();
        
        restServer.join();
    }
    catch (std::exception& e) {
        logger->logError(e.what());
    }

    return 0;
}