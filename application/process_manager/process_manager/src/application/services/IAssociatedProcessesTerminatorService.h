#pragma once

namespace process_manager::application::services {

class IAssociatedProcessesTerminatorService{
public:
    virtual ~IAssociatedProcessesTerminatorService() = default;
    virtual void terminate() = 0;
};

}