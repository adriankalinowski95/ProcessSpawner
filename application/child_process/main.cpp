// Client.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main(int argc, char** argv) {
    try {
        std::cout << "child project!" << std::endl;
        // Inicjalizacja kontekstu I/O
        boost::asio::io_context io_context;

        // Rozwiązanie adresu serwera (localhost) i portu 8080
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "8080");

        // Tworzenie i łączenie gniazda
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Połączono z serwerem." << std::endl;

        // Wysyłanie wiadomości do serwera
        std::string msg = "Witaj serwerze!\n";
        boost::asio::write(socket, boost::asio::buffer(msg));

        std::cout << "Wiadomość wysłana do serwera: " << msg;

        // Odbieranie odpowiedzi od serwera
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        std::istream input(&buffer);
        std::string reply;
        std::getline(input, reply);
        std::cout << "Odebrano od serwera: " << reply << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Błąd: " << e.what() << std::endl;
    }

    return 0;
}