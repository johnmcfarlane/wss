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
#include "result.h"
#include "search.h"

#include <letter_set.h>
#include <letter_values.h>
#include <node.h>
#include <ssize.h>
#include <tile.h>
#include <wss_assert.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <iterator>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

namespace {
    auto populate_qualifying_cells(board<bool>& qualifying_cells, board<char> const& board_tiles, coord const& offset, coord const& first, coord const& last)
    {
        coord pos;
        for (pos[1] = first[1]; pos[1] != last[1]; ++pos[1]) {
            for (pos[0] = first[0]; pos[0] != last[0]; ++pos[0]) {
                if (board_tiles.cell(pos + offset) != vacant) {
                    qualifying_cells.cell(pos) = true;
                }
            }
        }
    }

    auto make_qualifying_cells(board<char> const& board_tiles) -> board<bool>
    {
        auto const edge{ssize(board_tiles)};
        board<bool> qualifying_cells(edge);

        if (edge > 0) {  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
            populate_qualifying_cells(qualifying_cells, board_tiles, {-1, 0}, {1, 0}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {1, 0}, {0, 0}, {edge - 1, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, -1}, {0, 1}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, 1}, {0, 0}, {edge, edge - 1});
        }

        qualifying_cells.cell({edge / 2, edge / 2}) = true;

        return qualifying_cells;
    }

    auto calculate_cross(
            coord pos,
            letter_values const& letter_scores,
            board<char> const& tiles,
            board<premium> const& premiums,
            node lexicon,
            letter_values const& rack,
            std::vector<std::string>& invalid_words)
    {
        crosswords result;

        if (tiles.cell(pos) != vacant) {
            // The cell is already occupied so filtering isn't an issue
            // and there is no score from crosswords.
            result.filter = letter_set::all;
            return result;
        }

        auto const column = pos[0];
        auto const extents = word_extent(tiles, pos, 1, coord{0, 1});

        if (extents.second - extents.first == 1) {
            // The cell is already occupied so filtering isn't an issue
            // and there is no score from crosswords.
            result.filter = letter_set::all;
            return result;
        }

        auto const cell_premium_index = static_cast<int>(premiums.cell(pos));
        WSS_ASSERT(cell_premium_index >= 0 && cell_premium_index < ssize(word_multipliers));

        auto const word_multiplier = word_multipliers[cell_premium_index];  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        auto const letter_multiplier = letter_multipliers[cell_premium_index];  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        auto const have_wildcard = rack[blank] != 0 || rack[wildcard] != 0;

        // returns end-point of word in lexicon and tile score
        auto travel_word_portion = [pos, &letter_scores, &tiles = tiles, column](
                                           node const* n,
                                           std::array<int, 2> row_delta_range)
                -> std::optional<std::tuple<node const*, int>> {
            auto const row_first = pos[1] + row_delta_range[0];
            auto const row_last = pos[1] + row_delta_range[1];

            auto score = 0;
            for (auto row = row_first; row != row_last; ++row) {
                auto const letter = tiles.cell(coord{column, row});
                auto const found{std::find(
                        begin(*n),
                        end(*n),
                        std::toupper(letter))};
                if (found == end(*n)) {
                    // The letters above the cell don't form the beginning of a word.
                    return std::nullopt;
                }
                score += letter_scores[letter];
                n = &found.child();
            }
            return std::tuple{n, score};
        };

        auto travel_word = [&](std::array<int, 2> row_delta_range) {
            auto upper_portion = travel_word_portion(&lexicon, row_delta_range);
            if (!upper_portion) {
                std::string invalid_word;

                for (auto row = pos[1] + row_delta_range[0],
                          row_last = pos[1] + row_delta_range[1];
                     row != row_last;
                     ++row) {
                    invalid_word.push_back(
                            std::tolower(tiles.cell(coord{column, row})));
                }
                invalid_words.push_back(std::move(invalid_word));
                return std::tuple{&std::as_const(lexicon), 0};
            }
            return *upper_portion;
        };

        // Travel through the lexicon for letters above the cell.
        auto [upper_node, upper_score] = travel_word({extents.first, 0});

        // Travel through the lexicon for letters below the cell.
        auto [lower_node, lower_score] = travel_word({1, extents.second});

        // Travel through lexicon for letters on the cell.
        for (auto edge = begin(*upper_node), edge_end = end(*upper_node);
             edge != edge_end;
             ++edge) {
            auto const letter = edge.letter();

            if (auto const have_letter = rack[letter] != 0;
                !(have_letter || have_wildcard)) {
                continue;
            }

            auto lower_result = travel_word_portion(&edge.child(), {1, extents.second});
            if (!lower_result || !std::get<0>(*lower_result)->letters['\0']) {
                // With this letter, no word can be joined from the upper and lower portions.
                continue;
            }

            result.filter.set(letter);
            result.letter_scores[letter] =
                    (letter_scores[letter] * letter_multiplier
                     + upper_score + lower_score)
                    * word_multiplier;
            result.blank_scores[letter] =
                    result.blank_scores[std::tolower(letter)] =
                            (upper_score + lower_score) * word_multiplier;
        }

        return result;
    }

    auto solve_axial(initial_state& init, step_state& step, std::vector<std::string>& invalid_words)
    {
        auto const edge = init.tiles.size();

        for (auto row = 0; row != edge; ++row) {
            for (auto column = 0; column != edge; ++column) {
                auto const pos = coord{row, column};
                init.crossword_cells.cell(pos) = calculate_cross(
                        pos, init.letter_scores, init.tiles, init.premiums,
                        init.lexicon, step.rack, invalid_words);
            }
        }

        search_state const state{
                init,
                step};

        for (init.pos[1] = 0;
             init.pos[1] != edge;
             ++init.pos[1]) {
            auto count_back{0};
            for (init.pos[0] = edge - 1;
                 init.pos[0] >= 0;
                 --init.pos[0]) {
                step.pos = init.pos;
                if (state.init.qualifying_cells.cell(init.pos)) {
                    count_back = init.rack_size;
                } else {
                    --count_back;
                }
                if (count_back > 0) {
                    search(state);
                }
            }
        }
    }
}  // namespace

auto solve(node const& lexicon, letter_values const& letter_scores, std::string_view letters, board<char> tiles, board<premium> premiums)
        -> std::tuple<std::vector<result>, std::vector<std::string>>
{
    auto const edge{ssize(tiles)};
    std::vector<char> word(edge + 1);
    std::vector<result> finds;
    letter_values rack{};

    auto qualifying_cells{make_qualifying_cells(tiles)};
    initial_state init{
            {},
            letter_scores,
            std::move(tiles),
            std::move(premiums),
            std::move(qualifying_cells),
            std::begin(word),
            lexicon,
            ssize(letters),
            board<crosswords>{edge}};
    step_state step{
            std::begin(word),
            finds,
            rack,
            coord{},
            init.rack_size};

    for (auto letter : letters) {
        ++step.rack[letter];
    }

    auto results = std::vector<result>{};
    auto invalid_words = std::vector<std::string>{};

    // horizontal scan
    solve_axial(init, step, invalid_words);
    std::swap(step.finds, results);

    // vertical scan
    transpose(init.tiles);
    transpose(init.premiums);
    transpose(init.qualifying_cells);
    solve_axial(init, step, invalid_words);
    for (auto& result : step.finds) {
        transpose(result.pos.start);
        transpose(result.pos.direction);
    }
    std::move(begin(step.finds), end(step.finds), std::back_inserter(results));

    // sort invalid words, remove duplicates
    sort(begin(invalid_words), end(invalid_words));
    invalid_words.erase(unique(begin(invalid_words), end(invalid_words)), end(invalid_words));

    // sort results
    sort(begin(results), end(results));

    return {results, invalid_words};
}
