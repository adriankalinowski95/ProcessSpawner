#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace process_manager::domain::models {

struct InputConfig {
    std::string process_manager_server_ip;
    std::string process_manager_server_port;
    std::string process_manager_name;
    std::string process_manager_process_name;
    std::string core_server_ip;
    std::string core_server_port;
    std::string child_process_ip;
    std::string child_process_port;
    std::string child_process_path;
    std::string child_process_process_name;

    std::string to_string() const {
        return "process_manager_server_ip: " + process_manager_server_ip + ", process_manager_server_port: " + process_manager_server_port + ", process_manager_name: " + process_manager_name + ", process_manager_process_name: " + process_manager_process_name + ", core_server_ip: " + core_server_ip + ", core_server_port: " + core_server_port + ", child_process_ip: " + child_process_ip + ", child_process_port: " + child_process_port + ", child_process_path: " + child_process_path + ", child_process_process_name: " + child_process_process_name;
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InputConfig, process_manager_server_ip, process_manager_server_port, process_manager_name, process_manager_process_name, core_server_ip, core_server_port, child_process_ip, child_process_port, child_process_path, child_process_process_name)

}