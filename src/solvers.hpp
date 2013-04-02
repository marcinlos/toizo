#ifndef SOLVERS_HPP_
#define SOLVERS_HPP_

#include <vector>
#include "data.hpp"
#include "board.hpp"
#include "solvers/pruning.hpp"

#define SOLVER(s) result s(board&, const problem&)

namespace solvers
{

SOLVER(simple);
SOLVER(prefer_outside);
SOLVER(parallel);



result pruning(board&, const problem&, const pruning_options&);

}

#endif /* SOLVERS_HPP_ */
