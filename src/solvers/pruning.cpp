#include <vector>
#include <list>
#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"
#include <unistd.h>

namespace solvers
{


template <typename Iter>
struct pruning_impl_
{
    Iter begin, end;
    board& board_;
    int calls;

    pruning_impl_(Iter begin, Iter end)
    : begin(begin), end(end), board_(begin->b), calls(0)
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
            //pretty_print(std::cout, it->b);
            //usleep(200000);
        if (calls % 100000 == 0)
        {
            std::cout << calls << std::endl;
        }
        if (it == end)
            return true;
        if (it->done())
            return run(++it);
        if (adjacent_count(it->pos, it->id) > 1)
            return false;
        if (! remaining_agents_reachable(it))
            return false;
        dir d = UP;
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (it->pos + UNIT[d] == it->dest)
            {
                it->go(d);
                if (run(it))
                    return true;
                else
                {
                    it->back();
                    return false;
                }
            }
        }
        d = approx_dir(it->pos, it->dest);
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            //if (adjacent_count(it->pos + UNIT[d], it->id) > 1)
            //    continue;
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

    /**
     * Checks whether all the remaining agents (that is, from the one pointed
     * to by this iterator inclusive to the end) still have the possibility
     * to reach their destination.
     */
    bool remaining_agents_reachable(Iter it)
    {
        while (it != end)
        {
            if (! reachable(board_, it->pos, it->dest, it->id))
                return false;
            ++ it;
        }
        return true;
    }

    /**
     * Counts vertices adjacent to p having color id.
     */
    int adjacent_count(const point& p, int id)
    {
        int count = 0;
        dir d = UP;
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            point q = p + UNIT[d];
            if (board_.inside(q))
            {
                if (board_[q].color == id)
                    ++ count;
            }
        }
        return count;
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
