#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "../utils/clear_console.hpp"
#include "../utils/get_local_ip.hpp"
#include "../utils/clear_last_line.hpp"

using boost::asio::ip::udp;

class ChatClient {
public:
    ChatClient(const std::string& host, short port)
        : socket(io_context_), endpoint(boost::asio::ip::make_address(host), port) {
        socket.open(udp::v4());
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
        socket.async_receive_from(
            boost::asio::buffer(recv_buffer), remote_endpoint,
            [this](boost::system::error_code error, std::size_t bytes_recvd) {
                if (!error) {
                    std::string message(recv_buffer.data(), bytes_recvd);
                    std::cout << message << std::endl;
                } else {
                    std::cerr << "Ошибка :( - " << error.message() << std::endl;
                }
                start_receive();
            });
    }

    void send(const std::string& message) {
        socket.send_to(boost::asio::buffer(message), endpoint);
    }

    boost::asio::io_context io_context_;
    udp::socket socket;
    udp::endpoint endpoint;
    udp::endpoint remote_endpoint;
    std::array<char, 4096> recv_buffer;
};

int main() {
    try {
        ChatClient client("127.0.0.1", 8000);
        client.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
