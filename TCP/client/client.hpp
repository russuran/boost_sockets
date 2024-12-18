#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "../utils/clear_console.hpp"
#include "../utils/get_local_ip.hpp"
#include "../utils/clear_last_line.hpp"

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(const std::string& host, short port)
        : socket(io_context_) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(socket, endpoints);
        start_receive();
    }

    void run() {
        std::thread t([this]() { io_context_.run(); });

        std::string username;
        std::cout << "Введите имя или оставьте поле пустым (по умолчанию будет ваш IP): ";
        
        std::getline(std::cin, username);
        if (username.empty()) {
            username = get_local_ip();
        }
        send("nu:" + username);

        std::string message;
        while (std::getline(std::cin, message)) {
            if (message == ":clear") {
                clear_console();
            } else {
                clear_line();
                send('\n' + message + '\n' + '-' + username);
            }
        }
        t.join();
    }

private:
    void start_receive() {
        auto buffer = std::make_shared<std::array<char, 1024>>();
        socket.async_read_some(boost::asio::buffer(*buffer), [this, buffer](boost::system::error_code error, std::size_t bytes_recvd) {
            if (!error) {
                std::string message(buffer->data(), bytes_recvd);
                std::cout << message << std::endl;
                start_receive();
            } else {
                std::cerr << "Ошибка: " << error.message() << std::endl;
            }
        });
    }

    void send(const std::string& message) {
        boost::asio::write(socket, boost::asio::buffer(message));
    }

    boost::asio::io_context io_context_;
    tcp::socket socket;
};

#endif // CHAT_CLIENT_HPP
