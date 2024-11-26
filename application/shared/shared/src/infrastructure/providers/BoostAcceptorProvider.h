#pragma once

#include <cstdint>
#include <memory>
#include <exception>
#include <boost/asio.hpp>

// UNUSED!

namespace shared::infrastructure::providers {

using namespace boost::asio;

class BoostAcceptorProvider {
public:
    ~BoostAcceptorProvider() {
        if (m_ioContext) {
            m_ioContext->stop();
        }
    }

    BoostAcceptorProvider(std::string address, std::uint32_t port) : 
        m_address{ address },
        m_port{ port },
        m_ioContext{ nullptr } {}
    
    std::unique_ptr<ip::tcp::acceptor> createAcceptor() {
        // Stop last server context
        if (m_ioContext) {
            m_ioContext->stop();
        }

        // Create new server context
        m_ioContext = std::make_unique<io_context>();
        if (m_ioContext == nullptr) {
            throw std::runtime_error("Can't create io_context");
        }

        ip::tcp::endpoint endpoint(ip::make_address(m_address), m_port);
        auto acceptor = std::make_unique<ip::tcp::acceptor>(*m_ioContext, endpoint);

        // run new server context
        m_ioContext->run();

        return std::move(acceptor);
    }

private:
    std::string m_address;
    std::uint32_t m_port;

    std::unique_ptr<io_context> m_ioContext;
};

}