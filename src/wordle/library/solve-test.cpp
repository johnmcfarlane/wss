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
#include "letter_set.h"
#include "letter_values.h"
#include <wordle/word.h>

#include <catch2/catch.hpp>

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

SCENARIO("Wordle", "solver")
{
    GIVEN("default-initialised constraints")
    {
        auto const p{wordle::constraints{}};

        WHEN("asked to solve")
        {
            auto const actual{wordle::solve(p)};

            THEN("result is an empty list of words")
            {
                auto const expected{wordle::words{}};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("open constraints")
    {
        auto p{wordle::constraints{}};
        std::fill(std::begin(p.maximum), std::end(p.maximum), wordle::word_size);
        std::fill(std::begin(p.allowed), std::end(p.allowed), letter_set::all);

        WHEN("asked to solve")
        {
            auto const suggestions{wordle::solve(p)};

            THEN("result is a large list of words")
            {
                REQUIRE(suggestions.size() == 8940);
            }
            THEN("result is a sorted list of words")
            {
                REQUIRE(std::is_sorted(std::begin(suggestions), std::end(suggestions)));
            }
            THEN("result is a unique list of words")
            {
                REQUIRE(std::adjacent_find(std::begin(suggestions), std::end(suggestions)) == std::end(suggestions));
            }
        }
    }

    GIVEN("constraints that each of 'Z', 'Y', 'M', 'E' and 'S' occurs at most once")
    {
        auto p{wordle::constraints{}};
        p.maximum['Z'] = 1;
        p.maximum['Y'] = 1;
        p.maximum['M'] = 1;
        p.maximum['E'] = 1;
        p.maximum['S'] = 1;
        std::fill(std::begin(p.allowed), std::end(p.allowed), letter_set::all);

        WHEN("asked to solve")
        {
            auto const actual{wordle::solve(p)};

            THEN("result is {\"ZYMES\"}")
            {
                auto const zymes{std::array<char, 5>{'Z', 'Y', 'M', 'E', 'S'}};
                auto const expected{wordle::words{zymes}};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("constraints that each of 'Y', 'M', 'E' and 'S' occurs at least once")
    {
        auto p{wordle::constraints{}};
        p.minimum['Z'] = 1;
        p.minimum['Y'] = 1;
        p.minimum['M'] = 1;
        p.minimum['E'] = 1;
        std::fill(std::begin(p.maximum), std::end(p.maximum), wordle::word_size);
        std::fill(std::begin(p.allowed), std::end(p.allowed), letter_set::all);

        WHEN("asked to solve")
        {
            auto const actual{wordle::solve(p)};

            THEN("result is {\"ZYMES\"}")
            {
                auto const expected{wordle::words{{'E', 'N', 'Z', 'Y', 'M'}, {'Z', 'Y', 'M', 'E', 'S'}}};
                REQUIRE(expected == actual);
            }
        }
    }
}
