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

#include "run.h"

#include "input.h"

#include <command_line.h>
#include <letter_set.h>
#include <letter_values.h>
#include <node.h>
#include <wordle_lexicon.h>
#include <wss_assert.h>

#include <fmt/format.h>
#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

template<>
struct fmt::formatter<wordle::word> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(wordle::word const w, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", std::string_view{w.data(), w.size()});
    }
};

namespace {
    struct constraints {
        // how few of each letter are allowed
        letter_values minimum{};

        // how many of each letter are allowed
        letter_values maximum{};

        // which letters are allowed in each position
        std::array<letter_set, wordle::word_size> allowed;
    };

    using suggestions = std::vector<wordle::word>;

    // https://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    auto has_multiple_bits_set(std::uint32_t v)
    {
        return (v & (v - 1)) != 0;
    }

    auto generate_constraints(wordle::attempts const& history) -> std::optional<constraints>
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

    auto solve(node const& n, constraints& c, wordle::word& suggestion, int position, suggestions& solution)
    {
        if (position == wordle::word_size) {
            for (auto l{'A'}; l != 'Z' + 1; ++l) {
                if (c.minimum[l] > 0) {
                    return;
                }
            }

            solution.emplace_back(suggestion);
            return;
        }

        for (auto edge_it{begin(n)}; edge_it != end(n); ++edge_it) {
            auto const edge_letter{*edge_it};

            if (!c.allowed[position][edge_letter]) {
                continue;
            }

            if (c.maximum[edge_letter] <= 0) {
                continue;
            }

            c.minimum[edge_letter]--;
            c.maximum[edge_letter]--;
            suggestion[position] = edge_letter;
            solve(edge_it.child(), c, suggestion, position + 1, solution);
            c.minimum[edge_letter]++;
            c.maximum[edge_letter]++;
        }
    }

    auto solve(constraints c) -> suggestions
    {
        suggestions solution;
        wordle::word suggestion;
        ::solve(wordle_lexicon, c, suggestion, 0, solution);
        return solution;
    }

    void dump(constraints const& c)
    {
        auto const dump_limit{[](auto const& limit, auto const* name, auto ignorable) {
            auto const first{std::next(std::cbegin(limit), int{'A'})};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)
            auto const last{std::next(std::cbegin(limit), int{'Z'} + 1)};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)
            if (std::all_of(first, last, [&](auto const letter_limit) {
                    return letter_limit == ignorable;
                })) {
                return;
            }

            fmt::print("{}:\n", name);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,llvm-qualified-auto,readability-qualified-auto)
            for (auto it{first}; it != last; it++) {
                auto const letter_limit{*it};
                if (letter_limit != ignorable) {
                    auto const index{std::distance(std::cbegin(limit), it)};
                    fmt::print("'{}': {}\n", static_cast<char>(index), letter_limit);
                }
            }
        }};

        dump_limit(c.minimum, "minimum", 0);
        dump_limit(c.maximum, "maximum", wordle::word_size);

        fmt::print("\ndisallowed:\n");
        for (auto pos{0}; pos != wordle::word_size; pos++) {
            auto const disallowed{c.allowed[pos] ^ letter_set::all};
            fmt::print("{}: ", pos);

            if (disallowed == letter_set::none) {
                fmt::print("<none>\n");
                continue;
            }

            for (auto l : disallowed) {
                fmt::print("{}", l);
            }
            fmt::print("\n");
        }
    }

    auto print(suggestions const& s)
    {
        std::for_each(
                std::cbegin(s), std::cend(s),
                [](auto const& match) {
                    fmt::print("{}\n", match);
                });
    }
}  // namespace

auto wordle::run(command_line args) -> int
{
    auto const input{parse_command_line(args)};

    if (auto const* exit_status = std::get_if<int>(&input)) {
        return *exit_status;
    }

    auto const& query{std::get<wordle::query>(input)};
    auto const constraints{generate_constraints(query.history)};
    if (!constraints) {
        return EXIT_FAILURE;
    }

    if (query.debug) {
        dump(*constraints);
        return EXIT_SUCCESS;
    }

    auto const solution{solve(*constraints)};

    print(solution);

    return EXIT_SUCCESS;
}
