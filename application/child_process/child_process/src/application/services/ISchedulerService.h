#pragma once

namespace child_process::application::services {

class ISchedulerService {
public:
    virtual ~ISchedulerService() = default;
    virtual void start() = 0;
    virtual void startAndJoin() = 0;
    [[nodiscard]] virtual bool isRunning() = 0;
};

}