#ifndef WALKER_HPP_
#define WALKER_HPP_

#include <stack>
#include "board.hpp"

struct walker
{
    int id;
    board& b;
    point pos, dest;
    std::stack<dir> moves;

    walker(int id, board& b, const point& pos, const point& dest)
    : id(id), b(b), pos(pos), dest(dest)
    { }

    bool go(dir d)
    {
        point p = pos + UNIT[d];
        if (b.can_go(p, id))
        {
            pos = p;
            b[p].color = id;
            moves.push(d);
            return true;
        }
        else
            return false;
    }

    void back()
    {
        b[pos].color = 0;
        dir last = moves.top();
        moves.pop();
        pos -= UNIT[last];
    }

    bool done() const
    {
        return pos == dest;
    }
};


#endif /* WALKER_HPP_ */
