#include <future>
#include <chrono>
#include <iostream>
#include <exception>

#include <process_manager/src/infrastructure/services/ApplicationSingleton.h>
#include <process_manager/src/infrastructure/commands/ProcessManagerInputRequestCommand.h>
#include <process_manager/src/api/server/ProcessManagerServer.h>

static constexpr std::uint32_t Max_Wait_For_Server_Start = 5;

int main(int argc, char** argv) {
    auto application = process_manager::application::services::ApplicationSingleton::GetInstance();
    auto logger = application.GetLogger();
    try {
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
        process_manager::infrastructre::commands::ProcessManagerInputRequestCommand processManagerInputRequestCommand{ 
            application.GetChildProcessHolderService(), 
            application.GetChildProcessSpawnerService(),
            application.GetGlobalConfigProvider(),
            application.GetLogger(), 
        };
        if (!processManagerInputRequestCommand.loadInputProcesses(application.GetGlobalConfigProvider()->GetProcessManagerConfig().coreSideName)) {
            throw std::runtime_error("Can't load input processes");
        }

        // <CUSTOM_CODE_START>
        // <CUSTOM_CODE_END>

        server.join();
    } catch (std::exception& e) {
        logger->logError(e.what());

        // Shutdown processes 
        application.GetAssociatedProcessesTerminatorService()->terminate();
    }

    return 0;
}