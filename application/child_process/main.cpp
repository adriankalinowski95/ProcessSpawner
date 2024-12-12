#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <boost/asio.hpp>
#include <child_process/src/application/utils/ProcessParserUtils.h>

#include <shared/src/infrastructure/services/RequestSenderService.h>

#include <shared/src/infrastructure/services/AsyncServerService.h>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(boost::asio::io_context &io_context, const std::string &host, const std::string &port)
        : socket_(io_context)
    {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);

        do_connect(endpoints);
    }

    void write(const std::string &msg) {
        auto self(shared_from_this());
        boost::asio::post(socket_.get_executor(),
                          [this, self, msg]() {
                              bool write_in_progress = !outbox_.empty();
                              outbox_.push_back(msg + "\n");
                              if (!write_in_progress) {
                                  do_write();
                              }
                          });
    }

private:
    void do_connect(const tcp::resolver::results_type &endpoints) {
        auto self(shared_from_this());
        boost::asio::async_connect(socket_, endpoints,
            [this, self](boost::system::error_code ec, tcp::endpoint) {
                if (!ec) {
                    // Po nawiazaniu polaczenia mozemy zaczac czytac odpowiedzi serwera
                    do_read();
                } else {
                    std::cerr << "Blad polaczenia: " << ec.message() << std::endl;
                }
            });
    }

    void do_read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string message;
                    std::getline(is, message);
                    std::cout << "Odebrano z serwera: " << message << std::endl;

                    // Po odczytaniu odpowiedzi z serwera, znowu czekamy na kolejne
                    do_read();
                } else if (ec != boost::asio::error::operation_aborted) {
                    std::cerr << "Blad odczytu: " << ec.message() << std::endl;
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(outbox_.front()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    outbox_.pop_front();
                    if (!outbox_.empty()) {
                        do_write();
                    }
                } else {
                    std::cerr << "Blad zapisu: " << ec.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> outbox_;
};


int main(int argc, char** argv) {
    try {
        const auto params = child_process::application::utils::ChildProcessParamsParser::GetParams(argc, argv);
        if (params.empty()) {
            std::cerr << "Niepoprawne parametry!" << std::endl;
            return 1;
        }

        

        shared::infrastructure::services::RequestSenderService sender{"127.0.0.1", 8080};
        sender.sendRequest("Hello, server!");

        shared::infrastructure::services::AsyncServerService server{"192.168.1.190", 8081};
        server.start();
        server.join();
    }
    catch (std::exception& e) {
        std::cerr << "Błąd: " << e.what() << std::endl;
    }

    return 0;
}