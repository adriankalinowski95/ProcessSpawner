#pragma once

#include <vector>
#include <process_manager/src/domain/models/ProcessInstance.h>

namespace process_manager::infrastructure::services {

class ChildProcessHolderService {
public:
    ChildProcessHolderService() = default;

    void addChildProcess(const process_manager::domain::models::ProcessInstance& process) {
        m_childProcesses.push_back(process);
    }

    [[nodiscard]] const std::vector<process_manager::domain::models::ProcessInstance>& getChildProcesses() const {
        return m_childProcesses;
    }

    void clearChildProcesses() {
        m_childProcesses.clear();
    }

private:
    std::vector<process_manager::domain::models::ProcessInstance> m_childProcesses;
};

}