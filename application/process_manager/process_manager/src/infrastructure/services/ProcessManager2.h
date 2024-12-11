#pragma once

#include <spawn.h>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <shared/src/application/services/ILogger.h>
#include <shared/src/infrastructure/services/AsyncServerService.h>

#include <boost/asio.hpp>

namespace process_manager::infrastructure::services {

using namespace shared::application::services;
using boost::asio::ip::tcp;

class ProcessManager2 {
public:
    ProcessManager2(std::shared_ptr<ILogger> logger) :
        m_logger{ logger },
        m_server{"192.168.1.190", 8080} {
            
        m_server.start();

        /*
        thr = std::thread([]() {
            try {
                boost::asio::io_context io_context;
                tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

                std::cout << "Asynchronous server started port: 8080" << std::endl;

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
        */
    }

    void startProcess(const std::string& program, const std::vector<std::string>& args) {
        std::vector<char*> cargs;
        cargs.reserve(args.size() + 2);
        cargs.push_back(const_cast<char*>(program.c_str()));
        for (auto &arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }

        cargs.push_back(nullptr);

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
    shared::infrastructure::services::AsyncServerService m_server;
};

}