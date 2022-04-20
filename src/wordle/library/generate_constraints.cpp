// Copyright 2022 John McFarlane
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

#include "generate_constraints.h"

#include "constraints.h"  // IWYU pragma: keep
#include <wordle/word.h>

#include "letter_set.h"
#include "letter_values.h"
#include "wss_assert.h"

#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <iterator>
#include <optional>
#include <vector>

namespace {
    auto has_multiple_bits_set(std::uint32_t v)
    {
        return (v & (v - 1)) != 0;
    }
}  // namespace

auto wordle::generate_constraints(wordle::attempts const& history) -> std::optional<constraints>
{
    constraints result;

    std::fill(begin(result.minimum), end(result.minimum), 0);
    std::fill(begin(result.maximum), end(result.maximum), wordle::word_size);
    std::fill(begin(result.allowed), end(result.allowed), letter_set::all);

    for (auto const play : history) {
        std::array<letter_values, static_cast<int>(wordle::letter_score_bound)> present{};
        for (auto position{0}; position != wordle::word_size; position++) {
            auto const position_letter{static_cast<char>(std::toupper(play.guess[position]))};
            WSS_ASSERT(std::isalpha(position_letter));

            auto const position_score{play.feedback[position]};

            present[static_cast<int>(position_score)][position_letter]++;

            auto& allowed_letters{result.allowed[position]};

            // Iff we don't already know what letter is in this position...
            if (has_multiple_bits_set(allowed_letters.bits())) {
                switch (position_score) {
                case wordle::letter_score::hit:
                    if (!allowed_letters[position_letter]) {
                        fmt::print("letter {} of play, \"{}\", already cannot be {}!\n", position + 1, play.guess, position_letter);
                        return std::nullopt;
                    }

                    // Only the guessed letter is allowed in this space.
                    allowed_letters = {position_letter};
                    break;

                case wordle::letter_score::near:
                    // The guessed letter is not allowed in this space.
                    allowed_letters.reset(position_letter);
                    WSS_ASSERT(allowed_letters != letter_set::none);
                    break;

                default:
                    WSS_ASSERT(position_score == wordle::letter_score::miss);
                    allowed_letters.reset(position_letter);
                    break;
                }
            }
            // Iff we already know what letter is in this position...
            else {
                switch (position_score) {
                case wordle::letter_score::hit:
                    if (!allowed_letters[position_letter]) {
                        fmt::print("letter {} of play, \"{}\", is not allowed to be {}!\n", position + 1, play.guess, position_letter);
                        return std::nullopt;
                    }
                    break;

                case wordle::letter_score::near:
                    if (allowed_letters[position_letter]) {
                        fmt::print("letter {} of play, \"{}\", is not allowed to not be {}!\n", position + 1, play.guess, position_letter);
                        return std::nullopt;
                    }
                    break;

                default:
                    WSS_ASSERT(position_score == wordle::letter_score::miss);

                    if (allowed_letters[position_letter]) {
                        fmt::print("letter {} of play, \"{}\", cannot not be {}!\n", position + 1, play.guess, position_letter);
                        return std::nullopt;
                    }
                    break;
                }
            }

            WSS_ASSERT(allowed_letters != letter_set::none);
        }

        for (auto letter = int{'A'}; letter != int{'Z' + 1}; letter++) {
            auto const misses{present[static_cast<int>(wordle::letter_score::miss)][letter]};
            auto const nears{present[static_cast<int>(wordle::letter_score::near)][letter]};
            auto const hits{present[static_cast<int>(wordle::letter_score::hit)][letter]};

            if (misses != 0) {
                result.maximum[letter] = std::min(result.maximum[letter], nears + hits);
            } else {
                result.minimum[letter] = std::max(result.minimum[letter], nears + hits);
            }
        }
    }
    return result;
}