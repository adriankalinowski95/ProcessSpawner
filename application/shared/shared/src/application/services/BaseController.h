#pragma once

#include <string>
#include <vector>
#include <functional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace http = boost::beast::http;

class BaseController;

class HandlerRegistrar {
public:
    HandlerRegistrar(BaseController& controller,
                     const std::string& route,
                     std::function<void(boost::asio::ip::tcp::socket&, const http::request<http::string_body>&)> handler);
};

class BaseController {
    using HandlerFunc = std::function<void(boost::asio::ip::tcp::socket&, const http::request<http::string_body>&)>;
public:
    BaseController() = default;

    void registerHandler(const std::string& route,
                         HandlerFunc handler) {
        m_handlers.emplace_back(route, handler);
    }

    const std::vector<std::pair<std::string, HandlerFunc>>& getHandlers() const {
        return m_handlers;
    }

private:
    std::vector<std::pair<std::string, HandlerFunc>> m_handlers;

    friend class HandlerRegistrar;

protected:
    http::response<http::string_body> getBaseResponse() {
        http::response<http::string_body> res{
            http::status::ok, 
            req.version()
        };
        
        res.set(http::field::content_type, "text/plain");
        res.prepare_payload();

        return res;
    }
};

inline HandlerRegistrar::HandlerRegistrar(BaseController& controller,
                                           const std::string& route,
                                           std::function<void(boost::asio::ip::tcp::socket&, const http::request<http::string_body>&)> handler) {
    controller.registerHandler(route, handler);
}