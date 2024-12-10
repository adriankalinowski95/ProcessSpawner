#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <boost/asio.hpp>

namespace shared::infrastructure::services {

using boost::asio::ip::tcp;

class AsyncServerService {
public:
    virtual ~AsyncServerService() = default;

    AsyncServerService(std::string address, std::uint32_t port) :
        m_address{ address },
        m_port{ port } {}

    bool start() {

    }

private:
    std::string m_address;
    std::uint32_t m_port;

    bool startThread() {
        try {
            boost::asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), m_port));
        
            std::function<void()> do_accept;
            do_accept = [&]() {
                acceptor.async_accept(
                    [&](boost::system::error_code ec, tcp::socket socket) {
                        if (!ec) {
                            std::make_shared<Session>(std::move(socket))->start();
                        }

                        do_accept();
                    });
            };
            
            do_accept();

            io_context.run();
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

}