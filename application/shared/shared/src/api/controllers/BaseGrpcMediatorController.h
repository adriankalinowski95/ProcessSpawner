#pragma once

#include <string>
#include <cstdint>
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <shared/src/api/controllers/BaseGrpController.h>

namespace shared::api::controllers {

template <typename Request, typename Response, typename Service>
class BaseGrpcMediatorController : public BaseGrpController, public Service::Service {
public:
    struct Config {
        std::string address;
        std::uint32_t port;
    };

    BaseGrpcMediatorController(Config config, std::shared_ptr<shared::application::services::ILogger> logger) : 
        BaseGrpController{ logger },
        m_config{ config } {}

    virtual ::grpc::Status Handle(::grpc::ServerContext* context, const Request* request, Response* response) override {
       try {
           auto channel = grpc::CreateChannel((m_config.address + ":" + std::to_string(m_config.port)).data(), grpc::InsecureChannelCredentials());
           std::unique_ptr<typename Service::Stub> stub = Service::NewStub(channel);
           grpc::ClientContext context{};

           return stub->Handle(&context, *request, response);
       } catch(...) {
           return ::grpc::Status::CANCELLED;
       }
    }

private:
    Config m_config;
};

}