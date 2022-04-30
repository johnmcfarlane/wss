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

#include "constraints.h"

#include "letter_set.h"
#include "letter_values.h"

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <string>

SCENARIO("Wordle compare constraints")
{
    GIVEN("a default-initialised constraints object")
    {
        constexpr auto di{wordle::constraints{}};

        WHEN("compared to another object with non-default minimum")
        {
            constexpr auto c{wordle::constraints{
                    .minimum = {letter_values{}},
                    .maximum = {letter_values{}},
                    .allowed = {}}};

            THEN("objects compare equal")
            {
                REQUIRE(true == (di == c));
            }
        }

        WHEN("compared to a object with non-default minimum")
        {
            constexpr auto c{wordle::constraints{
                    .minimum = {letter_values{42}},
                    .maximum = {},
                    .allowed = {}}};

            THEN("objects don't compare equal")
            {
                REQUIRE(false == (di == c));
            }
        }

        WHEN("compared to a object with non-default maximum")
        {
            constexpr auto c{wordle::constraints{
                    .minimum = {},
                    .maximum = {letter_values{42}},
                    .allowed = {}}};

            THEN("objects don't compare equal")
            {
                REQUIRE(false == (di == c));
            }
        }

        WHEN("compared to a object with non-default allowed")
        {
            constexpr auto c{wordle::constraints{
                    .minimum = {},
                    .maximum = {},
                    .allowed = {letter_set{'A'}}}};

            THEN("objects don't compare equal")
            {
                REQUIRE(false == (di == c));
            }
        }
    }
}

SCENARIO("Wordle format constraints")
{
    GIVEN("a default-initialised constraints object")
    {
        constexpr auto c{wordle::constraints{}};

        WHEN("formatted with {fmt}")
        {
            constexpr auto expected{R"({.limits={A<=0,B<=0,C<=0,D<=0,E<=0,F<=0,G<=0,H<=0,I<=0,J<=0,K<=0,L<=0,M<=0,N<=0,O<=0,P<=0,Q<=0,R<=0,S<=0,T<=0,U<=0,V<=0,W<=0,X<=0,Y<=0,Z<=0}, .disallowed=("ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ","ABCDEFGHIJKLMNOPQRSTUVWXYZ")})"};
            auto const actual{fmt::format("{}", c)};

            THEN("output is human-readable")
            {
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an open constraints object")
    {
        auto c{wordle::open_constraints()};

        WHEN("formatted with {fmt}")
        {
            constexpr auto expected{R"({.limits={}, .disallowed=("","","","","")})"};
            auto const actual{fmt::format("{}", c)};

            THEN("output is human-readable")
            {
                REQUIRE(expected == actual);
            }
        }
    }
}
