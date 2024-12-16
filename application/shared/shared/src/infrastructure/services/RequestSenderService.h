#pragma once

#include <cstdint>
#include <string>
#include <exception>
#include <optional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace shared::infrastructure::services {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class RequestSenderService {
public:
    RequestSenderService(const std::string& address, const std::uint32_t& port, const std::string& path) :
        m_address{ address },
        m_port{ port },
        m_path{ path } {}

    std::optional<std::string> sendRequest(std::optional<std::string> data, bool withReadResult = true) {
        try {
            net::io_context ioContext{};
            tcp::resolver resolver{ ioContext };
            auto const results = resolver.resolve(m_address, std::to_string(m_port));

            beast::tcp_stream stream{ ioContext };
            stream.connect(results);

            http::request<http::string_body> request{
                http::verb::get, 
                m_path, 
                11
            };

            request.set(http::field::host, m_address);
            request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            
            if(data) {
                request.body() = *data;
                request.prepare_payload();
            }

            http::write(stream, request);

            if (!withReadResult) {
                return "";
            }

            beast::flat_buffer buffer{};
            http::response<http::string_body> response{};
            response.version(11);
            response.set(http::field::content_type, "text/plain");
            
            http::read(stream, buffer, response);

            return std::string(response.body().c_str());
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