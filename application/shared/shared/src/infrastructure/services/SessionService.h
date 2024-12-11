#pragma once

#include <string>
#include <ranges>
#include <boost/asio.hpp>

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

            return readMessage();
        } catch(std::exception& e) {
            // @Todo logger

            return std::nullopt;
        }
    }

    std::optional<std::string> readMessage() {
        try {
            boost::asio::streambuf buffer{};
            boost::asio::read_until(m_socket, buffer, "\0");

            std::string result{};
            
            /*
            auto begin = boost::asio::buffers_begin(buffer);
            auto end = boost::asio::buffers_end(buffer);
            
            std::ranges::transform(begin, end, std::back_inserter(result), [](char c) {
                return c == '\0' ? '\n' : c;
            });

            return result;
            */

           return result;
        } catch(std::exception& e) {
            // @Todo logger

            return std::nullopt;
        }
    }

private:
    tcp::socket m_socket;
};

}