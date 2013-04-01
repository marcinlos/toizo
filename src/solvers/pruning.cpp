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

struct bfs_data
{
    bool visited;

    bfs_data()
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

struct vertex_data
{
    int component;
    int neighbours[4];

    vertex_data()
    : component(0)
    {
        for (int i = 0; i < 4; ++ i)
            neighbours[i] = 0;
    }

    void clear()
    {
        component = 0;
        for (int i = 0; i < 4; ++ i)
            neighbours[i] = 0;
    }

    bool reachable_from(const vertex_data& other) const
    {
        for (int i = 0; i < 4 && neighbours[i] != 0; ++ i)
        {
            for (int j = 0; j < 4 && other.neighbours[j] != 0; ++ j)
                if (neighbours[i] == other.neighbours[j])
                    return true;
        }
        return false;
    }

    bool visited() const
    {
        return component != 0;
    }

    void add_neighbour(int id)
    {
        int* n = neighbours;
        while (*n != 0)
        {
            if (*n == id)
                return;
            ++n;
        }
        *n = id;
    }
};


template <typename Iter>
struct pruning_impl_
{
    Iter begin, end;
    board& board_;
    array2d<vertex_data> data;
    int calls;

    pruning_impl_(Iter begin, Iter end)
    : begin(begin), end(end), board_(begin->b)
    , data(begin->b.width(), begin->b.height())
    , calls(0)
    { }

    result compute()
    {
        std::cout << "6, 0: " << board_[0][6].flags << std::endl;
        bool res = run(begin);
        result r = {calls, res};
        return r;
    }

    void update_counter_()
    {
        ++ calls;
        //usleep(200000);
        //pretty_print(std::cout, board_);
        if (calls % 100000 == 0)
        {
            //pretty_print(std::cout, board_);
            std::cout << '\r' << calls << std::flush;
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
        if (! old_remaining_agents_reachable(it))
            return false;
        dir d = UP;
        for (int i = 0; i < 4; ++ i, ++ d)
        {
            if (move(it->pos, d) == it->dest)
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
        compute_connected_components();
        bool unreachable = false;
        while (it != end)
        {
            point pos = it->pos, dest = it->dest;
            if (!data[dest].reachable_from(data[pos]))
            {
                unreachable = true;
                dir d = UP;
                for (int i = 0; i < 4; ++ i, ++ d)
                {
                    if (move(pos, d) == dest)
                    {
                        unreachable = false;
                        break;
                    }
                }
                if (unreachable)
                    break;
            }
            ++ it;
        }
        clear_data();
        return !unreachable;
    }

    bool old_remaining_agents_reachable(Iter it)
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
                if (board_[q].color == id && board_[q].is(USED))
                    ++ count;
            }
        }
        return count;
    }

    void compute_connected_components()
    {
        int id = 1;
        for (int i = 0; i < board_.height(); ++ i)
        {
            for (int j = 0; j < board_.width(); ++ j)
            {
                //if (! data[i][j].visited() && board_[i][j].color == EMPTY)
                /*if (calculation_needed_(i, j))
                    calculate_component_(point(j, i), id ++);*/
                if (! data[i][j].visited())
                {
                    if (board_[i][j].color == EMPTY)
                        calculate_component_(point(j, i), id++);
//                    else if (board_[i][j].is(DEST))
//                        calculate_component_(point(j, i), id++);

                }
            }
        }
        //std::cout << id << std::endl;
    }

    bool calculation_needed_(int i, int j)
    {
        return ! data[i][j].visited() &&
                (board_[i][j].color == EMPTY || (board_[i][j].is(DEST)));
    }

    struct printer_
    {
        void operator ()(std::ostream& stream, const vertex_data& v)
        {
            if (v.component != 0)
            {
                stream << COLORS[1 + (v.component - 1) % COLOR_NUM];
                stream << ' ' << v.component;
            }
            else if (v.neighbours[0] != 0)
                stream << v.neighbours[0] << v.neighbours[1];
            else
                stream << "  ";
            stream << RESET;
        }
    };

    void calculate_component_(const point& src, int id)
    {
        data[src].component = id;
        std::queue<point> q;
        q.push(src);
        while (! q.empty())
        {
//            printer_ pr;
//            pretty_print(std::cout, board_);
//
//            print_array(pr, std::cout, data);
//            usleep(200000);
            point p = q.front();
            q.pop();
            dir d = UP;
            for (int i = 0; i < 4; ++ i)
            {
                point p2 = move(p, d);
                ++d;
                if (! board_.inside(p2))
                    continue;
                if (!data[p2].visited())
                {
                    if (board_[p2].color == EMPTY)
                    {
                        data[p2].component = id;
                        q.push(p2);
                    }
                    else if (board_[p2].is(DEST | OCCUPIED))
                        data[p2].add_neighbour(id);
                }
                //else
                //    data[p2].add_neighbour(id);

            }
        }
    }

    void clear_data()
    {
        for (int i = 0; i < board_.height(); ++ i)
        {
            for (int j = 0; j < board_.width(); ++ j)
                data[i][j].clear();
        }
    }

    dir find_best_direction(const point& src, const point& dest, int id)
    {
        array2d<bfs_data> v(board_.width(), board_.height());
        std::queue<point> q;
        v[dest].visit();
        q.push(dest);
        while (! q.empty())
        {
            point p = q.front();
            q.pop();
            dir d = approx_dir(p, dest);
            for (int i = 0; i < 4; ++ i)
            {
                point p2 = move(p, d);
                if (p2 == src)
                    return reverse(d);
                if (board_.can_enter(p2, id) && !v[p2].visited)
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
    result operator()(Iter begin, Iter end)
    {
        pruning_impl_<Iter> s(begin, end);
        return s.compute();
    }
};


result pruning(board& b, const problem& p)
{
    return detail::solve_aux(pruning_(), b, p);
}

}
