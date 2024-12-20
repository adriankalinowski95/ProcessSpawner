#pragma once

#include <vector>
#include <exception>
#include <map>
#include <process_manager/src/domain/models/ProcessInstance.h>

namespace process_manager::infrastructure::services {

class ChildProcessHolderService {
public:
    ChildProcessHolderService() = default;

    void addChildProcess(const process_manager::domain::models::ProcessInstance& process) {
        if (m_childProcesses.find(process.internalId) != m_childProcesses.end()) {
            throw std::runtime_error("Process with this internalId already exists");
        }

        m_childProcesses[process.internalId] = process;
    }

    [[nodiscard]] std::vector<process_manager::domain::models::ProcessInstance> getChildProcesses() const {
        // @Todo Mutex
        std::vector<process_manager::domain::models::ProcessInstance> result{};
        for (const auto& process : m_childProcesses) {
            result.push_back(process.second);
        }

        return result;
    }

    [[nodiscard]] std::optional<process_manager::domain::models::ProcessInstance> getChildProcessByInternalId(const std::string& internalId) const {
        if (m_childProcesses.find(internalId) == m_childProcesses.end()) {
            return std::nullopt;
        }

        return m_childProcesses.at(internalId);
    }

    void updateChildProcess(const process_manager::domain::models::ProcessInstance& process) {
        m_childProcesses[process.internalId] = process;
    }

    void clearChildProcesses() {
        m_childProcesses.clear();
    }

private:
    std::map<std::string, process_manager::domain::models::ProcessInstance> m_childProcesses;
};

}