#ifndef PRUNING_HPP_
#define PRUNING_HPP_

struct dir_chooser
{
    enum type { FIXED, SIMPLE, BFS, ASTAR };
};

struct pruning_options
{
    bool elim_adjacent;
    bool cut_vertices;
    bool per_agent_reachability;
    bool components_reachability;
    dir_chooser::type direction_chooser;

    pruning_options()
    : elim_adjacent(true)
    , cut_vertices(true)
    , per_agent_reachability(false)
    , components_reachability(false)
    , direction_chooser(dir_chooser::BFS)
    { }
};


#endif /* PRUNING_HPP_ */
