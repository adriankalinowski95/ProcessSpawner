#pragma once

#include <iostream>
#include <unistd.h>
#include <shared/src/application/providers/ICurrentProcessInfoProvider.h>

namespace shared::infrastructure::providers {

// @Todo change name to adapter???
class UnixProcessInfoProvider : public shared::application::providers::ICurrentProcessInfoProvider {
public:
    virtual ~UnixProcessInfoProvider() = default;

    std::uint32_t getPid() const override {
        return getpid();
    }
};

}