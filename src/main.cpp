#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include "print.hpp"
#include "point.hpp"
#include "board.hpp"
#include "walker.hpp"
#include "solvers.hpp"

typedef bool (*solver)(board&, const problem&);

template <typename Solver>
void solve(Solver s, const problem& p)
{
    board b(p.width, p.height);
    std::list<walker> walkers;
    for (unsigned i = 0; i < p.agents.size(); ++ i)
    {
        int id = i + 1;
        b[p.agents[i].src] = id | SOURCE;
        b[p.agents[i].dest] = (id << ID_BITS) | DEST;
    }

    pretty_print(std::cout, b);
    if (s(b, p))
        std::cout << "Solution found!" << std::endl;
    else
        std::cout << "No solution found" << std::endl;
    pretty_print(std::cout, b);
}

solver choose_solver(int argc, char* argv[]);
void parse_input(std::istream& input, problem& p);


int main(int argc, char* argv[])
{
    problem p;
    parse_input(std::cin, p);
    solver s = choose_solver(argc, argv);
    solve(s, p);
    return 0;
}

solver choose_solver(int argc, char* argv[])
{
    if (argc < 2)
        return solvers::simple;
    std::string opt(argv[1]);
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
        std::cin >> p.agents[i].src >> p.agents[i].dest;
}

