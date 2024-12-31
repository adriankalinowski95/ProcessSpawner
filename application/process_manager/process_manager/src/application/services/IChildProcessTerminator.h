#pragma once

namespace process_manager::application::services {

class IChildProcessTerminator{
public:
    virtual ~IChildProcessTerminator() = default;
    virtual void terminateChildrens() = 0;
};

}