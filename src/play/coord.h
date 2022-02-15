// Copyright 2019 John McFarlane
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WSS_COORD_H
#define WSS_COORD_H

#include <array>

using coord = std::array<int, 2>;

constexpr coord operator+(coord const& lhs, coord const& rhs)
{
    return {lhs[0] + rhs[0], lhs[1] + rhs[1]};
}

constexpr coord operator-(coord const& lhs, coord const& rhs)
{
    return {lhs[0] - rhs[0], lhs[1] - rhs[1]};
}

template<typename Scalar>
constexpr coord operator*(coord const& lhs, Scalar const& rhs)
{
    return {lhs[0] * rhs, lhs[1] * rhs};
}

constexpr coord& operator+=(coord& lhs, coord const& rhs)
{
    lhs[0] += rhs[0];
    lhs[1] += rhs[1];
    return lhs;
}

constexpr coord& operator-=(coord& lhs, coord const& rhs)
{
    lhs[0] -= rhs[0];
    lhs[1] -= rhs[1];
    return lhs;
}

inline void transpose(coord& c)
{
    std::swap(c[0], c[1]);
}

#endif  // WSS_COORD_H
