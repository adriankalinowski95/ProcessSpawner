#pragma once

#include <memory>
#include <string_view>
#include <thread>
#include <future>

#include <shared/src/domain/protos/communication.pb.h>
#include <shared/src/domain/protos/communication.grpc.pb.h>
#include <shared/src/application/services/ILogger.h>

#include <process_manager/src/infrastructure/services/unix/UnixPlatformIO.h>
#include <process_manager/src/infrastructure/services/unix/UnixPlatformProcessLauncher.h>
#include <process_manager/src/infrastructure/services/ProcessManager.h>
#include <process_manager/src/infrastructure/services/ProcessFactory.h>

namespace process_manager::infrastructure::services {

using namespace process_manager::infrastructure::services::unix;

class ProcessManagerService : public Communication::ManagerService::Service {
public:
    ProcessManagerService(std::shared_ptr<shared::application::services::ILogger> logger) : 
        m_logger{ logger } {
        if (!m_logger) {
            throw std::runtime_error("Logger is not initialized!");
        }
    }

    virtual ::grpc::Status SpawnProcess(::grpc::ServerContext* context, const Communication::SpawnRequest* request, Communication::SpawnResponse* response) override {
        m_logger->log("SpawnProcess called!");
        
        auto platformLauncher = std::make_shared<UnixPlatformProcessLauncher>();
        auto platformIO = std::make_shared<UnixPlatformIO>();
        auto factory = std::make_unique<ProcessFactory>(platformLauncher, platformIO, nullptr);
        
        ProcessManager manager(std::move(factory));

        std::promise<void> shutdownPromise;
        std::future<void> shutdownFuture = shutdownPromise.get_future();

        std::thread managerThread(&ProcessManager::run, &manager, std::move(shutdownFuture));

        int pid = manager.startProcess("/bin/cat", {});
        manager.sendToProcess(pid, "Hello, process!\n");
        if (auto output = manager.readFromProcess(pid)) {
            std::cout << "Received: " << *output << std::endl;
        }

        manager.sendToProcess(pid, "Another line\n");
        if (auto output2 = manager.readFromProcess(pid)) {
            std::cout << "Received: " << *output2 << std::endl;
        }

        // Sygnalizujemy zamknięcie menedżera
        shutdownPromise.set_value();
        managerThread.join();

        //response->set_success(true);
        //response->set_message("Process spawned!");
        //response->set_process_id("1234");

        return grpc::Status::OK;
    }

    virtual ::grpc::Status StopProcess(::grpc::ServerContext* context, const Communication::StopRequest* request, Communication::StopResponse* response) override {
        m_logger->log("Stop process called!");

        return grpc::Status::OK;
    }
    
private:
    std::shared_ptr<shared::application::services::ILogger> m_logger;
};

}