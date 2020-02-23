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
#include "tile.h"
#include "wss_assert.h"

#include <wwf_lexicon.h>

#include <gsl/gsl_util>

#include <algorithm>
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
    constexpr auto full_rack_size{7};

    template<typename T>
    auto get(board<T> const& b, coord pos, T oob) -> T
    {
        if (pos[0]<0) {
            return oob;
        }
        if (pos[1]<0) {
            return oob;
        }
        auto const edge{b.size()};
        if (pos[0]>=edge) {
            return oob;
        }
        if (pos[1]>=edge) {
            return oob;
        }
        return b.cell(pos);
    }

    auto word_extent(
            board<char> const& board_tiles,
            coord const& part_start,
            int const part_length,
            coord const& cross_direction)
    {
        WSS_ASSERT((std::abs(cross_direction[0])==1)!=(std::abs(cross_direction[1])==1));

        auto pre_word_part{-1};
        while (get(
                board_tiles,
                part_start+cross_direction*pre_word_part,
                vacant)!=vacant) {
            --pre_word_part;
        }
        ++pre_word_part;

        auto post_word_part{part_length};
        while (get(
                board_tiles,
                part_start+cross_direction*post_word_part,
                vacant)!=vacant) {
            ++post_word_part;
        }

        return std::make_pair(pre_word_part, post_word_part);
    }

    auto calc_score(
            initial_state const& init,
            coord const& part_start,
            coord const& direction,
            std::pair<int, int> extents,
            gsl::span<char> word_part)
    -> std::optional<int>
    {
        auto const word_size{extents.second-extents.first};
        if (word_size==1) {
            return 0;
        }

        int word_multiplier{1};
        int score{0};
        auto const* node{&init.lexicon};
        for (auto i{extents.first}; i!=extents.second; ++i) {
            auto const pos{part_start+direction*(i)};
            auto const tile{init.tiles.cell(pos)};
            auto const cell_premium{init.premiums.cell(pos)};

            auto[letter, letter_multiplier] = [&]() {
                if (tile!=vacant) {
                    return std::make_tuple(tile, 1);
                }

                // mutation!!
                word_multiplier *= gsl::at(word_multipliers,
                        int(cell_premium));

                WSS_ASSERT(i>=0);
                WSS_ASSERT(i<ssize(word_part));
                auto const letter{gsl::at(word_part, i)};

                auto const letter_multiplier{
                        gsl::at(letter_multipliers, int(cell_premium))};

                return std::make_tuple(letter, letter_multiplier);
            }();

            auto const found{
                    std::find(begin(*node), end(*node), std::toupper(letter))};
            if (found==end(*node)) {
                return std::nullopt;
            }
            node = &found.child();

            auto const letter_score{init.letter_scores[letter]};
            score += letter_multiplier*letter_score;
        }
        if (!node->is_terminator) {
            return std::nullopt;
        }
        return word_multiplier*score;
    }

    void search(node const& n, search_state state);

    void recurse(node const& n, char letter, int qualifying_cells_count,
            search_state state)
    {
        *state.step.word_end++ = letter;
        state.step.num_qualifying_cells += qualifying_cells_count;
        state.step.pos += state.init.pos.direction;
        if (n.is_terminator
                && state.step.num_qualifying_cells>0
                && state.step.rack_remaining<state.init.rack_size
                && get(state.init.tiles, state.step.pos, vacant)
                        ==vacant) {
            auto const extents{word_extent(
                    state.init.tiles,
                    state.init.pos.start,
                    std::distance(
                            state.init.word, state.step.word_end),
                    state.init.pos.direction)};
            auto const word_score{calc_score(
                    state.init,
                    state.init.pos.start,
                    state.init.pos.direction,
                    extents,
                    gsl::span<char>{&*state.init.word, &*state.step.word_end})};
            WSS_ASSERT(word_score);

            auto play_score{state.step.cross_scores+*word_score};
            if (state.step.rack_remaining==0
                    && state.init.rack_size==full_rack_size) {
                play_score += state.init.letter_scores[full_rack_score_index];
            }

            state.step.finds.emplace_back(result{
                    std::string{state.init.word, state.step.word_end},
                    play_score,
                    state.init.pos});
        }

        if (state.step.pos[0] < state.init.tiles.size()
            && state.step.pos[1] < state.init.tiles.size()) {
            search(n, state);
        }
        
        // cppcheck-suppress unreadVariable
        state.step.pos -= state.init.pos.direction;
        // cppcheck-suppress unreadVariable
        state.step.num_qualifying_cells -= qualifying_cells_count;
        --state.step.word_end;
    }

    auto fill_square(search_state state, node const& edge,
            std::pair<int, int> extents, int qualifying_cells_count,
            int& counter,
            char letter)
    {
        if (counter==0) {
            return;
        }

        auto const cross_score{calc_score(
                state.init,
                state.step.pos,
                state.init.cross_direction,
                extents,
                gsl::span<char>(&letter, 1))};

        // Cross-word is not a valid word in the lexicon.
        if (!cross_score) {
            return;
        }

        --counter;
        state.step.cross_scores += *cross_score;
        recurse(edge, letter, qualifying_cells_count, state);
        // cppcheck-suppress unreadVariable
        state.step.cross_scores -= *cross_score;
        ++counter;
    }

    void search(node const& n, search_state state)
    {
        WSS_ASSERT(state.step.rack_remaining>=0);

        auto const qualifying_cells_count{
                state.init.qualifying_cells.cell(state.step.pos) ? 1 : 0};

        auto const board_tile{state.init.tiles.cell(state.step.pos)};
        if (board_tile==vacant) {
            auto i{begin(n)};
            auto const n_end{end(n)};
            if (i==n_end) {
                return;
            }

            --state.step.rack_remaining;
            auto& blank_count{state.step.rack[blank]};
            auto& wildcard_count{state.step.rack[wildcard]};

            auto const extents{word_extent(
                    state.init.tiles,
                    state.step.pos,
                    1,
                    state.init.cross_direction)};

            do {
                auto const letter{i.letter()};
                auto& letter_count{state.step.rack[letter]};
                auto const& edge(i.child());
                ++i;

                if (blank_count==0 && wildcard_count==0 && letter_count==0) {
                    continue;
                }

                fill_square(state, edge, extents, qualifying_cells_count,
                        letter_count,
                        letter);
                fill_square(state, edge, extents, qualifying_cells_count,
                        blank_count,
                        char(std::tolower(letter)));
                fill_square(state, edge, extents, qualifying_cells_count,
                        wildcard_count, letter);
            }
            while (i!=n_end);

            ++state.step.rack_remaining;
            return;
        }

        auto const found{std::find_if(
                begin(n),
                end(n),
                [board_tile](auto const& c) {
                    return c==std::toupper(board_tile);
                })
        };
        if (found==end(n)) {
            return;
        }

        recurse(found.child(), board_tile, qualifying_cells_count, state);
    }
} // namespace

void search(search_state state)
{
    if (state.init.qualifying_cells.cell(state.init.pos.start)) {
        auto const preceding{get(
                state.init.tiles,
                state.init.pos.start-state.init.pos.direction,
                vacant)};
        if (preceding!=vacant) {
            // The start of a word cannot go on the board here
            // because there's a tile in the preceding space.
            return;
        }
    }

    search(state.init.lexicon, state);
}
