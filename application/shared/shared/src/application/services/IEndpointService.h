#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <shared/src/application/services/ILogger.h>
#include <shared/src/application/services/IEndpointService.h>

namespace shared::application::services {

namespace beast = boost::beast;         
namespace http = beast::http;          
namespace net = boost::asio;           
using tcp = net::ip::tcp;

class IEndpointService {
public:
    // @Todo BOOST types should have wrapper to be placed inside application layer
    using HandlerFunc = std::function<void(boost::asio::ip::tcp::socket&, const http::request<http::string_body>&)>;

    virtual ~IEndpointService() = default;

    virtual void handleRequest(boost::asio::ip::tcp::socket socket) = 0;
    virtual void addHandler(const std::string& route, HandlerFunc handler) = 0;
    virtual void addHandlers(const std::vector<std::pair<std::string, HandlerFunc>>& handlers) = 0;
};

}