#pragma once

#include <future>
#include <thread>
#include <chrono>
#include <memory>

#include <child_process/src/infrastructure/services/ApplicationSingleton.h>
#include <child_process/src/api/controllers/ChildInitController.h>

namespace child_process::api::server {

class ChildProcessServer {
public:
    ChildProcessServer() :
        m_thread{},
        m_promise{},
        m_future{ m_promise.get_future() } {}

    ~ChildProcessServer() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void run() {
        m_thread = std::thread(&ChildProcessServer::threadFun, this);
    }

    void join() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    std::future<bool>& getServerStaredStatus() {
        return m_future;
    }

private:
    std::thread m_thread;
    std::promise<bool> m_promise;
    std::future<bool> m_future;

    void threadFun() {
        const auto globalConfig = child_process::application::services::ApplicationSingleton::GetInstance().GetGlobalConfigProvider();
        const auto processConfig = globalConfig->getProcessConfig();
        if (!processConfig) {
            m_promise.set_value(false);

            return;
        }

        auto logger = child_process::application::services::ApplicationSingleton::GetInstance().GetLogger();

        // <START> gRPC controllers
        child_process::api::controllers::ChildInitController childInitController{
            child_process::application::services::ApplicationSingleton::GetInstance().GetEventProviderSchedulerService(),
            child_process::application::services::ApplicationSingleton::GetInstance().GetPingManagerSchedulerService(),
            logger
        };
        // <END> gRPC controllers

        grpc::ServerBuilder builder;
        builder.AddListeningPort(processConfig->GetChildAddress(), grpc::InsecureServerCredentials());

        // Register endpoints
        builder.RegisterService(&childInitController);
    
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        if (!server) {
            logger->log(
                shared::application::services::ILogger::LogLevel::Error,
                "CHILD_PROCESS_SERVER", 
                "Can't create gRPC server"
            );
            m_promise.set_value(false);

            return;
        }

        m_promise.set_value(true);

        logger->log(
            shared::application::services::ILogger::LogLevel::Info,
            "CHILD_PROCESS_SERVER", 
            "gRPC server started on " + processConfig->GetChildAddress()
        );

	    server->Wait();
    }
};

}