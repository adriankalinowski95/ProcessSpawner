#pragma once

#include <future>
#include <thread>
#include <chrono>

#include <process_manager/src/infrastructure/services/ApplicationSingleton.h>
#include <process_manager/src/api/controllers/GenericControllersFactory.h>
#include <process_manager/src/api/controllers/ProcessManagerController.h>
#include <process_manager/src/api/controllers/ProcessQueryController.h>
#include <process_manager/src/api/controllers/ChildPingController.h>

namespace process_manager::api::server {

class ProcessManagerServer {
public:
    ProcessManagerServer() :
        m_thread{},
        m_promise{},
        m_future{ m_promise.get_future() } {}

    ~ProcessManagerServer() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void run() {
        m_thread = std::thread(&ProcessManagerServer::threadFun, this);
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
        auto application = process_manager::application::services::ApplicationSingleton::GetInstance();

        // <START> gRPC controllers
        process_manager::api::controllers::ProcessManagerController managerController{
            application.GetChildProcessSpawnerService(),
            application.GetChildProcessHolderService(),
            application.GetProcessTerminator(),
            application.GetLogger()
        };

        process_manager::api::controllers::ProcessQueryController queryController{
            application.GetChildProcessHolderService(),
            application.GetLogger()
        };

        process_manager::api::controllers::ChildPingController pingController{ 
            application.GetChildProcessHolderService(),
            application.GetLogger()
        };

        auto coreQueryCommunicationController = process_manager::api::controllers::GenericControllersFactory::createCoreQueryCommunicationController(application.GetLogger());
        auto coreCommandCommunicationController = process_manager::api::controllers::GenericControllersFactory::createCoreCommandCommunicationController(application.GetLogger());
        // <END> gRPC controllers

        auto globalConfig = application.GetGlobalConfigProvider();
        grpc::ServerBuilder builder;
        builder.AddListeningPort(globalConfig->GetProcessManagerConfig().endpoint.GetAddress(), grpc::InsecureServerCredentials());

        // Register endpoints
        builder.RegisterService(&managerController);
        builder.RegisterService(&queryController);
        builder.RegisterService(&pingController);
        builder.RegisterService(coreQueryCommunicationController.get());
        builder.RegisterService(coreCommandCommunicationController.get());

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        if (!server) {
            application.GetLogger()->logError("Can't create gRPC server");
            m_promise.set_value(false);

            return;
        }

        m_promise.set_value(true);

        application.GetLogger()->logInfo("gRPC server started on " + globalConfig->GetProcessManagerConfig().endpoint.GetAddress());
	    server->Wait();
    }
};

}