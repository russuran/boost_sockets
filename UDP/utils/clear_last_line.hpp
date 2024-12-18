#ifndef CLEAR_LINE_HPP
#define CLEAR_LINE_HPP

#include <iostream>

inline void clear_line() {
    std::cout << "\033[F";
    std::cout << "\033[K";
}

#endif // CLEAR_LINE_HPP
