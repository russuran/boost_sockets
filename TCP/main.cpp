#include "server/server.hpp"
#include "client/client.hpp"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        try {
            ChatServer server(std::atoi(argv[1]));
            server.run();
        } catch (std::exception& e) {
            std::cerr << "Исключение: " << e.what() << std::endl;
        }
    } else if (argc == 3) {
        try {
            ChatClient client(argv[1], std::atoi(argv[2]));
            client.run();
        } catch (std::exception& e) {
            std::cerr << "Исключение: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Использование: " << argv[0] << " <порт> для сервера или " << argv[0] << " <хост> <порт> для клиента" << std::endl;
    }
    return 0;
}
