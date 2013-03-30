#ifndef ARRAY2D_HPP_
#define ARRAY2D_HPP_

#include "point.hpp"
#include <vector>

template <typename Field>
class array2d
{
private:
    int width_, height_;
    std::vector<Field> buffer;

public:
    array2d(int width, int height)
    : width_(width), height_(height), buffer(width * height)
    { }

    Field* operator [](std::size_t n)
    { return &buffer[n * width_]; }

    const Field* operator [](std::size_t n) const
    { return &buffer[n * width_]; }

    const Field& operator [](const point& p) const
    { return (*this)[p.y][p.x]; }

    Field& operator [](const point& p)
    { return (*this)[p.y][p.x]; }

    int width() const { return width_; }

    int height() const { return height_; }

    bool inside(const point& p) const
    {
        return p.x >= 0 && p.x < width_
            && p.y >= 0 && p.y < height_;
    }

};


#endif /* ARRAY2D_HPP_ */
