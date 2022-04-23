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

#include "constraints.h"
#include "input.h"

#include "letter_set.h"
#include "letter_values.h"

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace Catch {
    template<>
    struct StringMaker<wordle::constraints> {
        static auto convert(wordle::constraints const& value) -> std::string
        {
            return fmt::format("{}", value);
        }
    };
}  // namespace Catch

SCENARIO("Generate Wordle constraints from well-formed attempts")
{
    GIVEN("an empty history of attempts")
    {
        auto history{wordle::attempts{}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns open constraints")
            {
                auto expected{wordle::open_constraints()};

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("\"tales20000\" as the history")
    {
        constexpr auto attempt{wordle::attempt{
                {'t', 'a', 'l', 'e', 's'},
                {wordle::letter_score::hit,
                 wordle::letter_score::miss,
                 wordle::letter_score::miss,
                 wordle::letter_score::miss,
                 wordle::letter_score::miss}}};
        auto const history{wordle::attempts{{attempt}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{*wordle::generate_constraints(history)};

            THEN("returned constraints force T for first letter and exclude the other four from the attempted positions")
            {
                auto expected{wordle::open_constraints()};

                expected.minimum['T'] = 1;
                expected.maximum['A'] = 0;
                expected.maximum['L'] = 0;
                expected.maximum['E'] = 0;
                expected.maximum['S'] = 0;

                expected.allowed[0] = letter_set({'T'});
                expected.allowed[1].reset('A');
                expected.allowed[2].reset('L');
                expected.allowed[3].reset('E');
                expected.allowed[4].reset('S');

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("Two identical attempts with a variety of scores")
    {
        auto const history{wordle::attempts{
                {{'A', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'A', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function succeeds")
            {
                auto expected{wordle::open_constraints()};

                expected.maximum['A'] = 1;
                expected.minimum['B'] = 1;

                expected.allowed[0] = letter_set({'A'});
                expected.allowed[1].reset('B');
                expected.allowed[2].reset('A');
                expected.allowed[3].reset('A');
                expected.allowed[4].reset('A');

                REQUIRE(expected == *actual);
            }
        }
    }

    GIVEN("Two attempts with first a hit, then a near miss in the same spot")
    {
        auto const history{wordle::attempts{
                {{'A', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'B', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::near,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function succeeds")
            {
                auto expected{wordle::open_constraints()};

                expected.maximum['A'] = 0;
                expected.minimum['B'] = 2;

                expected.allowed[0] = letter_set({'A'});
                expected.allowed[1].reset('B');
                expected.allowed[2].reset('A');
                expected.allowed[3].reset('A');
                expected.allowed[4].reset('A');

                REQUIRE(expected == *actual);
            }
        }
    }

    GIVEN("Two attempts with first a hit, then a miss in the same spot")
    {
        auto const history{wordle::attempts{
                {{'A', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'B', 'B', 'a', 'a', 'a'},
                 {wordle::letter_score::miss,
                  wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function succeeds")
            {
                auto expected{wordle::open_constraints()};

                expected.maximum['A'] = 0;
                expected.minimum['B'] = 1;
                expected.maximum['B'] = 1;

                expected.allowed[0] = letter_set({'A'});
                expected.allowed[1].reset('B');
                expected.allowed[2].reset('A');
                expected.allowed[3].reset('A');
                expected.allowed[4].reset('A');

                REQUIRE(expected == *actual);
            }
        }
    }
}

SCENARIO("Fail to generate Wordle constraints from ill-formed attempts")
{
    GIVEN("two attempts with a conflicting hit for the first letter")
    {
        auto const history{wordle::attempts{
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'B', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns failure")
            {
                auto expected{std::nullopt};

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("two attempts with a conflicting score (hit and miss) for the first letter")
    {
        auto const history{wordle::attempts{
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns failure")
            {
                auto expected{std::nullopt};

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("two attempts with a conflicting score (hit and near) for the first letter")
    {
        auto const history{wordle::attempts{
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns failure")
            {
                auto expected{std::nullopt};

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("twenty six attempts which each claim that a different letter is a near miss")
    {
        auto history{wordle::attempts{}};
        for (auto l{'A'}; l <= 'Z'; ++l) {
            history.push_back(wordle::attempt{
                    {l, 'a', 'a', 'a', 'a'},
                    {wordle::letter_score::near,
                     wordle::letter_score::miss,
                     wordle::letter_score::miss,
                     wordle::letter_score::miss,
                     wordle::letter_score::miss}});
        }

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns failure")
            {
                auto expected{std::nullopt};

                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("two attempts with same first letter, but contradictory scores (near, then hit)")
    {
        auto const history{wordle::attempts{
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::near,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}},
                {{'A', 'a', 'a', 'a', 'a'},
                 {wordle::letter_score::hit,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss,
                  wordle::letter_score::miss}}}};

        WHEN("wordle::generate_constraints is called")
        {
            auto const actual{wordle::generate_constraints(history)};

            THEN("function returns failure")
            {
                auto expected{std::nullopt};

                REQUIRE(expected == actual);
            }
        }
    }
}
