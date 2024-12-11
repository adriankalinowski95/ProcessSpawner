#pragma once

#include <cstdint>
#include <string>
#include <exception>
#include <optional>
#include <boost/asio.hpp>

#include <shared/src/infrastructure/services/SessionService.h>

namespace shared::infrastructure::services {

class RequestSenderService {
public:
    RequestSenderService(const std::string& address, const std::uint32_t& port) :
        m_address{ address },
        m_port{ port } {}

    std::optional<std::string> sendRequest(const std::string& request) {
        try {
            boost::asio::io_context io_context;
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(m_address, std::to_string(m_port));

            tcp::socket socket(io_context);

            boost::asio::connect(socket, endpoints);

            return SessionService{ std::move(socket) }.sendMessage(request);
        } catch(std::exception& e) {
            return std::nullopt;
        }
    }
    
private:
    std::string m_address;
    std::uint32_t m_port;
};

}