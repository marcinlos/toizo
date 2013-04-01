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
        if (! remaining_agents_reachable(it))
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
        Iter it2 = it;
        bool unr = false;
        while (it2 != end)
        {
            if (! reachable(board_, it2->pos, it2->dest, it2->id))
            {
                unr = true;
                break;
            }
            ++ it2;
        }
        //return true;

        compute_connected_components();
        bool unreachable = false;
        while (it != end)
        {
            point pos = it->pos, dest = it->dest;
            if (!data[dest].reachable_from(data[pos]))
            {
                unreachable = true;
                break;
            }
            ++ it;
        }
        if (unr != unreachable)
        {
            std::cout << "No kurwa :/ unr=" << unr << ", unreachable=" << unreachable << std::endl;
            if (unr)
                std::cout << "Looser: " << it2->id << " " << std::endl;
            if (unreachable)
            {
                std::cout << "Looser: " << it->id << " " << std::endl;
                std::cout << "Src: " << data[it->pos].component << " / "
                        << data[it->pos].neighbours[0]
                        << data[it->pos].neighbours[1]
                        << data[it->pos].neighbours[2] << std::endl;
                std::cout << "Dst: " << data[it->dest].component << " / "
                        << data[it->dest].neighbours[0]
                        << data[it->dest].neighbours[1]
                        << data[it->dest].neighbours[2]<< std::endl;
            }
                print_array(printer_(), std::cout, data);
            pretty_print(std::cout, board_);
        }
        clear_data();
        return !unreachable;

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

    void compute_connected_components()
    {
        int id = 1;
        for (int i = 0; i < board_.height(); ++ i)
        {
            for (int j = 0; j < board_.width(); ++ j)
            {
                if (! data[i][j].visited() && board_[i][j].color == 0 && (board_[i][j].extra & DEST) == 0)
                    calculate_component_(point(j, i), id ++);
            }
        }
        //std::cout << id << std::endl;
    }

    struct printer_
    {
        void operator ()(std::ostream& stream, const vertex_data& v)
        {
            if (v.component != 0)
                stream << COLORS[1 + (v.component - 1) % COLOR_NUM];
            //else if (v.neighbour != 0)
            //    stream << COLORS[2 + (v.neighbour - 1) % (COLOR_NUM - 1)];
            //stream << v.component << v.neighbour;
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
                    if (board_[p2].color == 0)
                    {
                        if (board_[p2].extra == 0)
                        {
                            data[p2].component = id;
                            q.push(p2);
                        }
                        else
                            data[p2].add_neighbour(id);
                    }
                    else if (board_[p2].is(OCCUPIED))
                    {
                        data[p2].add_neighbour(id);
                    }
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
