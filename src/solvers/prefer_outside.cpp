#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"

namespace solvers
{

struct prefer_outside_
{
    template <typename Iter>
    result operator ()(Iter begin, Iter end)
    {
        bool res = solve(begin, end);
        result r = {0, res};
        return r;
    }

    template <typename Iter>
    bool solve(Iter begin, Iter end)
    {
        if (begin == end)
            return true;
        walker& w = *begin;
        if (w.done())
        {
            return solve(++ begin, end);
        }
        dir d = approx_dir(w.pos, w.dest);
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (w.go(d))
            {
                if (solve(begin, end))
                    return true;
                else
                    w.back();
            }
        }
        return false;
    }
};

result prefer_outside(board& b, const problem& p)
{
    return detail::solve_aux(prefer_outside_(), b, p);
}

}
