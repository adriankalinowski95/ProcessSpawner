#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <atomic>
#include <future>

#include <process_manager/src/application/services/IProcess.h>
#include <process_manager/src/application/services/IProcessFactory.h>
#include <process_manager/src/application/services/IProcessManager.h>

namespace process_manager::infrastructure::services {

class ProcessManager final : public process_manager::application::services::IProcessManager {
public:
    explicit ProcessManager(std::unique_ptr<process_manager::application::services::IProcessFactory> factory) :
        processFactory_(std::move(factory)), 
        stopFlag_(false) {}

    int startProcess(const std::string &program, const std::vector<std::string> &args) override {
        auto proc = processFactory_->createProcess(program, args);
        int pid = proc->getProcessId();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            processes_.emplace(pid, std::move(proc));
        }
    
        return pid;
    }

    bool sendToProcess(int processId, std::string_view data) override {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = processes_.find(processId);
        if (it == processes_.end()) {
            return false;
        }

        return it->second->sendData(data);
    }

    std::optional<std::string> readFromProcess(int processId) override {
        // Aby uniknąć długiego blokowania mapy, kopiujemy wskaźnik poza sekcję krytyczną
        
        process_manager::application::services::IProcess* proc = nullptr;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = processes_.find(processId);
            if (it == processes_.end()) return std::nullopt;
            proc = it->second.get();
        }
        
        return proc->getNextOutput();
    }

    void terminateProcess(int processId) override {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = processes_.find(processId);
        if (it != processes_.end()) {
            it->second->terminate();
            processes_.erase(it);
        }
    }

    void run(std::future<void> shutdownSignal) override {
         shutdownSignal.wait();
        stopFlag_.store(true);

        std::lock_guard<std::mutex> lock(mutex_);
        for (auto & [_, proc] : processes_) {
            proc->terminate();
        }
        processes_.clear();
    }

private:
    process_manager::application::services::IProcess* getProcess(int processId) {
        auto it = processes_.find(processId);
        return (it == processes_.end()) ? nullptr : it->second.get();
    }

    std::map<int, std::unique_ptr<process_manager::application::services::IProcess>> processes_;
    std::unique_ptr<process_manager::application::services::IProcessFactory> processFactory_;
    std::mutex mutex_;
    std::atomic<bool> stopFlag_;
};

}