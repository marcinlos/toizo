#include <iostream>
#include <sstream>
#include "board.hpp"
#include "walker.hpp"
#include "print.hpp"
#include "print_array.hpp"


void print_field(std::ostream& stream, const field& f)
{
    int bg = f.color;
    if (bg != 0)
        bg = 1 + (bg - 1) % COLOR_NUM;
    const char* text =
        f.is(SOURCE) ? "^^" :
        f.is(DEST) ? "$$" :
        f.is(OCCUPIED) ? " *" :
        f.is(USED) ? " ." :
        f.is(CUSTOM) ? "##" : "  ";
    stream << COLORS[bg] << text << RESET;
}


void pretty_print(std::ostream& stream, const board& b)
{
    print_array(print_field, stream, b);
}
