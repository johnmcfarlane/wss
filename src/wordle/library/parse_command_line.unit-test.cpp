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

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <array>
#include <cstdlib>
#include <iterator>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace Catch {
    template<>
    struct StringMaker<wordle::query> {
        static auto convert(wordle::query const& value) -> std::string
        {
            auto out{fmt::memory_buffer{}};
            auto it{std::back_inserter(out)};
            fmt::format_to(it, "{{attempts=(");
            auto need_comma{false};
            for (auto const& attempt : value.history) {
                if (need_comma) {
                    fmt::format_to(it, ",");
                }
                need_comma = true;
                fmt::format_to(it, "{{\"{}\",", attempt.guess);
                for (auto score : attempt.feedback) {
                    fmt::format_to(it, "{}", static_cast<int>(score));
                }
                fmt::format_to(it, "}}");
            }
            fmt::format_to(it, ")}}");
            return to_string(out);
        }
    };
}  // namespace Catch

namespace wordle {
    auto operator==(wordle::attempt const& l, wordle::attempt const& r) -> bool
    {
        return std::tie(l.guess, l.feedback) == std::tie(r.guess, r.feedback);
    }

    auto operator==(wordle::query const& l, wordle::query const& r) -> bool
    {
        return std::tie(l.history, l.debug) == std::tie(r.history, r.debug);
    }
}  // namespace wordle

SCENARIO("Empty Wordle command line parameters")
{
    GIVEN("empty command-line parameters")
    {
        constexpr auto command_line{std::array{"wordle"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const actual{wordle::parse_command_line(command_line)};

            THEN("function returns input with empty list of queries")
            {
                auto const expected{wordle::input{wordle::query{{}, false}}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Wordle --help command line parameters")
{
    GIVEN("command-line parameter, \"--help\"")
    {
        constexpr auto command_line{std::array{"wordle", "--help"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const actual{wordle::parse_command_line(command_line)};

            THEN("the function returns just a successful exit code")
            {
                auto const expected{wordle::input{EXIT_SUCCESS}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Wordle regular command line parameters")
{
    GIVEN("an attempt passed in as a command-line parameter")
    {
        constexpr auto command_line{std::array{"wordle", "ABCDE01210"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<wordle::query>(result)};

            THEN("the function returns a query")
            {
                auto const expected{
                        wordle::query{
                                wordle::attempts{
                                        {{'A', 'B', 'C', 'D', 'E'},
                                         {
                                                 wordle::letter_score::miss,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::hit,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::miss,
                                         }}},
                                false}};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("two attempts passed in as command-line parameters")
    {
        constexpr auto command_line{std::array{"wordle", "ABCDE01210,ABCDE01210"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<wordle::query>(result)};

            THEN("the function returns a query")
            {
                auto const expected{
                        wordle::query{
                                wordle::attempts{
                                        {{'A', 'B', 'C', 'D', 'E'},
                                         {
                                                 wordle::letter_score::miss,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::hit,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::miss,
                                         }},
                                        {{'A', 'B', 'C', 'D', 'E'},
                                         {
                                                 wordle::letter_score::miss,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::hit,
                                                 wordle::letter_score::near,
                                                 wordle::letter_score::miss,
                                         }}},
                                false}};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt passed in as a command-line parameter with debug flag")
    {
        constexpr auto command_line{std::array{"wordle", "ABCDE01210", "--debug"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<wordle::query>(result)};

            THEN("the function returns a query with debug flag set")
            {
                auto const expected{wordle::query{
                        wordle::attempts{
                                {{'A', 'B', 'C', 'D', 'E'},
                                 {
                                         wordle::letter_score::miss,
                                         wordle::letter_score::near,
                                         wordle::letter_score::hit,
                                         wordle::letter_score::near,
                                         wordle::letter_score::miss,
                                 }}},
                        true}};
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("Wordle ill-formed regular command line parameters")
{
    GIVEN("command-line parameters with an unrecognised flag")
    {
        constexpr auto command_line{std::array{"wordle", "AAAAA00000", "--this-is-wrong"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt as a command-line parameter with a bad letter")
    {
        constexpr auto command_line{std::array{"wordle", "AAAA!00000"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt as a command-line parameter with an above-limit score")
    {
        constexpr auto command_line{std::array{"wordle", "AAAAA00009"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt as a command-line parameter with an below-limit score")
    {
        constexpr auto command_line{std::array{"wordle", "AAAAA0000 "}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt as a command-line parameter with a missing character")
    {
        constexpr auto command_line{std::array{"wordle", "AAAAA0000"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("an attempt as a command-line parameter with a stray comma")
    {
        constexpr auto command_line{std::array{"wordle", "AAAAA0,000"}};

        WHEN("wordle::parse_command_line is called")
        {
            auto const result{wordle::parse_command_line(command_line)};
            auto const& actual{std::get<int>(result)};

            THEN("the function returns a failure exit code")
            {
                auto const expected{EXIT_FAILURE};
                REQUIRE(expected == actual);
            }
        }
    }
}
