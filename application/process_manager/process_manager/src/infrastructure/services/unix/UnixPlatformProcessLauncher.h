#pragma once

#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <process_manager/src/application/services/IPlatformProcessLauncher.h>

namespace process_manager::infrastructure::services::unix {

class UnixPlatformProcessLauncher : public process_manager::application::services::IPlatformProcessLauncher {
public:
    process_manager::domain::models::ProcessHandles launchProcess(const std::string& program, const std::vector<std::string>& args) override {
        int inPipe[2];
        int outPipe[2];

        if (pipe(inPipe) == -1 || pipe(outPipe) == -1) {
            throw std::runtime_error("Failed to create pipes");
        }

        pid_t pid = fork();
        if (pid < 0) {
            throw std::runtime_error("Failed to fork process");
        }

        if (pid == 0) {
            // Proces potomny
            close(inPipe[1]);
            close(outPipe[0]);

            dup2(inPipe[0], STDIN_FILENO);
            dup2(outPipe[1], STDOUT_FILENO);

            close(inPipe[0]);
            close(outPipe[1]);

            std::vector<char*> cargs;
            cargs.reserve(args.size() + 2);
            cargs.push_back(const_cast<char*>(program.c_str()));
            for (auto &arg : args) {
                cargs.push_back(const_cast<char*>(arg.c_str()));
            }
            cargs.push_back(nullptr);

            execvp(program.c_str(), cargs.data());
            _exit(1);
            // @Todo throw

        } else {
            // Proces macierzysty
            close(inPipe[0]);
            close(outPipe[1]);

            process_manager::domain::models::ProcessHandles handles;
            handles.readHandle = outPipe[0];
            handles.writeHandle = inPipe[1];
            handles.processId = pid;

            return handles;
        }
    }
};

}