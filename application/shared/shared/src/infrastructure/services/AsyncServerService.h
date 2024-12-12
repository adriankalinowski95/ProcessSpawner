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
    virtual ~AsyncServerService() {
        if (m_ioContext) {
            m_ioContext->stop();
        }
    }

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

    std::unique_ptr<boost::asio::io_context> m_ioContext;

    void startThread() {
        try {
            m_ioContext = std::make_unique<boost::asio::io_context>();
            tcp::endpoint endpoint(boost::asio::ip::make_address(m_address), m_port);
            tcp::acceptor acceptor(*m_ioContext, endpoint);

            // acceptor.open(endpoint.protocol());
            // acceptor.set_option(boost::asio::socket_base::reuse_address(true));
            // acceptor.bind(endpoint);
        
            std::function<void()> doAccept;
            doAccept = [&]() {
                // auto socket = acceptor.accept();
                acceptor.async_accept(
                    [&](boost::system::error_code ec, tcp::socket socket) {
                        if (!ec) {
                            auto sessionService = SessionService{ std::move(socket) };
                            auto result = sessionService.readMessage();
                            std::cout << "result: " << result.value() << std::endl;

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

            m_ioContext->run();
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

}