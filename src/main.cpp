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


typedef result (*solver)(board&, const problem&);

template <typename Solver>
void solve(Solver s, const problem& p)
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
    result res = s(b, p);
    std::cerr << "Calls: " << res.calls << std::endl;
    std::cerr << "Solution: " << (res.found ? "YES" : "NO") << std::endl;
    pretty_print(std::cout, b);
}

solver choose_solver(int argc, char* argv[]);
void parse_input(std::istream& input, problem& p);
void random_problem(int N, problem& p);
void dump_problem(std::ostream& stream, const problem& p);
void dump_problem_to_file(const std::string& file, const problem& p);


int main(int argc, char* argv[])
{
    problem p;
    -- argc, ++ argv;
    bool rand = true;
    if (argc > 0)
    {
        std::string opt(argv[0]);
        if (opt == "-r" || opt == "--random")
        {
            p.width = atoi(argv[1]);
            p.height = atoi(argv[2]);
            int n = atoi(argv[3]);
            std::cout << "Generatiing " << p.width << "x" << p.height
                      << " problem with " << n << " agents" << std::endl;
            argc -= 4;
            argv += 4;
            random_problem(n, p);
        }
        else rand = false;
    }
    if (! rand)
        parse_input(std::cin, p);
    dump_problem_to_file("last", p);
    solver s = choose_solver(argc, argv);
    solve(s, p);
    return 0;
}

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
}

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

void random_problem(int N, problem& p)
{
    array2d<int> board(p.width, p.height);
    p.agents.resize(N);
    timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
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

