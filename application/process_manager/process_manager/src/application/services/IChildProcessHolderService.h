#pragma once

#include <shared/src/application/services/IBaseHolderService.h>
#include <process_manager/src/domain/models/ProcessInstance.h>

namespace process_manager::application::services {

class IChildProcessHolderService : public shared::application::services::IBaseHolderService<domain::models::ProcessInstance> {
public:
    virtual ~IChildProcessHolderService() = default;
};

}