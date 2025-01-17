#pragma once

#include <map>
#include <mutex>
#include <vector>
#include <exception>

#include <process_manager/src/application/services/IChildProcessHolderService.h>

namespace process_manager::infrastructure::services {

class ChildProcessHolderService : public process_manager::application::services::IChildProcessHolderService {
public:
    ChildProcessHolderService() : 
        process_manager::application::services::IChildProcessHolderService{},
        m_childProcesses{} {}

    void add(const process_manager::domain::models::ProcessInstance& item) override {
        std::scoped_lock lock(m_mutex);
        if (m_childProcesses.find(item.internalId) != m_childProcesses.end()) {
            throw std::runtime_error("Process with this internalId already exists");
        }

        m_childProcesses[item.internalId] = item;
    }
    
    [[nodiscard]] std::vector<process_manager::domain::models::ProcessInstance> getAll() const override {
        std::scoped_lock lock(m_mutex);
        std::vector<process_manager::domain::models::ProcessInstance> result{};
        for (const auto& process : m_childProcesses) {
            result.push_back(process.second);
        }

        return result;
    }

    void update(const process_manager::domain::models::ProcessInstance& item) override {
        std::scoped_lock lock(m_mutex);
        m_childProcesses[item.internalId] = item;
    }

    void clear() override {
        std::scoped_lock lock(m_mutex);
        m_childProcesses.clear();
    }
    
    [[nodiscard]] std::optional<process_manager::domain::models::ProcessInstance> getById(const std::string& id) const override {
        std::scoped_lock lock(m_mutex);
        if (m_childProcesses.find(id) == m_childProcesses.end()) {
            return std::nullopt;
        }

        return m_childProcesses.at(id);
    }

    void removeById(const std::string& id) override {
        std::scoped_lock lock(m_mutex);
        m_childProcesses.erase(id);
    }

private:
    std::map<std::string, process_manager::domain::models::ProcessInstance> m_childProcesses;
    mutable std::mutex m_mutex;
};

}