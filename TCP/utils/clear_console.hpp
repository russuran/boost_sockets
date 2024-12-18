#ifndef CLEAR_CONSOLE_HPP
#define CLEAR_CONSOLE_HPP

#include <cstdlib>

inline void clear_console() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

#endif // CLEAR_CONSOLE_HPP
