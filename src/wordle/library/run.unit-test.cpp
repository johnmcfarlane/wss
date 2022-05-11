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

#include <wordle/run.h>

#include <wordle/word.h>

#include <catch2/catch.hpp>

#include <array>
#include <cstdlib>
#include <variant>
#include <vector>

SCENARIO("Wordle entry point")
{
    GIVEN("no command-line parameters")
    {
        constexpr auto params{std::array{"wordle"}};

        WHEN("run")
        {
            auto const result{wordle::run(params)};

            THEN("a list of many words is returned")
            {
                constexpr auto expected{8940};
                auto const actual{std::get<wordle::words>(result).size()};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("a query")
    {
        constexpr auto params{std::array{"wordle", "XYMES02222"}};

        WHEN("run")
        {
            auto const result{wordle::run(params)};

            THEN("a list of two words is returned")
            {
                auto const expected{wordle::words{{'C', 'Y', 'M', 'E', 'S'}, {'Z', 'Y', 'M', 'E', 'S'}}};
                auto const actual{std::get<wordle::words>(result)};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("a query with debug flag")
    {
        constexpr auto params{std::array{"wordle", "XYMES02222", "--debug"}};

        WHEN("run")
        {
            auto const result{wordle::run(params)};

            THEN("program exits with success code")
            {
                auto const expected{EXIT_SUCCESS};
                auto const actual{std::get<int>(result)};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an ill-formed attempt")
    {
        constexpr auto params{std::array{"wordle", "x"}};

        WHEN("run")
        {
            auto const result{wordle::run(params)};

            THEN("a list of two words is returned")
            {
                auto const expected{EXIT_FAILURE};
                auto const actual{std::get<int>(result)};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("impossible attempts")
    {
        constexpr auto params{std::array{"wordle", "AAAAA22222,BBBBB22222"}};

        WHEN("run")
        {
            auto const result{wordle::run(params)};

            THEN("a list of two words is returned")
            {
                auto const expected{EXIT_FAILURE};
                auto const actual{std::get<int>(result)};
                REQUIRE(expected == actual);
            }
        }
    }
}
