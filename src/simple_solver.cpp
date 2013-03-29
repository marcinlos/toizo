#include <vector>
#include <list>
#include "board.hpp"
#include "walker.hpp"
#include "solvers.hpp"

template <typename Iter>
bool find_path(Iter begin, Iter end)
{
    if (begin == end)
        return true;
    walker& w = *begin;
    if (w.done())
    {
        return find_path(++ begin, end);
    }
    dir d = approx_dir(w.pos, w.dest);
    for (int i = 0; i < 4; ++ i, ++ d)
    {
        if (w.go(d))
        {
            if (find_path(begin, end))
                return true;
            else
                w.back();
        }
    }
    return false;
}


bool simple_solve(board& b, const problem& p)
{
    std::list<walker> walkers;
    typedef std::vector<agent>::const_iterator iter;
    int id = 1;
    for (iter i = p.agents.begin(); i != p.agents.end(); ++ i)
        walkers.push_back(walker(id++, b, i->src, i->dest));

    return find_path(walkers.begin(), walkers.end());
}

