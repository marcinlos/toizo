#ifndef WALKER_HPP_
#define WALKER_HPP_

#include <stack>
#include "board.hpp"

const short OCCUPIED = 0x0002;

struct walker
{

    int id;
    board& b;
    point pos, dest;
    std::stack<dir> moves;

    walker(int id, board& b, const point& pos, const point& dest)
    : id(id), b(b), pos(pos), dest(dest)
    {
        b[pos].set(OCCUPIED);
    }

    bool go(dir d)
    {
        point p = pos + UNIT[d];
        if (b.can_go(p, id))
        {
            b[pos].clear(OCCUPIED);
            pos = p;
            b[p].color = id;
            b[p].set(OCCUPIED);
            moves.push(d);
            return true;
        }
        else
            return false;
    }

    void back()
    {
        b[pos].color = 0;
        b[pos].clear(OCCUPIED);
        dir last = moves.top();
        moves.pop();
        pos -= UNIT[last];
        b[pos].set(OCCUPIED);
    }

    bool done() const
    {
        return pos == dest;
    }
};


#endif /* WALKER_HPP_ */
