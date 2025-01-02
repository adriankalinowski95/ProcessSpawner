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
        std::string processName;
        std::string coreSideName;
    };

    struct ChildProcessConfig {
        EndpointConfig endpoint;
        std::string processName;
        std::string processApplicationPath;
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

    [[nodiscard]] static CoreServerConfig GetDefaultCoreServerConfig() {
        return CoreServerConfig {
            .endpoint = {
                .ip = environment::core_server::Address.data(),
                .port = environment::core_server::Port
            }
        };
    }

    [[nodiscard]] static ProcessManagerConfig GetDefaultProcessManagerConfig() {
        return ProcessManagerConfig {
            .endpoint = {
                .ip = environment::parent_process::Address.data(),
                .port = environment::parent_process::Port
            },
            .processName = environment::parent_process::Process_Name.data(),
            .coreSideName = environment::parent_process::Core_Side_Process_Manager_Name.data()
        };
    } 

    [[nodiscard]] static ChildProcessConfig GetDefaultChildProcessConfig() {
        return ChildProcessConfig {
            .endpoint = {
                .ip = environment::child_process::Address.data(),
                .port = environment::child_process::Port
            },
            .processName = environment::child_process::Process_Name.data(),
            .processApplicationPath = environment::child_process::Process_Path.data()
        };
    }
};

}