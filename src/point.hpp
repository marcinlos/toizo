#ifndef POINT_HPP_
#define POINT_HPP_

#include <cstdlib>
#include <iostream>


enum dir { UP, RIGHT, DOWN, LEFT, NO_DIR };


inline dir& operator ++ (dir& d)
{
    return d = static_cast<dir>((d + 1) & 3);
}

inline dir reverse(dir d)
{
    return static_cast<dir>(d ^ 2);
}

struct point
{
    int x, y;

    point()
    : x(0), y(0)
    { }

    point(int x, int y)
    : x(x), y(y)
    { }

    point& operator += (const point& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    point& operator -= (const point& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
};

inline point operator + (const point& p, const point& q)
{
    point r(p);
    return r += q;
}

inline point operator - (const point& p, const point& q)
{
    point r(p);
    return r -= q;
}

inline bool operator == (const point& p, const point& q)
{
    return p.x == q.x && p.y == q.y;
}

inline std::istream& operator >> (std::istream& stream, point& p)
{
    return stream >> p.x >> p.y;
}

inline std::ostream& operator << (std::ostream& stream, const point& p)
{
    return stream << "(" << p.x << ", " << p.y << ")";
}

const point UNIT[] = { point(0, 1), point(1, 0), point(0, -1), point(-1, 0) };

const point VEC_UP = UNIT[0];
const point VEC_RIGHT = UNIT[1];
const point VEC_DOWN = UNIT[2];
const point VEC_LEFT = UNIT[3];


inline dir approx_dir(const point& src, const point& dest)
{
    point dist = dest - src;
    if (abs(dist.x) > abs(dist.y))
        return dist.x > 0 ? RIGHT : LEFT;
    else
        return dist.y > 0 ? UP : DOWN;
}

inline point move(const point& p, dir d)
{
    return p + UNIT[d];
}



#endif /* POINT_HPP_ */
