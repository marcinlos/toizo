#include <iostream>
#include <sstream>
#include "board.hpp"
#include "print.hpp"

const int VISITED = 0x10000000;
const int MARKED  = 0x20000000;

void print_field(std::ostream& stream, field f)
{
    int bg = (f.color | f.extra) & ID_MASK;
    const char* text =
        (f.extra & VISITED) != 0 ? "@@" :
        (f.extra & SOURCE) != 0 ? "^^" :
        (f.extra & DEST) != 0 ? "$$" : "  ";
    stream << COLORS[bg] << text << RESET;
}


void pretty_print(std::ostream& stream, const board& b)
{
    const int PAD = 10;
    const char* BORD = LIGHT_GRAY;
    std::stringstream sstream;
    stream << print_n(PAD) << BORD << print_n(2 * (b.width() + 2))
           << RESET << std::endl;

    for (int i = 0; i < b.height(); ++ i)
    {
        sstream << print_n(PAD) << BORD << "  " << RESET;
        for (int j = 0; j < b.width(); ++ j)
        {
            print_field(sstream, b[i][j]);
        }
        sstream << BORD << "  " << RESET << std::endl;
    }

    sstream << print_n(PAD) << BORD << print_n(2 * (b.width() + 2))
              << RESET << std::endl;
    stream << sstream.str();
}
