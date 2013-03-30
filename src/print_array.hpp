#ifndef PRINT_ARRAY_HPP_
#define PRINT_ARRAY_HPP_

#include <iostream>
#include "array2d.hpp"
#include "print.hpp"

template <
    typename FieldPrinter,
    typename CharT,
    typename Traits,
    typename Field
>
void print_array(
    FieldPrinter printer,
    std::basic_ostream<CharT, Traits>& stream,
    const array2d<Field>& array)
{
    const int PAD = 10;
    const char* BORD = LIGHT_GRAY;
    stream << print_n(PAD) << BORD << print_n(2 * (array.width() + 2))
           << RESET;
    stream << '\n';

    for (int i = 0; i < array.height(); ++ i)
    {
        stream << print_n(PAD) << BORD << "  " << RESET;
        for (int j = 0; j < array.width(); ++ j)
            printer(stream, array[i][j]);
        stream << BORD << "  " << RESET << '\n';
    }
    stream << print_n(PAD) << BORD << print_n(2 * (array.width() + 2))
              << RESET << '\n';
}



#endif /* PRINT_ARRAY_HPP_ */
