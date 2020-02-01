//
// Created by john on 27/10/2019.
//

#ifndef WSS_BOARD_H
#define WSS_BOARD_H

#include "coord.h"
#include "csv.h"
#include "load_buffer.h"

#include <ssize.h>

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
        // LCOV_EXCL_START
        Expects(c[0]>=0);
        Expects(c[0]<edge);
        Expects(c[1]>=0);
        Expects(c[1]<edge);
        // LCOV_EXCL_STOP
        return cells.get()[c[0]+c[1]*edge];
    }

    T& cell(coord c)
    {
        // LCOV_EXCL_START
        Expects(c[0]>=0);
        Expects(c[0]<edge);
        Expects(c[1]>=0);
        Expects(c[1]<edge);
        // LCOV_EXCL_STOP
        return cells.get()[c[0]+c[1]*edge];
    }

private:
    int edge;
    std::unique_ptr<T[]> cells;
};

template<typename CellType, typename TextToCell>
std::optional<board<CellType>> make_board(
        std::vector<std::vector<gsl::cstring_span<>>> const& lines,
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
                        std::string(std::begin(field), std::end(field)),
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
    
    auto const fields{parse_csv(*buffer)};
    
    return make_board<CellType>(fields, mapping);
}

#endif //WSS_BOARD_H
