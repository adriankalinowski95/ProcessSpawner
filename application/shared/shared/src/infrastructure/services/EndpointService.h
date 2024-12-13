#pragma once

#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/IEndpointService.h>
#include <shared/src/infrastructure/services/SessionService.h>

namespace shared::infrastructure::services {

namespace beast = boost::beast;         
namespace http = beast::http;          
namespace net = boost::asio;           
using tcp = net::ip::tcp;

class EndpointService : public shared::application::services::IEndpointService {
public:
    EndpointService(const std::string& basePath, const std::shared_ptr<shared::application::services::ILogger>& logger) : 
        m_basePath{ basePath },
        m_logger{ logger } {}

    void handleRequest(boost::asio::ip::tcp::socket socket) override {
        try {
            auto request = getRequest(socket);
            auto path = pathCat(m_basePath, request.target());
            auto path2 = request.target();
            if (m_handlers.find(path) != m_handlers.end()) {
                m_handlers[path](socket, request);
            } else {
                m_logger->log("No handler for path: " + path);
            }
        } catch (std::exception& e) {
            m_logger->logError(e.what());
        }
    }

private:
    std::string m_basePath;
    std::shared_ptr<shared::application::services::ILogger> m_logger;
    std::map<std::string, std::function<void(boost::asio::ip::tcp::socket&, const http::request<http::string_body>&)>> m_handlers;

    http::request<http::string_body> getRequest(boost::asio::ip::tcp::socket& socket) {
        boost::system::error_code errorCode{};
        boost::beast::flat_buffer buffer{};

        http::request<http::string_body> request{};
        http::read(socket, buffer, request, errorCode);
        if (errorCode) {
            throw std::runtime_error("Request failed!");
        }

        return request;
    }

    std::string pathCat(boost::beast::string_view base, boost::beast::string_view path) {
        if(base.empty()) {
            return path.data();
        }

        std::string result = base.data();
        char constexpr path_separator = '/';
        if (result.back() == path_separator) {
            result.resize(result.size() - 1);
        }   
        
        result.append(path.data(), path.size());    
        
        return result;
    }
};

}