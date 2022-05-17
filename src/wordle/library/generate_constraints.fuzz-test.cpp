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

#include "input.h"
#include <wordle/word.h>

#include <fuzz-test.h>

#include <fmt/format.h>
#include <fmt/printf.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <vector>

template<>
struct fmt::formatter<wordle::attempts> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(wordle::attempts const& attempts, FormatContext& ctx)
    {
        fmt::format_to(ctx.out(), "{{");

        bool need_comma = false;
        for (auto const& attempt : attempts) {
            if (need_comma) {
                fmt::format_to(ctx.out(), ",");
            }
            need_comma = true;

            fmt::format_to(ctx.out(), "\"");
            for (auto i{0}; i != wordle::word_size; ++i) {
                fmt::format_to(ctx.out(), "{}", attempt.guess[i]);
            }
            for (auto i{0}; i != wordle::word_size; ++i) {
                fmt::format_to(ctx.out(), "{}", static_cast<int>(attempt.feedback[i]));
            }
            fmt::format_to(ctx.out(), "\"");
        }

        fmt::format_to(ctx.out(), "}}");
        return ctx.out();
    }
};

auto read_attempts(fuzzer_input in, int max_attempts)
        -> std::optional<wordle::attempts>
{
    auto const num_attempts{in.read_integer(0, max_attempts)};
    if (!num_attempts) {
        return std::nullopt;
    }

    wordle::attempts attempts{};
    for (auto i{0}; i != *num_attempts; ++i) {
        auto attempt{wordle::attempt{}};

        for (auto j{0}; j != wordle::word_size; ++j) {
            auto const guess{in.read_integer<char>('A', 'Z')};
            if (!guess) {
                return std::nullopt;
            }

            auto const feedback{in.read_integer(0, wordle::letter_score_bound - 1)};
            if (!feedback) {
                return std::nullopt;
            }

            attempt.guess[j] = *guess;
            attempt.feedback[j] = static_cast<wordle::letter_score>(*feedback);
        }

        attempts.push_back(attempt);
    }

    return attempts;
}

extern "C" auto LLVMFuzzerTestOneInput(std::uint8_t const* data, std::size_t size)
        -> int
{
    auto const history{read_attempts(fuzzer_input{data, size}, 10)};
    if (!history) {
        return 0;
    }

    fmt::print(stderr, "calling wordle::generate_constraints({})\n", *history);
    wordle::generate_constraints(*history);

    return 0;  // Non-zero return values are reserved for future use.
}
