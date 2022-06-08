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

#include "parse_command_line.h"

#include "input.h"

#include <letter_set.h>

#include <catch2/catch.hpp>

#include <array>
#include <cstdlib>
#include <tuple>
#include <variant>

namespace spelling_bee {
    auto operator==(spelling_bee::constraints const& l, spelling_bee::constraints const& r) -> bool
    {
        return std::tie(l.allowed, l.center) == std::tie(r.allowed, r.center);
    }
}  // namespace spelling_bee

SCENARIO("Empty Spelling Bee command line parameters")
{
    GIVEN("empty command-line parameters")
    {
        constexpr auto params{std::array{"spelling-bee"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("function returns failure")
            {
                auto const expected{spelling_bee::input{EXIT_FAILURE}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Spelling Bee --help command line parameters")
{
    GIVEN("command-line parameter, \"--help\"")
    {
        constexpr auto params{std::array{"spelling-bee", "--help"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("the function returns success")
            {
                auto const expected{spelling_bee::input{EXIT_SUCCESS}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Spelling Bee query with non-letter")
{
    GIVEN("command-line parameter, \"abcdef7\"")
    {
        constexpr auto params{std::array{"spelling-bee", "abcdef7"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("the function returns failure")
            {
                auto const expected{spelling_bee::input{EXIT_FAILURE}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Spelling Bee query with too few letters")
{
    GIVEN("command-line parameter, \"abcdef\"")
    {
        constexpr auto params{std::array{"spelling-bee", "abcdef"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("the function returns failure")
            {
                auto const expected{spelling_bee::input{EXIT_FAILURE}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Spelling Bee query with too few unique letters")
{
    GIVEN("command-line parameter, \"abcdeff\"")
    {
        constexpr auto params{std::array{"spelling-bee", "abcdeff"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("the function returns failure")
            {
                auto const expected{spelling_bee::input{EXIT_FAILURE}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Spelling Bee query with too many parameters")
{
    GIVEN("command-line parameters, \"a\" and \"b\"")
    {
        constexpr auto params{std::array{"spelling-bee", "a", "b"}};

        WHEN("spelling_bee::parse_command_line is called")
        {
            auto const actual{spelling_bee::parse_command_line(params)};

            THEN("the function returns failure")
            {
                auto const expected{spelling_bee::input{EXIT_FAILURE}};
                REQUIRE(expected == actual);
            }
        }
    }
}
