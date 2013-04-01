#ifndef PRETTY_PRINT_HPP_
#define PRETTY_PRINT_HPP_

#include <cstddef>
#include <iostream>


const char RESET[] = "\u001B[0m";

const char* const COLORS[] = {
    "\u001B[0m",
    "\u001B[1;44m",
    "\u001B[1;42m",
    "\u001B[1;46m",
    "\u001B[1;41m",
    "\u001B[1;45m",
    "\u001B[1;43m",
    "\u001B[1;47m",
};

const int COLOR_NUM = sizeof(COLORS) / sizeof(COLORS[0]) - 1;

const char* const BLUE          = COLORS[1];
const char* const GREEN         = COLORS[2];
const char* const CYAN          = COLORS[3];
const char* const RED           = COLORS[4];
const char* const PURPLE        = COLORS[5];
const char* const BROWN         = COLORS[6];
const char* const LIGHT_GRAY    = COLORS[7];

struct print_n
{
    int n;
    char c;
    print_n(std::size_t n, char c = ' ')
    : n(n), c(c)
    { }
};

inline std::ostream& operator << (std::ostream& stream, const print_n& p)
{
    for (int i = 0; i < p.n; ++ i)
        stream << p.c;
    return stream;
}


#endif /* PRETTY_PRINT_HPP_ */
