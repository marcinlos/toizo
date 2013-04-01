#ifndef DATA_HPP_
#define DATA_HPP_

#include "point.hpp"

struct agent
{
    point src, dest;
};

struct problem
{
    int width, height;
    std::vector<agent> agents;
};

struct result
{
    long calls;
    bool found;
};

#endif /* DATA_HPP_ */
