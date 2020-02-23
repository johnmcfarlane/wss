// Copyright 2020 John McFarlane
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

#include "solve.h"

#include "board.h"
#include "board_premiums.h"
#include "coord.h"
#include "search.h"
#include "tile.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <iterator>
#include <string_view>
#include <utility>
#include <vector>

namespace {
    auto populate_qualifying_cells(board<bool>& qualifying_cells,
            board<char> const& board_tiles, coord const& offset,
            coord const& first, coord const& last)
    {
        coord pos;
        for (pos[1] = first[1]; pos[1]!=last[1]; ++pos[1]) {
            for (pos[0] = first[0]; pos[0]!=last[0]; ++pos[0]) {
                if (board_tiles.cell(pos+offset)!=vacant) {
                    qualifying_cells.cell(pos) = true;
                }
            }
        }
    }

    auto make_qualifying_cells(board<char> const& board_tiles) -> board<bool>
    {
        auto const edge{ssize(board_tiles)};
        board<bool> qualifying_cells(edge);

        if (edge>0) {  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
            populate_qualifying_cells(qualifying_cells, board_tiles, {-1, 0}, {1, 0}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {1, 0}, {0, 0}, {edge-1, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, -1}, {0, 1}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, 1}, {0, 0}, {edge, edge-1});
        }

        qualifying_cells.cell({edge/2, edge/2}) = true;

        return qualifying_cells;
    }

    void refine_results(std::vector<result>& finds)
    {
        sort(begin(finds), end(finds), [](auto const& a, auto const& b) {
            return tie(b.score, a.word, a.pos.start[1], a.pos.start[0],
                    a.pos.direction[1], a.pos.direction[0])
                    <tie(a.score, b.word, b.pos.start[1], b.pos.start[0],
                            b.pos.direction[1], b.pos.direction[0]);
        });

        auto const last_unique = unique(begin(finds), end(finds),
                [](auto const& a, auto const& b) {
                    return tie(a.word, a.score, a.pos.start, a.pos.direction)
                            ==tie(b.word, b.score, b.pos.start, b.pos.direction);
                });
        finds.erase(last_unique, end(finds));
    }

    auto make_start_state(node const& lexicon, letter_values const& letter_scores,
            board<char> tiles, board<premium> premiums,
            std::vector<char>& word, std::string_view letters)
    {
        auto qualifying_cells{make_qualifying_cells(tiles)};
        return initial_state {
                {},
                {},
                letter_scores,
                std::move(tiles),
                std::move(premiums),
                std::move(qualifying_cells),
                std::begin(word),
                lexicon,
                ssize(letters)
        };
    }

    auto solve_axial(initial_state& init, step_state& step, int bearing)
    {
        init.pos.direction[0] = init.cross_direction[1] = 1-bearing;
        init.pos.direction[1] = init.cross_direction[0] = bearing;

        search_state const state{
                init,
                step
        };

        auto const edge = ssize(init.tiles);
        for (init.pos.start[init.pos.direction[0]] = 0;
             init.pos.start[init.pos.direction[0]]!=edge;
             ++init.pos.start[init.pos.direction[0]]) {
            auto count_back{0};
            for (init.pos.start[init.pos.direction[1]] = edge-1;
                 init.pos.start[init.pos.direction[1]]>=0;
                 --init.pos.start[init.pos.direction[1]]) {
                step.pos = init.pos.start;
                if (state.init.qualifying_cells.cell(init.pos.start)) {
                    count_back = init.rack_size;
                }
                else {
                    --count_back;
                }
                if (count_back>0) {
                    search(state);
                }
            }
        }
    }
}  // namespace

auto solve(node const& lexicon, letter_values const& letter_scores,
        std::string_view letters, board<char> tiles,
        board<premium> premiums) -> std::vector<result>
{
    auto const edge{ssize(tiles)};
    std::vector<char> word(edge+1);
    std::vector<result> finds;
    letter_values rack{};

    auto init{make_start_state(
            lexicon, letter_scores, std::move(tiles),
            std::move(premiums), word, letters)};
    step_state step{
            std::begin(word),
            finds,
            rack,
            coord{},
            init.rack_size
    };

    for (auto letter : letters) {
        ++step.rack[letter];
    }

    solve_axial(init, step, 0);
    solve_axial(init, step, 1);

    refine_results(step.finds);
    return move(step.finds);
}
