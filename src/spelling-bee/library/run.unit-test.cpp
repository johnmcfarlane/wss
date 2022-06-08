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

#include <spelling-bee/run.h>

#include <spelling-bee/word.h>

#include <catch2/catch.hpp>

#include <array>
#include <cstdlib>
#include <string>
#include <variant>
#include <vector>

SCENARIO("Spelling Bee entry point")
{
    GIVEN("no command-line parameters")
    {
        constexpr auto params{std::array{"spelling-bee"}};

        WHEN("run")
        {
            auto const result{spelling_bee::run(params)};

            THEN("the function returns failure")
            {
                auto const expected{std::variant<int, spelling_bee::words>{EXIT_FAILURE}};
                REQUIRE(expected == result);
            }
        }
    }

    GIVEN("a query")
    {
        constexpr auto params{std::array{"spelling-bee", "tuvwxyz"}};

        WHEN("run")
        {
            auto const result{spelling_bee::run(params)};

            THEN("a list of many words is returned")
            {
                auto const expected{spelling_bee::words{
                        "TUT",
                        "TUTTY",
                        "TUTU",
                        "TUX",
                        "UT",
                        "YUTZ"}};
                auto const actual{std::get<spelling_bee::words>(result)};
                REQUIRE(expected == actual);
            }
        }
    }

    // GIVEN("help flag")
    // {
    //     constexpr auto params{std::array{"spelling-bee", "--help"}};

    //     WHEN("run")
    //     {
    //         auto const result{spelling_bee::run(params)};

    //         THEN("program exits with success code")
    //         {
    //             auto const expected{EXIT_SUCCESS};
    //             auto const actual{std::get<int>(result)};
    //             REQUIRE(expected == actual);
    //         }
    //     }
    // }

    // GIVEN("an ill-formed attempt")
    // {
    //     constexpr auto params{std::array{"spelling-bee", "x"}};

    //     WHEN("run")
    //     {
    //         auto const result{spelling_bee::run(params)};

    //         THEN("a list of two words is returned")
    //         {
    //             auto const expected{EXIT_FAILURE};
    //             auto const actual{std::get<int>(result)};
    //             REQUIRE(expected == actual);
    //         }
    //     }
    // }

    // GIVEN("impossible attempts")
    // {
    //     constexpr auto params{std::array{"spelling-bee", "AAAAA22222,BBBBB22222"}};

    //     WHEN("run")
    //     {
    //         auto const result{spelling_bee::run(params)};

    //         THEN("a list of two words is returned")
    //         {
    //             auto const expected{EXIT_FAILURE};
    //             auto const actual{std::get<int>(result)};
    //             REQUIRE(expected == actual);
    //         }
    //     }
    // }
}
