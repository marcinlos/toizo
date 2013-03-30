#include <queue>
#include "board.hpp"

const field VISITED = 0x10000000;
const field MARKED  = 0x20000000;

const field TOUCHED = VISITED | MARKED;

static bool is_clean(field f)
{
    return (f & TOUCHED) == 0;
}

static void clean_marks(board& b)
{
    for (int i = 0; i < b.height(); ++ i)
    {
        for (int j = 0; j < b.width(); ++ j)
            b[i][j] &= ~TOUCHED;
    }
}

bool reachable(board& b, point src, point dst, int id)
{
    //std::cout << "reachable" << std::endl;
    bool result = false;
    std::queue<point> q;
    q.push(src);
    while (! q.empty())
    {
        point p = q.front();
        q.pop();

        if (p == dst)
        {
            result = true;
            break;
        }

        for (int i = 0; i < 4; ++ i)
        {
            point p2 = p + UNIT[i];
            if (b.can_go(p2, id) && is_clean(b[p2]))
            {
                b[p2] |= VISITED;
                q.push(p2);
            }
        }
    }
    //pretty_print(std::cout, b);
    clean_marks(b);
    return result;
}