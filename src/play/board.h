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

#include <wss_assert.h>

#include <fmt/printf.h>
#include <gsl/string_span>

#include <array>
#include <optional>
#include <unordered_map>

template<typename T>
class board {
public:
    explicit board(int init_edge)
        : _edge{init_edge}
        , _cells{std::make_unique<T[]>(_edge * _edge)}  // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    {
    }

    [[nodiscard]] auto size() const -> int
    {
        return _edge;
    }

    [[nodiscard]] auto cell(coord c) const -> T const&
    {
        WSS_ASSERT(c[0] >= 0);
        WSS_ASSERT(c[0] < _edge);
        WSS_ASSERT(c[1] >= 0);
        WSS_ASSERT(c[1] < _edge);

        return _cells.get()[c[0] + c[1] * _edge];
    }

    auto cell(coord c) -> T&
    {
        WSS_ASSERT(c[0] >= 0);
        WSS_ASSERT(c[0] < _edge);
        WSS_ASSERT(c[1] >= 0);
        WSS_ASSERT(c[1] < _edge);

        return _cells.get()[c[0] + c[1] * _edge];
    }

private:
    int _edge;
    std::unique_ptr<T[]> _cells;  // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
};

template<typename CellType, typename TextToCell>
auto make_board(
        std::vector<std::vector<char>> const& lines,
        TextToCell const& mapping) -> std::optional<board<CellType>>
{
    auto const num_lines{ssize(lines)};
    constexpr auto max_num_lines{std::numeric_limits<int>::max()};

    // LCOV_EXCL_START - input would bloat repo
    if (num_lines > max_num_lines) {
        fmt::print(stderr, "error: board is too big; lines={}; maximum={}\n", num_lines, max_num_lines);
        return std::nullopt;
    }
    // LCOV_EXCL_STOP

    auto edge{static_cast<int>(num_lines)};
    board<CellType> result{edge};

    for (auto row_index{0}; row_index != edge; ++row_index) {
        auto const& line{lines[row_index]};
        auto const num_fields{ssize(line)};
        if (num_fields != edge) {
            fmt::print(stderr, "error: input row #{} has {} fields, expected {}.\n", row_index + 1, num_fields, edge);
            return std::nullopt;
        }

        for (auto column_index{0}; column_index != edge; ++column_index) {
            auto const field{line[column_index]};
            auto const cell_found{mapping(field)};
            if (!cell_found) {
                fmt::print(
                        stderr,
                        "Unrecognised field, '{}', in row #{}, column #{}.\n",
                        static_cast<char>(field),
                        row_index + 1, column_index + 1);
                return std::nullopt;
            }

            result.cell(coord{column_index, row_index}) = *cell_found;
        }
    }

    return result;
}

template<typename CellType, typename TextToCell>
auto load_board(
        gsl::cstring_span<> filename,
        TextToCell const& mapping) -> std::optional<board<CellType>>
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
    auto const edge = std::ssize(b);
    for (auto row = 0; row != edge; ++row) {
        for (auto column = 0; column != row; ++column) {
            std::swap(b.cell(coord{column, row}), b.cell(coord{row, column}));
        }
    }
}

#endif  // WSS_BOARD_H
