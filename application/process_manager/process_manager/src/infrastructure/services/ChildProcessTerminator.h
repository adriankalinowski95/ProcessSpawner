#pragma once


#include <process_manager/src/application/services/IChildProcessTerminator.h>
#include <process_manager/src/application/services/IChildProcessHolderService.h>

namespace process_manager::infrastructure::services {

class ChildProcessTerminator : public process_manager::application::services::IChildProcessTerminator {
public:
    ChildProcessTerminator

    void terminateAll(const std::vector<process_manager::domain::models::ProcessInfo>& processes) {
        for (const auto& process : processes) {
            if (process.pid == 0) {
                continue;
            }

            kill(process.pid, SIGKILL);
        }
    }
};

}