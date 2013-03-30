#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"

namespace solvers
{

struct prefer_outside_
{
    template <typename Iter>
    bool operator ()(Iter begin, Iter end)
    {
        if (begin == end)
            return true;
        walker& w = *begin;
        if (w.done())
        {
            return (*this)(++ begin, end);
        }
        dir d = approx_dir(w.pos, w.dest);
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (w.go(d))
            {
                if ((*this)(begin, end))
                    return true;
                else
                    w.back();
            }
        }
        return false;
    }
};

bool prefer_outside(board& b, const problem& p)
{
    return detail::solve_aux(prefer_outside_(), b, p);
}

}
