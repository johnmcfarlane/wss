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

#include "search.h"

#include "board.h"
#include "coord.h"
#include "letter_set.h"
#include "tile.h"
#include "wss_assert.h"

#include <node.h>

#include <gsl/gsl_util>

#include <cctype>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace {
    struct score_accumulator {
        int word_score{};
        int word_multiplier{1};
        int crossword_scores{};
    };

    constexpr auto full_rack_size{7};

    template<typename T>
    auto get(board<T> const& b, coord pos, T oob) -> T
    {
        if (pos[0] < 0) {
            return oob;
        }
        if (pos[1] < 0) {
            return oob;
        }
        auto const edge{b.size()};
        if (pos[0] >= edge) {
            return oob;
        }
        if (pos[1] >= edge) {
            return oob;
        }
        return b.cell(pos);
    }

    void search(node const& n, search_state state, score_accumulator score);

    void recurse(node const& n, char letter, int qualifying_cells_count, search_state state, score_accumulator score)
    {
        *state.step.word_end++ = letter;
        state.step.num_qualifying_cells += qualifying_cells_count;
        ++state.step.pos[0];
        if (n.letters['\0']
            && state.step.num_qualifying_cells > 0
            && state.step.rack_remaining < state.init.rack_size
            && get(state.init.tiles, state.step.pos, vacant)
                       == vacant) {
            auto const full_rack_bonus = (state.step.rack_remaining == 0
                                          && state.init.rack_size == full_rack_size)
                                               ? state.init.letter_scores[full_rack_score_index]
                                               : 0;
            auto const play_score = score.word_score * score.word_multiplier
                                  + score.crossword_scores
                                  + full_rack_bonus;

            state.step.finds.emplace_back(result{
                    std::string{state.init.word, state.step.word_end},
                    play_score,
                    {state.init.pos, coord{1, 0}}});
        }

        if (state.step.pos[0] < state.init.tiles.size()) {
            search(n, state, score);
        }

        // cppcheck-suppress unreadVariable
        state.step.pos -= coord{1, 0};
        // cppcheck-suppress unreadVariable
        state.step.num_qualifying_cells -= qualifying_cells_count;
        --state.step.word_end;
    }

    auto fill_square(search_state state, node const& edge, int qualifying_cells_count, int& counter, letter_set const crossword_filter, letter_values const& crossword_scores, char letter, score_accumulator score)
    {
        if (counter == 0) {
            return;
        }

        if (!crossword_filter[letter]) {
            return;
        }

        --counter;

        auto const cell_premium = int(state.init.premiums.cell(state.step.pos));
        score.word_score += state.init.letter_scores[letter]
                          * letter_multipliers[cell_premium];  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        score.word_multiplier *= word_multipliers[cell_premium];  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        score.crossword_scores += crossword_scores[letter];

        recurse(edge, letter, qualifying_cells_count, state, score);

        ++counter;
    }

    void search(node const& n, search_state state, score_accumulator score)
    {
        WSS_ASSERT(state.step.rack_remaining >= 0);

        auto const qualifying_cells_count{
                state.init.qualifying_cells.cell(state.step.pos) ? 1 : 0};

        auto const board_tile{state.init.tiles.cell(state.step.pos)};
        if (board_tile == vacant) {
            auto i{begin(n)};
            auto const n_end{end(n)};
            if (i == n_end) {
                return;
            }

            --state.step.rack_remaining;
            auto& blank_count{state.step.rack[blank]};
            auto& wildcard_count{state.step.rack[wildcard]};

            auto const& crossword_cell = state.init.crossword_cells.cell(state.step.pos);

            do {
                auto const letter{i.letter()};
                auto& letter_count{state.step.rack[letter]};
                auto const& edge(i.child());
                ++i;

                if (blank_count == 0 && wildcard_count == 0 && letter_count == 0) {
                    continue;
                }

                fill_square(state, edge, qualifying_cells_count, letter_count, crossword_cell.filter, crossword_cell.letter_scores, letter, score);
                fill_square(state, edge, qualifying_cells_count, blank_count, crossword_cell.filter, crossword_cell.blank_scores, char(std::tolower(letter)), score);
                fill_square(state, edge, qualifying_cells_count, wildcard_count, crossword_cell.filter, crossword_cell.letter_scores, letter, score);
            } while (i != n_end);

            ++state.step.rack_remaining;
            return;
        }

        auto const needle{std::toupper(board_tile)};
        auto found{n.letters.find(needle)};
        if (found == end(n.letters)) {
            return;
        }

        score.word_score += state.init.letter_scores[board_tile];

        auto const letter_index{found - begin(n.letters)};
        auto const& child_edge{n.edges[letter_index]};  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        recurse(child_edge, board_tile, qualifying_cells_count, state, score);
    }
}  // namespace

auto word_extent(
        board<char> const& board_tiles,
        coord const& part_start,
        int const part_length,
        coord const& cross_direction)
        -> std::pair<int, int>
{
    WSS_ASSERT((std::abs(cross_direction[0]) == 1) != (std::abs(cross_direction[1]) == 1));

    auto pre_word_part{-1};
    while (get(
                   board_tiles,
                   part_start + cross_direction * pre_word_part,
                   vacant)
           != vacant) {
        --pre_word_part;
    }
    ++pre_word_part;

    auto post_word_part{part_length};
    while (get(
                   board_tiles,
                   part_start + cross_direction * post_word_part,
                   vacant)
           != vacant) {
        ++post_word_part;
    }

    return std::pair{pre_word_part, post_word_part};
}

void search(search_state state)
{
    if (state.init.qualifying_cells.cell(state.init.pos)) {
        auto const preceding{new char{get(
                state.init.tiles,
                state.init.pos - coord{1, 0},
                vacant)}};
        if (*preceding != vacant) {
            // The start of a word cannot go on the board here
            // because there's a tile in the preceding space.
            return;
        }
        delete preceding;
    }

    search(state.init.lexicon, state, score_accumulator{});
}
