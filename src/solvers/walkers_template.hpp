#ifndef WALKERS_TEMPLATE_HPP_
#define WALKERS_TEMPLATE_HPP_

#include <list>
#include "../board.hpp"
#include "../walker.hpp"
#include "../solvers.hpp"

namespace solvers
{
namespace detail
{

template <typename Solver>
result solve_aux(Solver s, board& b, const problem& p)
{
    std::list<walker> walkers;
    typedef std::vector<agent>::const_iterator iter;
    int id = 1;
    for (iter i = p.agents.begin(); i != p.agents.end(); ++ i)
        walkers.push_back(walker(id++, b, i->src, i->dest));

    return s(walkers.begin(), walkers.end());
}


} // detail
} // solvers


#endif /* WALKERS_TEMPLATE_HPP_ */
