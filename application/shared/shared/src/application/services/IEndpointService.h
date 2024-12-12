#pragma once

#include <boost/asio.hpp>

namespace shared::application::services {

class IEndpointService {
public:
    virtual ~IEndpointService() = default;

    virtual void handleRequest(boost::asio::ip::tcp::socket socket) = 0;
};

}