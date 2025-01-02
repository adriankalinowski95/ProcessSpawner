#pragma once

#include <cstdint>

namespace shared::application::providers {

class ICurrentProcessInfoProvider {
public:
    virtual ~ICurrentProcessInfoProvider() = default;
    [[nodiscard]] virtual std::uint32_t getPid() const = 0;
};

}