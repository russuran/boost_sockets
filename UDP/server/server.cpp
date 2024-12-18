#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <array>
#include "../utils/hashing.hpp"
#include "../utils/clear_console.hpp"

using boost::asio::ip::udp;

class ChatServer {
public:
    ChatServer(short port) : socket(io_context_, udp::endpoint(udp::v4(), port)) {
        start_receive();
        std::cout << "Сервер запущен на порту " << port << std::endl;
    }

    void run() {
        io_context_.run();
    }

private:
    void start_receive() {
        socket.async_receive_from(
            boost::asio::buffer(recv_buffer), remote_endpoint,
            [this](boost::system::error_code error, std::size_t bytes_recvd) {
                if (!error) {
                    std::string message(recv_buffer.data(), bytes_recvd);
                    handle_message(message);
                }
                else {
                    std::cerr << "Ошибка: " << error.message() << std::endl;
                }
                start_receive();
            });
    }

    void handle_message(const std::string& message) {
        if (message.substr(0, 3) == "nu:") {
            std::string username = message.substr(3);
        
        username.erase(0, username.find_first_not_of(" \n\r\t"));
        username.erase(username.find_last_not_of(" \n\r\t") + 1);

        users[remote_endpoint] = username;

        std::string arrival_message = "Пользователь " + username + " прибыл!";
        std::string border = "+" + std::string(arrival_message.length() - 16, '-') + "+\n" +
                             "| " + arrival_message + " |\n" +
                             "+" + std::string(arrival_message.length() - 16, '-') + "+";

        broadcast(border);

            std::cout << "Пользователь подключен: " << username << std::endl;

            send_user_list();
        } else if (message.substr(1, 2) == ":q") {
            auto it = users.find(remote_endpoint);
            if (it != users.end()) {
                broadcast("\nПользователь " + it->second + " покинул чат.");
                users.erase(it);
            }
        } else if (message.substr(1, 6) == ":users") {
            send_user_list();
        } else if (message.substr(1, 6) == ":clear") {
            clear_console();
        } else {
            broadcast(message);
        }
    }

    void broadcast(const std::string& message) {
        for (const auto& user : users) {
            socket.send_to(boost::asio::buffer(message), user.first);
        }
    }

    void send_user_list() {
        std::string user_list = "Список пользователей: ";
        for (const auto& user : users) {
            user_list += user.second + ", ";
        }
        if (!users.empty()) {
            user_list = user_list.substr(0, user_list.size() - 2);
        }
        socket.send_to(boost::asio::buffer(user_list), remote_endpoint);
    }

    boost::asio::io_context io_context_;
    udp::socket socket;
    udp::endpoint remote_endpoint;
    std::array<char, 4096> recv_buffer;
    std::unordered_map<udp::endpoint, std::string, EndpointHash, EndpointEqual> users;
};

int main() {
    try {
        ChatServer server(8000);
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return 0;
}
