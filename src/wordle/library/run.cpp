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

#include "generate_constraints.h"
#include "input.h"
#include "parse_command_line.h"
#include "solve.h"

#include <command_line.h>
#include <letter_set.h>

#include <fmt/printf.h>

#include <cstdlib>
#include <optional>

auto wordle::run(command_line args) -> std::variant<int, words>
{
    auto const input{parse_command_line(args)};

    if (auto const* exit_status = std::get_if<int>(&input)) {
        return *exit_status;
    }

    auto const& query{std::get<wordle::query>(input)};
    auto const constraints{generate_constraints(query.history)};
    if (!constraints) {
        return EXIT_FAILURE;
    }

    if (query.debug) {
        fmt::print("{}\n", *constraints);
        return EXIT_SUCCESS;
    }

    return solve(*constraints);
}
