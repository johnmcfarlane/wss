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

#include "command_line.h"
#include <fuzz-test.h>

#include <fmt/printf.h>

#include <cstddef>
#include <cstdint>
#include <optional>

extern "C" auto LLVMFuzzerTestOneInput(std::uint8_t const* data, std::size_t size)
        -> int
{
    auto in{fuzzer_input{data, size}};

    auto const command_line_data{in.read_command_line(7, 41)};
    if (!command_line_data) {
        return 0;
    }

    auto const params{command_line_data->command_line()};

    fmt::print("calling wordle::run({})\n", params);
    wordle::run(params);

    return 0;  // Non-zero return values are reserved for future use.
}
