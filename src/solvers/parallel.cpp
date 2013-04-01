#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"


namespace solvers
{

template <typename Iter>
struct parallel_aux_
{
    Iter begin, end;

    parallel_aux_(Iter begin, Iter end)
    : begin(begin), end(end)
    { }

    bool search(Iter next)
    {
        walker& w = *next;
        //pretty_print(std::cout, w.b);

        dir d = approx_dir(w.pos, w.dest);
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (w.go(d))
            {
                if (w.done())
                {
                    if (all_done())
                        return true;
                }
                if (search(inc(next)))
                    return true;
                else
                    w.back();
            }
        }
        return false;
    }

    Iter inc(Iter i) const
    {
        do
        {
            if (++ i == end)
                i = begin;
        }
        while (i->done());
        return i;
    }

    bool all_done() const
    {
        for (Iter i = begin; i != end; ++ i)
        {
            if (! i->done())
                return false;
        }
        return true;
    }
};


struct parallel_
{
    int N;

    parallel_(int N): N(N) { }

    template <typename Iter>
    result operator()(Iter begin, Iter end)
    {
        parallel_aux_<Iter> p(begin, end);
        result r = {0, p.search(begin)};
        return r;
    }
};


result parallel(board& b, const problem& p)
{
    return detail::solve_aux(parallel_(p.agents.size()), b, p);
}

}
