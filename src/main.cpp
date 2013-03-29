#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
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

solver choose_solver(int args, char* argv[])
{
    return simple_solve;
}

void parse_input(std::istream& input, problem& p);


int main(int argc, char* argv[])
{
    problem p;
    parse_input(std::cin, p);
    solver s = choose_solver(argc, argv);
    solve(s, p);
    return 0;
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

