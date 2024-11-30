#include <iostream>
#include <communication.pb.h>

int main(int argc, char** argv) {
    std::cout << "hello world!" << std::endl;
    Communication::StopRequest stop_request;
    if (stop_request.process_id() == "123") {
        std::cout << "process_id is 123" << std::endl;
    }

    return 0;
}