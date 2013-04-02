#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <cstdlib>
#include <sys/time.h>
#include "print.hpp"
#include "point.hpp"
#include "board.hpp"
#include "walker.hpp"
#include "solvers.hpp"
#include "solvers/pruning.hpp"

typedef result (*solver)(board&, const problem&, const pruning_options&);

struct rand_options
{
    int width;
    int height;
    int agents;
};

struct program_options
{
    bool random;
    std::string file;
    rand_options rand_opts;
    pruning_options prun_opts;

    program_options()
    : random(false)
    , file("last")
    { }
};


template <typename Solver>
void solve(Solver s, const problem& p, const pruning_options& opts)
{
    board b(p.width, p.height);
    std::list<walker> walkers;
    for (unsigned i = 0; i < p.agents.size(); ++ i)
    {
        int id = i + 1;
        b[p.agents[i].src].color = id;
        b[p.agents[i].src].set(SOURCE);
        b[p.agents[i].dest].color = id;
        b[p.agents[i].dest].set(DEST);
    }

    pretty_print(std::cout, b);
    result res = s(b, p, opts);
    std::cerr << "Calls: " << res.calls << std::endl;
    std::cerr << "Solution: " << (res.found ? "YES" : "NO") << std::endl;
    pretty_print(std::cout, b);
}

void init_rand();
//solver choose_solver(int argc, char* argv[]);
void parse_input(std::istream& input, problem& p);
void random_problem(int N, problem& p);
void dump_problem(std::ostream& stream, const problem& p);
void dump_problem_to_file(const std::string& file, const problem& p);

void process_args(int argc, char* argv[], program_options&  opts);


int main(int argc, char* argv[])
{
    init_rand();
    program_options opts;
    ++ argv, -- argc;
    process_args(argc, argv, opts);

    problem p;
    if (opts.random)
    {
        p.width = opts.rand_opts.width;
        p.height = opts.rand_opts.height;
        random_problem(opts.rand_opts.agents, p);
    }
    else
        parse_input(std::cin, p);
    dump_problem_to_file(opts.file, p);
    solver s = solvers::pruning;//choose_solver(argc, argv);
    solve(s, p, opts.prun_opts);
    return 0;
}

void process_args(int argc, char* argv[], program_options&  opts)
{
    while (argc-- > 0)
    {
        std::string arg(*argv++);
        if (arg == "-r" || arg == "--random")
        {
            if (argc >= 3)
            {
                opts.random = true;
                opts.rand_opts.width = atoi(*argv++);
                opts.rand_opts.height = atoi(*argv++);
                opts.rand_opts.agents = atoi(*argv++);
                argc -= 3;
            }
            else
            {
                std::cerr << "Error: " << arg << " requires 3 arguments\n";
                std::exit(-1);
            }
        }
        else if (arg == "--skip-adjacent")
            opts.prun_opts.elim_adjacent = true;
        else if (arg == "--no-skip-adjacent")
            opts.prun_opts.elim_adjacent = false;
        else if (arg == "--find-cut-vertices")
            opts.prun_opts.cut_vertices = true;
        else if (arg == "--no-find-cut-vertices")
            opts.prun_opts.cut_vertices = false;
        else if (arg == "--reach-bfs")
            opts.prun_opts.per_agent_reachability = true;
        else if (arg == "--no-reach-bfs")
            opts.prun_opts.per_agent_reachability = false;
        else if (arg == "--compute-components")
            opts.prun_opts.components_reachability = true;
        else if (arg == "--no-compute-components")
            opts.prun_opts.components_reachability = false;
        else
        {
            std::size_t n = arg.find('=');
            if (n != std::string::npos)
            {
                std::string name = arg.substr(0, n), value = arg.substr(n + 1);
                if (name == "--direction")
                {
                    if (value == "fixed")
                        opts.prun_opts.direction_chooser = dir_chooser::FIXED;
                    else if (value == "simple")
                        opts.prun_opts.direction_chooser = dir_chooser::SIMPLE;
                    else if (value == "bfs")
                        opts.prun_opts.direction_chooser = dir_chooser::BFS;
                    else if (value == "A*")
                        opts.prun_opts.direction_chooser = dir_chooser::ASTAR;
                    else
                        std::cerr << "Unknown direction chooser: "
                                  << value << "\n";
                }
                else
                    std::cerr << "Unknown option " << name <<"; ignoring\n";
            }
            else
                std::cerr << "Unknown option " << arg <<"; ignoring\n";
        }
    }
}

/*
solver choose_solver(int argc, char* argv[])
{
    if (argc < 1)
        return solvers::simple;
    std::string opt(argv[0]);
    if (opt == "-s" || opt == "--simple")
        return solvers::simple;
    if (opt == "-p" || opt == "--pruning")
        return solvers::pruning;
    else if (opt == "-P" || opt == "--parallel")
        return solvers::parallel;
    else
    {
        std::cerr << "Invalid solver" << std::endl;
        std::exit(-1);
    }
    return NULL;
}*/

void parse_input(std::istream& input, problem& p)
{
    input >> p.width >> p.height;
    int N;
    input >> N;
    p.agents.resize(N);
    for (int i = 0; i < N; ++ i)
        input >> p.agents[i].src >> p.agents[i].dest;
}

point rand_point(int w, int h, array2d<int>& board)
{
    point p;
    while (true)
    {
        p.x = rand() % w;
        p.y = rand() % h;
        if (board[p] == 0)
        {
            board[p] = 1;
            break;
        }
    }
    return p;
}

void init_rand()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
}

void random_problem(int N, problem& p)
{
    array2d<int> board(p.width, p.height);
    p.agents.resize(N);

    for (int i = 0; i < N; ++ i)
    {
        p.agents[i].src = rand_point(p.width, p.height, board);
        p.agents[i].dest = rand_point(p.width, p.height, board);
    }
}

void dump_problem(std::ostream& stream, const problem& p)
{
    stream << p.width << " " << p.height << '\n'
           << p.agents.size() << '\n';
    typedef std::vector<agent>::const_iterator const_iter;
    for (const_iter i = p.agents.begin(); i != p.agents.end(); ++ i)
    {
        stream << i->src.x << ' ' << i->src.y << "   "
               << i->dest.x << ' ' << i->dest.y << '\n';
    }
}

void dump_problem_to_file(const std::string& file, const problem& p)
{
    std::ofstream out(file.c_str(), std::ios_base::out);
    dump_problem(out, p);
}

