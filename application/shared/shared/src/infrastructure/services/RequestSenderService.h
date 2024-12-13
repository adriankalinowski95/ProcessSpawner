#pragma once

#include <cstdint>
#include <string>
#include <exception>
#include <optional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <shared/src/infrastructure/services/SessionService.h>

namespace shared::infrastructure::services {

namespace beast = boost::beast;         // Skrót dla boost::beast
namespace http = beast::http;          // Skrót dla boost::beast::http
namespace net = boost::asio;           // Skrót dla boost::asio
using tcp = net::ip::tcp;              // Skrót dla boost::asio::ip::tcp

class RequestSenderService {
public:
    RequestSenderService(const std::string& address, const std::uint32_t& port, const std::string& path) :
        m_address{ address },
        m_port{ port },
        m_path{ path } {}

    std::optional<std::string> sendRequest(const std::string& request, bool withReadResult = true) {
        /*
        try {
            boost::asio::io_context io_context;
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(m_address, std::to_string(m_port));

            tcp::socket socket(io_context);

            boost::asio::connect(socket, endpoints);

            return SessionService{ std::move(socket) }.sendMessage(request, withReadResult);
        } catch(std::exception& e) {
            return std::nullopt;
        }
        */

        try {
            net::io_context ioc;

            // ip::tcp::endpoint endpoint(ip::make_address(m_address), m_port);
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(m_address, std::to_string(m_port));

            // Socket do połączenia z serwerem
            beast::tcp_stream stream(ioc);

            // Połączenie z serwerem
            stream.connect(results);

            // Tworzenie żądania HTTP GET
            http::request<http::string_body> req{http::verb::get, m_path, 11};
            req.set(http::field::host, m_address);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // Wysyłanie żądania do serwera
            http::write(stream, req);

            beast::flat_buffer buffer;
            http::read(stream, buffer, req);

            auto bufs = buffer.data();
            std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());

            return result;
        } catch(std::exception& e) {
            return std::nullopt;
        }
    }
    
private:
    std::string m_address;
    std::uint32_t m_port;
    std::string m_path;
};

}