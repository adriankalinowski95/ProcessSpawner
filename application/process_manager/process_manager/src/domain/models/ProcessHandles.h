#pragma once

namespace process_manager::domain::models {

struct ProcessHandles {
    int readHandle = -1;
    int writeHandle = -1;
    int processId = -1;
};

}