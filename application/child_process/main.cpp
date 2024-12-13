#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include <child_process/src/application/utils/ChildProcessParamsParser.h>

#include <shared/src/infrastructure/services/RequestSenderService.h>
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

int main(int argc, char** argv) {
    setTemporaryParams(&argc, &argv);
    
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

        shared::infrastructure::services::RequestSenderService sender{ config->parentAddress, config->parentPort, "/fun1" };
        const auto result = sender.sendRequest("Hello, server!", true);
        if (result) {
            std::cout << "Hello server result: " << *result << std::endl;
        } else {
            std::cout << "Didn't recive a hello server result" << std::endl;
        }

        shared::infrastructure::services::AsyncServerService server { 
            config->childAddress, 
            config->childPort, 
            std::make_unique<shared::infrastructure::services::EndpointService>(config->childAddress.data(), logger),
            logger
        };
        
        server.start();
        server.join();
    }
    catch (std::exception& e) {
        logger->logError(e.what());
    }

    return 0;
}