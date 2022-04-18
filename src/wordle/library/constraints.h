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

#ifndef WSS_WORDLE_CONSTRAINTS_H
#define WSS_WORDLE_CONSTRAINTS_H

#include <wordle/word.h>

#include <command_line.h>
#include <letter_set.h>
#include <letter_values.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <variant>
#include <vector>

namespace wordle {
    struct constraints {
        // how few of each letter are allowed
        letter_values minimum{};

        // how many of each letter are allowed
        letter_values maximum{};

        // which letters are allowed in each position
        std::array<letter_set, wordle::word_size> allowed;

        friend auto operator==(constraints const& l, constraints const& r) noexcept -> bool = default;
    };
}  // namespace wordle

template<>
struct fmt::formatter<wordle::constraints> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(wordle::constraints const c, FormatContext& ctx)
    {
        format_to(ctx.out(), "{{.limits={{");

        bool need_comma = false;
        for (auto l{'A'}; l <= 'Z'; ++l) {
            auto const limits{std::array{c.minimum[l], c.maximum[l]}};
            auto const limited{std::array{limits[0] > 0, limits[1] < wordle::word_size}};
            if (!limited[0] && !limited[1]) {
                continue;
            }

            if (need_comma) {
                format_to(ctx.out(), ",");
            }
            need_comma = true;

            if (limited[0]) {
                format_to(ctx.out(), "{}<=", limits[0]);
            }
            format_to(ctx.out(), "{}", l);
            if (limited[1]) {
                format_to(ctx.out(), "<={}", limits[1]);
            }
        }
        format_to(ctx.out(), "}},");

        format_to(ctx.out(), " .disallowed=(");
        need_comma = false;
        for (auto pos{0}; pos != wordle::word_size; pos++) {
            if (need_comma) {
                format_to(ctx.out(), ",");
            }
            need_comma = true;

            auto const disallowed{c.allowed[pos] ^ letter_set::all};

            format_to(ctx.out(), "\"");
            for (auto l : disallowed) {
                format_to(ctx.out(), "{}", l);
            }
            format_to(ctx.out(), "\"");
        }
        format_to(ctx.out(), ")}}");

        return ctx.out();
    }
};

#endif  // WSS_WORDLE_CONSTRAINTS_H
