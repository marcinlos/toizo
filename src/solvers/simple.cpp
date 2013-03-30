#include <vector>
#include <list>
#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"

namespace solvers
{

template <typename Iter>
struct simple_impl_
{
    Iter begin, end;
    unsigned long calls;

    simple_impl_(Iter begin, Iter end)
    : begin(begin), end(end), calls(0)
    { }

    bool compute()
    {
        bool res = run(begin);
        std::cout << "Calls: " << calls << std::endl;
        return res;
    }

    bool run(Iter it)
    {
        ++ calls;
        if (calls % 10000000 == 0)
            std::cout << calls << std::endl;
        if (it == end)
            return true;
        if (it->done())
            return run(++it);
        dir d = approx_dir(it->pos, it->dest);
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (it->go(d))
            {
                if (run(it))
                    return true;
                else
                    it->back();
            }
        }
        return false;
    }

};

struct simple_
{
    template <typename Iter>
    bool operator()(Iter begin, Iter end)
    {
        simple_impl_<Iter> s(begin, end);
        return s.compute();
    }
};


bool simple(board& b, const problem& p)
{
    return detail::solve_aux(simple_(), b, p);
}

}
