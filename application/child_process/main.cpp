#include <string>

#include <shared/src/infrastructure/commands/RequestSenderCommand.h>

#include <child_process/src/application/utils/ChildProcessParamsParser.h>
#include <child_process/src/infrastructure/services/ApplicationSingleton.h>
#include <child_process/src/api/server/ChildProcessServer.h>

// For testing
void setTemporaryParams(int* argc, char*** argv) {
    std::string params = "{\"internalId\":\"abcdefg\", \"childAddress\":\"127.0.0.1\",\"childPort\":8085,\"parentAddress\":\"127.0.0.1\",\"parentPort\":50051}";

    *argc = 2;
    *argv = new char*[*argc];

    (*argv)[0] = new char[256];
    (*argv)[0] = "child_process";

    (*argv)[1] = new char[params.size()];
    memcpy((*argv)[1], params.data(), params.size());
}

// Example for sending a rest request
/*
void setTempData(shared::domain::models::ProcessConfig& config) {
    shared::infrastructure::commands::RequestSenderCommand sender{ config.parentAddress, config.parentPort, "/fun1" };
    const auto result = sender.sendRequest("Hello, server!" , true);
    if (result) {
            std::cout << "Hello server result: " << *result << std::endl;
    } else {
        std::cout << "Didn't recive a hello server result" << std::endl;
    }
}
*/

static constexpr std::uint32_t Max_Wait_For_Server_Start = 5;

int main(int argc, char** argv) {
    //setTemporaryParams(&argc, &argv);

    auto application = child_process::application::services::ApplicationSingleton::GetInstance();
    auto logger = application.GetLogger();
    try {
        const auto params = child_process::application::utils::ChildProcessParamsParser::GetParams(argc, argv);
        if (params.empty()) {
            throw std::runtime_error("Bad parameters");
        }
        
        const auto config = child_process::application::utils::ChildProcessParamsParser::ParseConfig(params);
        if (!config) {
            throw std::runtime_error("Not correct configuration");
        }

        application.GetGlobalConfigProvider()->setProcessConfig(*config);

        // Start server thread
        child_process::api::server::ChildProcessServer server{};
        server.run();

        const auto isServerStatusReady = server.getServerStaredStatus().wait_for(std::chrono::seconds(Max_Wait_For_Server_Start)) == std::future_status::ready;
        if (!isServerStatusReady) {
            throw std::runtime_error("Server didn't start in time!");
        }

        // <CUSTOM_CODE_START>
        // <CUSTOM_CODE_END>

        server.join();
    }
    catch (std::exception& e) {
        logger->logError(e.what());
    }

    return 0;
}