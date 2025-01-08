#include <future>
#include <chrono>
#include <iostream>
#include <exception>

#include <process_manager/src/application/utils/ProcessManagerParamsParser.h>
#include <process_manager/src/infrastructure/services/ApplicationSingleton.h>
#include <process_manager/src/infrastructure/commands/ProcessManagerInputRequestCommand.h>
#include <process_manager/src/api/server/ProcessManagerServer.h>

static constexpr std::uint32_t Max_Wait_For_Server_Start = 5;

int main(int argc, char** argv) {
    // @Todo Set parameteres for server and name here!
    auto application = process_manager::application::services::ApplicationSingleton::GetInstance();
    auto logger = application.GetLogger();
    logger->start();

    try {
        const auto params = process_manager::application::utils::ProcessManagerParamsParser::GetParams(argc, argv);
        if (!params.empty()) {
            logger->log(shared::application::services::ILogger::LogLevel::Debug, "PROCESS_MANAGER", "Params: " + params[1]);
            const auto config = process_manager::application::utils::ProcessManagerParamsParser::ParseConfig(params);
            if (config) {
                application.GetGlobalConfigProvider()->setProcessManagerConfig(*config);
            }
        }

        // Shutdown old processes 
        application.GetAssociatedProcessesTerminatorService()->terminate();

        // Run server thread
        process_manager::api::server::ProcessManagerServer server{};
        server.run();

        const auto isServerStatusReady = server.getServerStaredStatus().wait_for(std::chrono::seconds(Max_Wait_For_Server_Start)) == std::future_status::ready;
        if (!isServerStatusReady) {
            throw std::runtime_error("Server didn't start in time!");
        }

        // Load input processes
        auto processManagerInputRequestCommand = application.GetProcessManagerCommandFactory()->createProcessManagerInputRequestCommand();
        if (!processManagerInputRequestCommand->loadInputProcesses(application.GetGlobalConfigProvider()->GetProcessManagerConfig().coreSideName)) {
            throw std::runtime_error("Can't load input processes");
        }

        // <CUSTOM_CODE_START>
        // <CUSTOM_CODE_END>

        server.join();
    } catch (const std::exception& e) {
        logger->log(shared::application::services::ILogger::LogLevel::Error, e.what());
    }
    
    logger->stop();

    // Shutdown processes 
    application.GetAssociatedProcessesTerminatorService()->terminate();

    return 0;
}