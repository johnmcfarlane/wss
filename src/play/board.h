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

#ifndef WSS_BOARD_H
#define WSS_BOARD_H

#include "coord.h"
#include "grid.h"
#include "load_buffer.h"

#include <ssize.h>
#include <wss_assert.h>

#include <fmt/printf.h>
#include <gsl/string_span>

#include <array>
#include <optional>
#include <unordered_map>

template<typename T>
class board {
public:
    board(board&&) = default;

    explicit board(int init_edge)
            :edge{init_edge}, cells{std::make_unique<T[]>(edge*edge)} { }

    int size() const
    {
        return edge;
    }

    T const& cell(coord c) const
    {
        WSS_ASSERT(c[0]>=0);
        WSS_ASSERT(c[0]<edge);
        WSS_ASSERT(c[1]>=0);
        WSS_ASSERT(c[1]<edge);

        return cells.get()[c[0]+c[1]*edge];
    }

    T& cell(coord c)
    {
        WSS_ASSERT(c[0]>=0);
        WSS_ASSERT(c[0]<edge);
        WSS_ASSERT(c[1]>=0);
        WSS_ASSERT(c[1]<edge);

        return cells.get()[c[0]+c[1]*edge];
    }

private:
    int edge;
    std::unique_ptr<T[]> cells;
};

template<typename CellType, typename TextToCell>
std::optional<board<CellType>> make_board(
        std::vector<std::vector<char>> const& lines,
        TextToCell const& mapping)
{
    auto edge{ssize(lines)};
    board<CellType> result{int(edge)};

    for (auto row_index{0}; row_index!=edge; ++row_index) {
        auto const& line{lines[row_index]};
        auto const num_fields{ssize(line)};
        if (num_fields!=edge) {
            fmt::print(stderr,
                    "error: input row #{} has {} fields, expected {}.\n",
                    row_index+1, num_fields, edge);
            return std::nullopt;
        }

        for (auto column_index{0}; column_index!=edge; ++column_index) {
            auto const field{line[column_index]};
            auto const cell_found{mapping(field)};
            if (!cell_found) {
                fmt::print(
                        "Unrecognised field, '{}', in row #{}, column #{}.\n",
						(char)field,
                        row_index+1, column_index+1);
                return std::nullopt;
            }

            result.cell(coord{column_index,row_index}) = *cell_found;
        }
    }

    return result;
}

template<typename CellType, typename TextToCell>
std::optional<board<CellType>> load_board(
        gsl::cstring_span<> filename,
        TextToCell const& mapping)
{
    auto const buffer{load_buffer(filename)};
    if (!buffer) {
        return std::nullopt;
    }
    
    auto const fields{parse_grid(*buffer)};
    
    return make_board<CellType>(fields, mapping);
}

template<typename T>
void transpose(board<T>& b)
{
    auto const edge = ssize(b);
    for (auto row = 0; row != edge; ++row)
    {
        for (auto column = 0; column != row; ++ column)
        {
            std::swap(b.cell(coord{column, row}), b.cell(coord{row, column}));
        }
    }
}

#endif //WSS_BOARD_H
