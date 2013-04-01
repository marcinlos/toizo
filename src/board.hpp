#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <vector>
#include "point.hpp"
#include "array2d.hpp"

struct field
{
    short color;
    short flags;

    field()
    : color(0), flags(0)
    { }

    void set(short flag) { flags |= flag; }
    bool is(short flag) const { return (flags & flag) != 0; }
    void clear(short flag) { flags &= ~flag; }
};

const short EMPTY = 0;

const short USED    = 0x0001;
const short SOURCE  = 0x0020;
const short DEST    = 0x0040;


class board: public array2d<field>
{
public:
    board(int width, int height)
    : array2d(width, height)
    { }

    bool is_free(const point& p) const
    {
        if (inside(p))
        {
            const field& f = (*this)[p];
            bool endpoint = f.is(DEST);
            return f.color == 0 && ! endpoint;
        }
        return false;
    }

    bool is_his_dest_nopos_(const point& p, int id) const
    {
        const field& f = (*this)[p];
        return f.is(DEST) && f.color == id;
    }

    bool is_his_dest(const point& p, int id) const
    {
        if (inside(p))
            return is_his_dest_nopos_(p, id);
        else
            return false;
    }

    bool can_enter(const point& p, int id) const
    {
        if (inside(p))
        {
            const field& f = (*this)[p];
            return f.color == EMPTY || is_his_dest(p, id);
        }
        return false;
    }
};

void pretty_print(std::ostream& stream, const board& b);

bool reachable(board& b, point src, point dst, int id);


#endif /* BOARD_HPP_ */
