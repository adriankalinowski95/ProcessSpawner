#pragma once

#include <string>
#include <cstdint>
#include <environments/environments.h>

namespace process_manager::application::providers {

class GlobalConfigProvider {
public:
    struct EndpointConfig {
        const std::string ip;
        const std::uint32_t port;

        [[nodiscard]] std::string GetAddress() const {
            return ip + ":" + std::to_string(port);
        }
    }; 

    struct CoreServerConfig {
        EndpointConfig endpoint;
    };

    struct ProcessManagerConfig {
        EndpointConfig endpoint;
        std::string Name;
    };

    struct ChildProcessConfig {
        EndpointConfig endpoint;
        std::string Process_Application_Path;
    };

    GlobalConfigProvider() :
        m_coreServerConfig{ GetDefaultCoreServerConfig() },
        m_processManagerConfig{ GetDefaultProcessManagerConfig() },
        m_childProcessConfig{ GetDefaultChildProcessConfig() } {}

    GlobalConfigProvider(const CoreServerConfig& coreServerConfig, const ProcessManagerConfig& processManagerConfig, const ChildProcessConfig& childProcessConfig) :
        m_coreServerConfig{ coreServerConfig },
        m_processManagerConfig{ processManagerConfig },
        m_childProcessConfig{ childProcessConfig } {}

    [[nodiscard]] const CoreServerConfig& GetCoreServerConfig() const {
        return m_coreServerConfig;
    }

    [[nodiscard]] const ProcessManagerConfig& GetProcessManagerConfig() const {
        return m_processManagerConfig;
    }

    [[nodiscard]] const ChildProcessConfig& GetChildProcessConfig() const {
        return m_childProcessConfig;
    }

private:
    CoreServerConfig m_coreServerConfig;
    ProcessManagerConfig m_processManagerConfig;
    ChildProcessConfig m_childProcessConfig;

    static CoreServerConfig GetDefaultCoreServerConfig() {
        return CoreServerConfig {
            .endpoint = {
                .ip = environment::core_server::Address.data(),
                .port = environment::core_server::Port
            }
        };
    }

    static ProcessManagerConfig GetDefaultProcessManagerConfig() {
        return ProcessManagerConfig {
            .endpoint = {
                .ip = environment::parent_process::Address.data(),
                .port = environment::parent_process::Port
            },
            .Name = environment::parent_process::Default_Process_Manager_Name.data()
        };
    } 

    static ChildProcessConfig GetDefaultChildProcessConfig() {
        return ChildProcessConfig {
            .endpoint = {
                .ip = environment::child_process::Address.data(),
                .port = environment::child_process::Port
            },
            .Process_Application_Path = environment::child_process::Process_Path.data()
        };
    }
};

}