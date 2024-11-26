#pragma once

#include <vector>
#include <string>
#include <optional>
#include <exception>

namespace shared::application::services {

template <typename T>
class IBaseHolderService {
public:
    virtual void add(const T& item) = 0;
    [[nodiscard]] virtual std::vector<T> getAll() const = 0;
    virtual void update(const T& item) = 0;
    virtual void clear() = 0;
    
    [[nodiscard]] virtual std::optional<T> getById(const std::string& id) const {
        throw std::runtime_error("Not implemented");
    }

    virtual void removeById(const std::string& id) {
        throw std::runtime_error("Not implemented");
    }
};

}