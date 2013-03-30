#ifndef SOLVERS_HPP_
#define SOLVERS_HPP_

#include <vector>
#include "data.hpp"
#include "board.hpp"

#define SOLVER(s) bool s(board&, const problem&)

namespace solvers
{

SOLVER(simple);
SOLVER(prefer_outside);
SOLVER(parallel);
SOLVER(pruning);

}

#endif /* SOLVERS_HPP_ */
