#include <vector>
#include <list>
#include <queue>
#include "../array2d.hpp"
#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"
#include "walkers_template.hpp"
#include "../print_array.hpp"
#include <unistd.h>

namespace solvers
{

struct vertex_data
{
    bool visited;

    vertex_data()
    : visited(false)
    { }

    bool visit()
    {
        if (visited)
            return false;
        else
        {
            visited = true;
            return true;
        }
    }
};


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

    void update_counter_()
    {
        ++ calls;
        //usleep(200000);
        //pretty_print(std::cout, board_);
        if (calls % 100000 == 0)
        {
            pretty_print(std::cout, board_);
            std::cout << calls << std::endl;
        }
    }

    bool run(Iter it)
    {
        update_counter_();
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
        //d = approx_dir(it->pos, it->dest);
        d = find_best_direction(it->pos, it->dest, it->id);
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
     * to by this izdterator inclusive to the end) still have the possibility
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
            point q = move(p, d);
            if (board_.inside(q))
            {
                if (board_[q].color == id)
                    ++ count;
            }
        }
        return count;
    }

    dir find_best_direction(const point& src, const point& dest, int id)
    {
        array2d<vertex_data> v(board_.width(), board_.height());
        std::queue<point> q;
        v[dest].visit();
        q.push(dest);
        while (! q.empty())
        {
            point p = q.front();
            q.pop();
            dir d = UP;
            for (int i = 0; i < 4; ++ i)
            {
                point p2 = move(p, d);
                if (p2 == src)
                    return reverse(d);
                if (board_.can_go(p2, id) && !v[p2].visited)
                {
                    v[p2].visited = true;
                    q.push(p2);
                }
                ++d;
            }
        }
        return NO_DIR;
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
