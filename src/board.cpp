#include <queue>
#include "board.hpp"


static void clean_marks(board& b)
{
    for (int i = 0; i < b.height(); ++ i)
    {
        for (int j = 0; j < b.width(); ++ j)
            b[i][j].clear(MARKED);
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
            if (b.can_enter(p2, id) && !b[p2].is(MARKED))
            {
                b[p2].set(MARKED);
                q.push(p2);
            }
        }
    }
    //pretty_print(std::cout, b);
    clean_marks(b);
    return result;
}
