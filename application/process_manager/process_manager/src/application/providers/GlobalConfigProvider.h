#pragma once

#include <string>
#include <cstdint>
#include <environments/environments.h>
#include <process_manager/src/domain/models/InputConfig.h>

namespace process_manager::application::providers {

class GlobalConfigProvider {
public:
    struct EndpointConfig {
        std::string ip;
        std::uint32_t port;

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

    void setProcessManagerConfig(const domain::models::InputConfig& inputConfig) {
        if (!inputConfig.core_server_ip.empty()) {
            m_coreServerConfig.endpoint.ip = inputConfig.core_server_ip;
        }

        if (!inputConfig.core_server_port.empty()) {
            m_coreServerConfig.endpoint.port = std::stoi(inputConfig.core_server_port);
        }

        if (!inputConfig.process_manager_server_ip.empty()) {
            m_processManagerConfig.endpoint.ip = inputConfig.process_manager_server_ip;
        }

        if (!inputConfig.process_manager_server_port.empty()) {
            m_processManagerConfig.endpoint.port = std::stoi(inputConfig.process_manager_server_port);
        }

        if (!inputConfig.process_manager_name.empty()) {
            m_processManagerConfig.processName = inputConfig.process_manager_name;
        }

        if (!inputConfig.process_manager_process_name.empty()) {
            m_processManagerConfig.coreSideName = inputConfig.process_manager_process_name;
        }

        if (!inputConfig.child_process_ip.empty()) {
            m_childProcessConfig.endpoint.ip = inputConfig.child_process_ip;
        }

        if (!inputConfig.child_process_port.empty()) {
            m_childProcessConfig.endpoint.port = std::stoi(inputConfig.child_process_port);
        }

        if (!inputConfig.child_process_path.empty()) {
            m_childProcessConfig.processApplicationPath = inputConfig.child_process_path;
        }

        if (!inputConfig.child_process_process_name.empty()) {
            m_childProcessConfig.processName = inputConfig.child_process_process_name;
        }
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