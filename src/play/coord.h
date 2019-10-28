//
// Created by john on 28/10/2019.
//

#ifndef WSS_COORD_H
#define WSS_COORD_H

#include <array>

using coord = std::array<int, 2>;

constexpr coord operator+(coord const& lhs, coord const& rhs)
{
    return {lhs[0]+rhs[0], lhs[1]+rhs[1]};
}

constexpr coord operator-(coord const& lhs, coord const& rhs)
{
    return {lhs[0]-rhs[0], lhs[1]-rhs[1]};
}

template<typename Scalar>
constexpr coord operator*(coord const& lhs, Scalar const& rhs)
{
    return {lhs[0]*rhs, lhs[1]*rhs};
}

constexpr coord& operator+=(coord& lhs, coord const& rhs)
{
    lhs[0]+=rhs[0];
    lhs[1]+=rhs[1];
    return lhs;
}

constexpr coord& operator-=(coord& lhs, coord const& rhs)
{
    lhs[0]-=rhs[0];
    lhs[1]-=rhs[1];
    return lhs;
}

#endif //WSS_COORD_H
