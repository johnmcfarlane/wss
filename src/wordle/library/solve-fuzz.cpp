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

#include "constraints.h"
#include "wordle/word.h"

#include <fuzz-test.h>
#include <letter_set.h>
#include <letter_values.h>

#include <fmt/printf.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

auto read_constraints(fuzzer_input in)
        -> std::optional<wordle::constraints>
{
    auto c{wordle::constraints{}};

    for (auto i{'A'}; i <= 'Z'; ++i) {
        auto const minimum{in.read_integer<int>(0, wordle::word_size)};
        if (!minimum) {
            return std::nullopt;
        }

        auto const maximum{in.read_integer<int>(0, wordle::word_size)};
        if (!maximum) {
            return std::nullopt;
        }

        if (*maximum < *minimum) {
            return std::nullopt;
        }

        c.minimum[i] = *minimum;
        c.maximum[i] = *maximum;
    }

    for (auto i{0}; i != wordle::word_size; ++i) {
        auto const bits{in.read_object<std::uint8_t>()};
        if (!bits) {
            return std::nullopt;
        }

        c.allowed[i] = from_bits(*bits) & letter_set::all;
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

    fmt::print("calling wordle::solve({})\n", *c);
    wordle::solve(*c);

    return 0;  // Non-zero return values are reserved for future use.
}
