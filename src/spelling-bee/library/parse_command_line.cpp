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

#include <command_line.h>
#include <letter_set.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <string>

namespace {
    auto generate_constraints(std::string const& letters) -> std::optional<spelling_bee::constraints>
    {
        auto const actual_num_letters{letters.size()};
        if (actual_num_letters != spelling_bee::num_letters) {
            fmt::print(stderr, "{} letters provided; {} letters expected\n", actual_num_letters, spelling_bee::num_letters);
            return std::nullopt;
        }

        for (auto const letter : letters) {
            if (std::isalpha(letter) == 0) {
                fmt::print(stderr, "'{}', isn't a letter\n", letter);
                return std::nullopt;
            }
        }

        auto const c{spelling_bee::constraints{
                std::accumulate(begin(letters), end(letters), letter_set::none, [](auto const a, auto const letter) {
                    return a | letter_set{letter};
                }),
                static_cast<char>(std::toupper(letters[0]))}};

        auto const actual_num_allowed{spelling_bee::num_allowed(c)};
        if (actual_num_allowed != spelling_bee::num_letters) {
            fmt::print(stderr, "{} unique letters provided; {} unique letters expected\n", actual_num_allowed, spelling_bee::num_letters);
            return std::nullopt;
        }

        return c;
    }
}  // namespace

auto spelling_bee::parse_command_line(command_line args) -> spelling_bee::input
{
    using namespace std::literals::string_literals;
    auto help{false};
    auto letters{""s};
    auto cli{
            lyra::help(help)
            | lyra::arg(letters, "letters")("seven letters, starting with center letter")};
    auto result = cli.parse(lyra::args(args.size(), args.data()));

    if (!result) {
        fmt::print(stderr, "command line error: {}\n", result.errorMessage());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("Spelling Bee cheater\n"
                    "(C)2022 John McFarlane\n\n");
        std::cout << cli;
        return EXIT_SUCCESS;
    }

    auto const constraints{generate_constraints(letters)};
    if (!constraints) {
        return EXIT_FAILURE;
    }

    return *constraints;
}
