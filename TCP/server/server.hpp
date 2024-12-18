#ifndef CHAT_SERVER_HPP
#define CHAT_SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <array>
#include <thread>
#include "../utils/hashing.hpp"
#include "../utils/clear_console.hpp"

using boost::asio::ip::tcp;

class ChatServer {
public:
    ChatServer(short port) : acceptor(io_context_, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
        std::cout << "Сервер запущен на порту " << port << std::endl;
    }

    void run() {
        io_context_.run();
    }

private:
    void start_accept() {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor.async_accept(*socket, [this, socket](boost::system::error_code error) {
            if (!error) {
                std::cout << "Новый пользователь подключен." << std::endl;
                start_receive(socket);
            }
            start_accept();
        });
    }

    void start_receive(std::shared_ptr<tcp::socket> socket) {
        auto buffer = std::make_shared<std::array<char, 1024>>();
        socket->async_read_some(boost::asio::buffer(*buffer), [this, socket, buffer](boost::system::error_code error, std::size_t bytes_recvd) {
            if (!error) {
                std::string message(buffer->data(), bytes_recvd);
                handle_message(message, socket);
                start_receive(socket);
            } else {
                std::cerr << "Ошибка: " << error.message() << std::endl;
            }
        });
    }

    void handle_message(const std::string& message, std::shared_ptr<tcp::socket> sender_socket) {
        if (message.substr(0, 3) == "nu:") {
            std::string username = message.substr(3);
            username.erase(0, username.find_first_not_of(" \n\r\t"));
            username.erase(username.find_last_not_of(" \n\r\t") + 1);
            users[sender_socket] = username;

            std::string arrival_message = "Пользователь " + username + " прибыл!";
            std::string border = "+" + std::string(arrival_message.length() - 16, '-') + "+\n" +
                                 "| " + arrival_message + " |\n" +
                                 "+" + std::string(arrival_message.length() - 16, '-') + "+\n";

            broadcast(border);
            std::cout << "Пользователь подключен: " << username << std::endl;
            send_user_list(sender_socket);
        } else if (message.substr(1, 2) == ":q") {
            auto it = users.find(sender_socket);
            if (it != users.end()) {
                broadcast("\nПользователь " + it->second + " покинул чат.");
                users.erase(it);
            }
        } else if (message.substr(1, 6) == ":users") {
            send_user_list(sender_socket);
        } else if (message.substr(1, 6) == ":clear") {
            clear_console();
        } else {
            broadcast(message);
        }
    }

    void broadcast(const std::string& message) {
        for (const auto& user : users) {
            boost::asio::write(*user.first, boost::asio::buffer(message));
        }
    }

    void send_user_list(std::shared_ptr<tcp::socket> socket) {
        std::string user_list = "Список пользователей: ";
        for (const auto& user : users) {
            user_list += user.second + ", ";
        }
        if (!users.empty()) {
            user_list = user_list.substr(0, user_list.size() - 2);
        }
        boost::asio::write(*socket, boost::asio::buffer(user_list));
    }

    boost::asio::io_context io_context_;
    tcp::acceptor acceptor;
    std::unordered_map<std::shared_ptr<tcp::socket>, std::string> users;
};

#endif // CHAT_SERVER_HPP
