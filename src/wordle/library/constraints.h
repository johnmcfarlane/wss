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
#include <wss_assert.h>

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

    [[nodiscard]] constexpr auto open_constraints()
    {
        constraints c;

        std::fill(begin(c.minimum), end(c.minimum), 0);

        auto const first{std::begin(c.maximum)};
        auto const alpha_first{std::next(first, 'A')};
        auto const alpha_last{std::next(first, 'Z' + 1)};
        auto const last{std::end(c.maximum)};
        std::fill(first, alpha_first, 0);
        std::fill(alpha_first, alpha_last, wordle::word_size);
        std::fill(alpha_last, last, 0);
        std::fill(begin(c.allowed), end(c.allowed), letter_set::all);

        return c;
    }

    [[nodiscard]] constexpr auto validate(constraints const& c)
    {
        for (auto i{0}; i != static_cast<int>(letter_values{}.size()); ++i) {
            auto const minimum{c.minimum[i]};
            auto const maximum{c.maximum[i]};
            (void)minimum;
            (void)maximum;
            if (i >= 'A' && i <= 'Z') {
                WSS_ASSERT(minimum >= 0 && minimum <= word_size);
                WSS_ASSERT(maximum >= 0 && maximum <= word_size);
                WSS_ASSERT(minimum <= maximum);
            } else {
                WSS_ASSERT(minimum == 0);
                WSS_ASSERT(maximum == 0);
            }
        }
        for (auto i{0}; i != word_size; ++i) {
            WSS_ASSERT((c.allowed[i] & letter_set::all) == c.allowed[i]);
        }
    }
}  // namespace wordle

template<>
struct fmt::formatter<wordle::constraints> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(wordle::constraints const& c, FormatContext& ctx)
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
