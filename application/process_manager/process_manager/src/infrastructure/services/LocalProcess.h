#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <process_manager/src/application/services/IProcess.h>
#include <process_manager/src/application/services/IPlatformIO.h>

namespace process_manager::infrastructure::services {

using namespace process_manager::application::services;

class LocalProcess : public process_manager::application::services::IProcess {
public:
    LocalProcess(int processId, int writeHandle, int readHandle, std::shared_ptr<IPlatformIO> platformIO): 
            processId_(processId),
            writeHandle_(writeHandle),
            readHandle_(readHandle),
            terminated_(false),
            platformIO_(platformIO) 
        {
            readerThread_ = std::thread(&LocalProcess::readerLoop, this);
        }
        
    ~LocalProcess() override {

    }

    int getProcessId() const override {
        return processId_;
    }

    bool sendData(std::string_view data) override {
        std::lock_guard<std::mutex> lock(writeMutex_);
        if (writeHandle_ < 0 || terminated_.load()) {
            return false;
        }
        
        return platformIO_->writeData(writeHandle_, data);
    }
    
    std::optional<std::string> getNextOutput() override {
        std::unique_lock<std::mutex> lock(outputMutex_);
        outputCv_.wait(lock, [this]{
            return !outputQueue_.empty() || terminated_.load();
        });

        if (outputQueue_.empty()) {
            return std::nullopt;
        }

        std::string line = std::move(outputQueue_.front());
        outputQueue_.pop();
        
        return line;
    }

    void terminate() override {
        if (!terminated_.exchange(true)) {
            if (processId_ > 0) {
                // Zakładamy UNIX, w implementacji Windows użyjemy innej metody kończenia procesu.
                // kill(processId_, SIGTERM);
            }
        }
    }

private:
    void readerLoop() {
        while (!terminated_.load()) {
            auto data = platformIO_->readDataNonBlocking(readHandle_);
            if (data.has_value()) {
                std::lock_guard<std::mutex> lock(outputMutex_);
                outputQueue_.push(*data);
                outputCv_.notify_one();
            } else {
                // Brak danych lub EOF
                break;
            }
        }

        {
            std::lock_guard<std::mutex> lock(outputMutex_);
            outputCv_.notify_all();
        }
    }
    void closeDescriptors()  {
        if (writeHandle_ >= 0) {
            platformIO_->closeHandle(writeHandle_);
            writeHandle_ = -1;
        }
        
        if (readHandle_ >= 0) {
            platformIO_->closeHandle(readHandle_);
            readHandle_ = -1;
        }

        if (processId_ > 0) {
            int status = 0;
            ::waitpid(processId_, &status, 0);
            processId_ = -1;
        }
    }

    int processId_;
    int writeHandle_;
    int readHandle_;
    std::atomic<bool> terminated_;
    std::shared_ptr<process_manager::application::services::IPlatformIO> platformIO_;

    std::thread readerThread_;
    std::mutex writeMutex_;

    std::mutex outputMutex_;
    std::condition_variable outputCv_;
    std::queue<std::string> outputQueue_;
};

}