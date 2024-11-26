#pragma once

#include <string>
#include <ranges>
#include <boost/asio.hpp>

// UNUSED!

namespace shared::infrastructure::services {

using boost::asio::ip::tcp;

class SessionService {
public:
    SessionService(tcp::socket socket) : 
        m_socket{ std::move(socket) } {}

    std::optional<std::string> sendMessage(const std::string& msg, bool withReadResult = true) {
        try {
            const auto readedSize = boost::asio::write(m_socket, boost::asio::buffer(msg));
            if (readedSize != msg.size()) {
                throw std::runtime_error("Not all data was written!");
            }
            
            if (withReadResult) {
                return readMessage();
            }

            return "";
        } catch(std::exception& e) {
            std::cout << e.what() << std::endl;
            // @Todo logger

            return std::nullopt;
        }
    }

    std::optional<std::string> readMessage() {
        try {
            boost::asio::streambuf buffer{};
            boost::asio::read_until(m_socket, buffer, "\0");

            auto bufs = buffer.data();
            std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());

            return result;
        } catch(std::exception& e) {
            std::cout << e.what() << std::endl;
            // @Todo logger

            return std::nullopt;
        }
    }

private:
    tcp::socket m_socket;
};

}