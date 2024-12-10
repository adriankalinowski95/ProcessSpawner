#pragma once

#include <spawn.h>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <shared/src/application/services/ILogger.h>
#include <boost/asio.hpp>

namespace process_manager::infrastructure::services {

using namespace shared::application::services;
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, "\n",
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string message;
                    std::getline(is, message);
                    std::cout << "Odebrano od klienta: " << message << std::endl;

                    // Wysyłanie odpowiedzi
                    std::string reply = "Witaj z asynchronicznego serwera!\n";
                    do_write(reply);
                }
            });
    }

    void do_write(const std::string& msg) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    // Możesz kontynuować czytanie kolejnych wiadomości lub zakończyć sesję
                }
            });
    }

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
};

class ProcessManager2 {
public:
    ProcessManager2(std::shared_ptr<ILogger> logger) :
        m_logger{ logger } {
        
        thr = std::thread([]() {
            try {
            boost::asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
            std::cout << "Asynchroniczny serwer oczekuje na połączenie na porcie 8080..." << std::endl;

            // Funkcja lambda do akceptacji nowych połączeń
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
            }
            catch(std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void startProcess(const std::string& program, const std::vector<std::string>& args) {
        std::vector<char*> cargs;
        cargs.reserve(args.size() + 2);
        cargs.push_back(const_cast<char*>(program.c_str()));
        for (auto &arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }

        cargs.push_back(nullptr);

        // posix_spawn tworzy nowy proces i uruchamia w nim podany program
        pid_t pid;
        int status = posix_spawn(&pid, program.data(), NULL, NULL, cargs.data(), NULL);

        if (status == 0) {
            std::cout << "Uruchomiono nowy proces o PID: " << pid << "\n";
        } else {
            std::cerr << "posix_spawn nie powiodl sie, kod: " << status << "\n";
        }
    }

private:
    std::shared_ptr<ILogger> m_logger;
    std::thread thr;
};

}