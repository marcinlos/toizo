#include <iostream>
#include <sstream>
#include "board.hpp"
#include "print.hpp"


void print_field(std::ostream& stream, field f)
{
    int bg = (f | (f >> ID_BITS)) & ID_MASK;
    const char* text = (f & SOURCE) != 0 ? "^^" : (f & DEST) != 0 ? "$$" : "  ";
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
