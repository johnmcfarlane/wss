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

#include <wordle/word.h>

#include <command_line.h>
#include <wss_assert.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {
    auto generate_history(std::string const& input) -> std::optional<wordle::attempts>
    {
        auto result{wordle::attempts{}};
        if (input.empty()) {
            return result;
        }

        auto const submit{[&result](auto const m) {
            auto const parse_guess{[](auto const n) -> std::optional<wordle::word> {
                WSS_ASSERT(n.size() == wordle::word_size);
                auto guess{wordle::word{}};
                std::copy(std::begin(n), std::end(n), std::begin(guess));

                auto const bad_letter{std::find_if(std::cbegin(guess), std::cend(guess), [](auto const letter) {
                    return !std::isalpha(letter);
                })};
                if (bad_letter != std::cend(guess)) {
                    fmt::print(
                            stderr,
                            "guess, \"{}\" contains invalid letter, '{}'\n",
                            n,
                            *bad_letter);
                    return std::nullopt;
                }
                return guess;
            }};
            auto const parse_feedback{[](auto const n) -> std::optional<wordle::scores> {
                auto feedback{wordle::scores{}};
                constexpr auto index_to_score{std::array<wordle::letter_score, wordle::letter_score_bound>{
                        wordle::letter_score::miss, wordle::letter_score::near, wordle::letter_score::hit}};
                for (auto position{0}; position != wordle::word_size; position++) {
                    auto const digit{n[position]};
                    if (digit < '0' || digit > '2') {
                        fmt::print(
                                stderr,
                                "guess, \"{}\" contains invalid score, {}, at position, {}; range is [0..2]\n",
                                n, digit, position);
                        return std::nullopt;
                    }
                    feedback[position] = index_to_score[digit - '0'];
                }
                return feedback;
            }};
            if (m.size() != wordle::word_size * 2) {
                fmt::print(stderr, "Move, \"{}\", isn't in correct format: {} letters and {} numbers.\n", m, wordle::word_size, wordle::word_size);
                return false;
            }
            auto const first{m.data()};
            auto const middle{std::next(first, wordle::word_size)};
            auto const guess{parse_guess(std::string_view{first, wordle::word_size})};
            auto const feedback{parse_feedback(std::string_view{middle, wordle::word_size})};
            if (!guess || !feedback) {
                return false;
            }
            result.emplace_back(wordle::attempt{*guess, *feedback});
            return true;
        }};

        for (auto it{input.data()}, attempt_start{it};; it++) {
            auto const character{*it};
            auto const length{std::distance(attempt_start, it)};
            auto const attempt_view{std::string_view{attempt_start, static_cast<std::size_t>(length)}};
            switch (character) {
            case ',':
                if (!submit(attempt_view)) {
                    return std::nullopt;
                }
                ++it;
                attempt_start = it;
                break;
            case '\0':
                if (!submit(attempt_view)) {
                    return std::nullopt;
                }
                return result;
            }
        }
    }
}  // namespace

auto wordle::parse_command_line(command_line args) -> wordle::input
{
    using namespace std::literals::string_literals;
    auto help{false};
    auto debug{false};
    auto history_input{""s};
    auto cli{
            lyra::help(help)
            | lyra::opt(debug)["-d"]["--debug"]("display internal search data for command")
            | lyra::arg(history_input, "history")("comma-separated list of 10-character moves, e.g. answer is OTHER, you guess TALES then COMET: \"tales10020,comet01021\"")};
    auto result = cli.parse(lyra::args(args.size(), args.data()));

    if (!result) {
        fmt::print(stderr, "command line error: {}\n", result.errorMessage());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("Wordle cheater\n"
                    "(C)2022 John McFarlane\n\n");
        std::cout << cli;
        return EXIT_SUCCESS;
    }

    auto const history{generate_history(history_input)};
    if (!history) {
        return EXIT_FAILURE;
    }

    return query{*history, debug};
}
