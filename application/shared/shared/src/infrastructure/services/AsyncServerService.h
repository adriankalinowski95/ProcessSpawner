#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <boost/asio.hpp>

#include <shared/src/infrastructure/utils/AsyncSession.h>
#include <shared/src/infrastructure/services/SessionService.h>

namespace shared::infrastructure::services {

using boost::asio::ip::tcp;
using namespace std;

class AsyncServerService {
public:
    virtual ~AsyncServerService() = default;

    AsyncServerService(std::string address, std::uint32_t port) :
        m_address{ address },
        m_port{ port } {}

    void start() {
        m_thread = std::thread(&AsyncServerService::startThread, this); 
    }

    void join() {
        m_thread.join();
    }

private:
    std::string m_address;
    std::uint32_t m_port;

    std::thread m_thread;

    void startThread() {
        try {
            boost::asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), m_port));
        
            std::function<void()> doAccept;
            doAccept = [&]() {
                auto socket = acceptor.accept();
                acceptor.async_accept(
                    [&](boost::system::error_code ec, tcp::socket socket) {
                        if (!ec) {
                            auto sessionService = SessionService{ std::move(socket) };
                            auto result = sessionService.readMessage();
                            if (!result.has_value() || result->empty()) {
                                sessionService.sendMessage("Error", false);
                            } else {
                                sessionService.sendMessage("Echo", false);
                                // result parser
                            }
                        }

                        // @Todo
                        // stack overflow????
                        doAccept();
                    });
            };
            
            doAccept();

            io_context.run();
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

}