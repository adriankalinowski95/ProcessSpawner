#pragma once

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <future>
#include <mutex>
#include <deque>
#include <thread>

namespace shared::infrastructure::utils {

using boost::asio::ip::tcp;

class AsyncSession : public std::enable_shared_from_this<AsyncSession> {
public:
    AsyncSession(tcp::socket socket) : 
        m_socket{ std::move(socket) },
        m_strand{ m_socket.get_executor() },
        m_writeMessages{},
        m_readMessage{},
        m_readInProgress{} {}

    /*
    std::future<bool> sendMessage(const std::string& msg) {
        auto prom = std::make_shared<std::promise<bool>>();
        auto fut = prom->get_future();

        boost::asio::post(m_strand,
            [this, msg, prom]() {
                bool isWritting = !m_writeMessages.empty();
                m_writeMessages.push_back(msg);
                if (!isWritting) {
                    do_write(prom);
                } else {
                    // prom->set_value(false);
                }
            }
        );

        return fut;
    }

    std::future<std::string> receive_message() {
        auto prom = std::make_shared<std::promise<std::string>>();
        auto fut = prom->get_future();

        boost::asio::post(strand_,
            [this, prom]() {
                if (!m_readInProgress) {
                    m_readInProgress = true;
                    do_read(prom);
                } else {
                    prom->set_exception(std::make_exception_ptr(std::runtime_error("Read already in progress")));
                }
            }
        );

        return fut;
    }
    */

private:
    void do_read(std::shared_ptr<std::promise<std::string>> prom) {
        auto self(shared_from_this());
        boost::asio::async_read_until(m_socket, boost::asio::dynamic_buffer(m_readMessage), '\n',
            boost::asio::bind_executor(m_strand,
                [this, self, prom](boost::system::error_code ec, std::size_t length) {
                    if (!ec) {
                        std::string msg(m_readMessage.substr(0, length - 1));
                        m_readMessage.erase(0, length);
                        m_readInProgress = false;
                        prom->set_value(msg);
                    } else {
                        m_readInProgress = false;
                        prom->set_exception(std::make_exception_ptr(std::runtime_error("Read failed")));
                    }
                }
            )
        );
    }

    void do_write(std::shared_ptr<std::promise<std::string>> prom) {
        auto self(shared_from_this());

        std::string msg = m_writeMessages.front() + "\n";
        boost::asio::async_write(m_socket, boost::asio::buffer(msg),
            boost::asio::bind_executor(m_strand,
                [this, self, prom](boost::system::error_code ec, std::size_t length) {
                    if (!ec) {
                        m_writeMessages.pop_front();
                        if (!m_writeMessages.empty()) {
                            do_write(prom);
                        }

                        prom->set_value("");
                    } else {
                        prom->set_value("");
                        // prom->set_exception(std::make_exception_ptr(std::runtime_error("Write failed")));
                    }
                }
            )
        );
    }

    tcp::socket m_socket;
    boost::asio::strand<tcp::socket::executor_type> m_strand;

    std::deque<std::string> m_writeMessages;

    std::string m_readMessage;
    bool m_readInProgress;
};

}