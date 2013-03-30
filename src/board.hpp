#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <vector>
#include "point.hpp"

typedef int field;

const int ID_BITS = 8;

const int SOURCE  = 0x00010000;
const int DEST    = 0x00020000;
const int ID_MASK = 0x000000ff; // (1 << ID_BITS) - 1

inline int special_id(field f)
{
    return (f >> ID_BITS) & ID_MASK;
}


class board
{
private:
    int width_, height_;
    std::vector<field> buffer;

public:
    board(int width, int height)
    : width_(width), height_(height), buffer(width * height)
    { }

    field* operator [](std::size_t n) { return &buffer[n * width_]; }
    const field* operator [](std::size_t n) const { return &buffer[n * width_]; }

    field operator [](const point& p) const { return (*this)[p.y][p.x]; }
    field& operator [](const point& p) { return (*this)[p.y][p.x]; }

    int width() const { return width_; }
    int height() const { return height_; }

    bool inside(const point& p) const
    {
        return p.x >= 0 && p.x < width_
            && p.y >= 0 && p.y < height_;
    }

    bool can_go(const point& p, int id) const
    {
        if (inside(p))
        {
            field f = (*this)[p];
            int special = (f >> ID_BITS) & ID_MASK;
            return (f & ID_MASK) == 0 && (special == 0 || special == id);
        }
        return false;
    }
};

void pretty_print(std::ostream& stream, const board& b);

bool reachable(board& b, point src, point dst, int id);


#endif /* BOARD_HPP_ */
