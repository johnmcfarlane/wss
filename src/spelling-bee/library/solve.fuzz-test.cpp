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

#include "solve.h"

#include "input.h"

#include <fuzz-test.h>
#include <letter_set.h>

#include <fmt/format.h>
#include <fmt/printf.h>

#include <cstddef>
#include <cstdint>
#include <optional>

template<>
struct fmt::formatter<spelling_bee::constraints> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(spelling_bee::constraints constraints, FormatContext& ctx)
    {
        fmt::format_to(ctx.out(), "\"{}", constraints.center);

        for (auto const& l : constraints.allowed) {
            if (l == constraints.center) {
                continue;
            }

            fmt::format_to(ctx.out(), "{}", l);
        }

        fmt::format_to(ctx.out(), "\"");
        return ctx.out();
    }
};

auto read_constraints(fuzzer_input in)
        -> std::optional<spelling_bee::constraints>
{
    auto const c{in.read_object<spelling_bee::constraints>()};
    if (!c || !spelling_bee::valid(*c)) {
        return std::nullopt;
    }

    return c;
}

extern "C" auto LLVMFuzzerTestOneInput(std::uint8_t const* data, std::size_t size)
        -> int
{
    auto const c{read_constraints(fuzzer_input{data, size})};
    if (!c) {
        return 0;
    }

    fmt::print("calling spelling_bee::solve({})\n", *c);
    spelling_bee::solve(*c);

    return 0;
}
