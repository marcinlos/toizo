#include <vector>
#include <list>
#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"

namespace solvers
{

template <typename Iter>
struct pruning_impl_
{
    Iter begin, end;
    int calls;

    pruning_impl_(Iter begin, Iter end)
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
        if (it == end)
            return true;
        if (it->done())
            return run(++it);
        if (! check_bfs(it))
            return false;
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

    bool check_bfs(Iter it)
    {
        while (it != end)
        {
            if (! reachable(it->b, it->pos, it->dest, it->id))
                return false;
            ++ it;
        }
        return true;
    }

};

struct pruning_
{
    template <typename Iter>
    bool operator()(Iter begin, Iter end)
    {
        pruning_impl_<Iter> s(begin, end);
        return s.compute();
    }
};


bool pruning(board& b, const problem& p)
{
    return detail::solve_aux(pruning_(), b, p);
}

}
