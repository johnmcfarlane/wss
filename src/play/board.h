//
// Created by john on 27/10/2019.
//

#ifndef WSS_BOARD_H
#define WSS_BOARD_H

#include "csv.h"
#include "load_buffer.h"
#include "../common/ssize.h"

#include <fmt/printf.h>
#include <gsl/string_span>

#include <array>
#include <optional>
#include <unordered_map>

template<typename T>
class board {
public:
    board(board&&) = default;

    board(int init_edge)
            :edge{init_edge}, cells{std::make_unique<T[]>(edge*edge)} { }

    int size() const
    {
        return edge;
    }

    class iterator {
    public:
        iterator(T* p, int e)
                :pos(p), edge(e) { }

        iterator(iterator const& that) = default;

        friend bool operator!=(iterator lhs, iterator rhs)
        {
            Expects(lhs.edge==rhs.edge);
            return lhs.pos!=rhs.pos;
        }

        friend iterator& operator++(iterator& rhs)
        {
            std::advance(rhs.pos, rhs.edge);
            return rhs;
        }

    private:
        T* pos;
        int edge;
    };

    class const_iterator {
    public:
        const_iterator(T const* p, int e)
                :pos(p), edge(e) { }

        friend bool operator!=(const_iterator lhs, const_iterator rhs)
        {
            Expects(lhs.edge==rhs.edge);
            return lhs.pos!=rhs.pos;
        }

        friend const_iterator& operator++(const_iterator& rhs)
        {
            std::advance(rhs.pos, rhs.edge);
            return rhs;
        }

    private:
        T const* pos;
        int edge;
    };

    auto begin()
    {
        return iterator(begin(cells), edge);
    }

    auto begin() const
    {
        return const_iterator(begin(cells), edge);
    }

    auto end()
    {
        return iterator(end(cells), edge);
    }

    auto end() const
    {
        return const_iterator(end(cells), edge);
    }

    gsl::span<T const> operator[](int y) const
    {
        Expects(y>=0);
        Expects(y<edge);
        return {cells.get()+y*edge, edge};
    }

    gsl::span<T> operator[](int y)
    {
        Expects(y>=0);
        Expects(y<edge);
        return {cells.get()+y*edge, edge};
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

            result[row_index][column_index] = *cell_found;
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
