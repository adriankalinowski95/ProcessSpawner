#pragma once

#include <thread>
#include <chrono>
#include <cstdint>
#include <string>
#include <functional>
#include <optional>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>

namespace shared::infrastructure::commands {

template <typename Request, typename Response, typename Service>
class GenericGrpcRequestSenderCommand {
public:
    struct Config {
        std::string address;
        std::uint32_t port;
        std::uint32_t retries = 3;
        std::uint32_t delayBetweenRetries = 1000;
        std::function<bool(Response)> validationFunction = nullptr;
        std::uint32_t requestTimeout = 5000;
    };

    GenericGrpcRequestSenderCommand(const Config& config, std::function<grpc::Status(typename Service::Stub*, grpc::ClientContext*, Request*, Response*)> method) : 
        m_config{ config }, 
        m_method{ method }  {}

    std::optional<Response> sendRequest(Request request) {
        const auto requestFunction = getRequestFunction();
        for (auto i = 0; i < m_config.retries; i++) {
            const auto result = requestFunction(request);
            if (!result) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_config.delayBetweenRetries));

                continue;
            }

            if (!m_config.validationFunction) {
                return *result;
            }

            if (m_config.validationFunction(*result)) {
                return *result;
            }
        }

        return std::nullopt;
    }

private:
    Config m_config;
    std::function<grpc::Status(typename Service::Stub*, grpc::ClientContext*, Request*, Response*)> m_method;

    std::function<std::optional<Response>(Request)> getRequestFunction() {
        return [this](Request request) -> std::optional<Response> {
            try {
                auto channel = grpc::CreateChannel((m_config.address + ":" + std::to_string(m_config.port)).data(), grpc::InsecureChannelCredentials());
                std::unique_ptr<typename Service::Stub> stub = Service::NewStub(channel);
                grpc::ClientContext context{};

                auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(m_config.requestTimeout);
                context.set_deadline(deadline);

                Response response{};
                auto status = m_method(stub.get(), &context, &request, &response);
                if (!status.ok()) {
                    return std::nullopt;
                }

                return response;
            } catch(...) {
                return std::nullopt;
            }
        };
    }
};

}