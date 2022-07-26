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

#include <command_line.h>
#include <wss_assert.h>

#include <fmt/printf.h>

#include <algorithm>
#include <iterator>
#include <variant>
#include <vector>

namespace {
    auto print(spelling_bee::words const& suggestions)
    {
        std::for_each(
                std::cbegin(suggestions), std::cend(suggestions),
                [](auto const& match) {
                    fmt::print("{}\n", match);
                });
    }
}  // namespace

auto main(int argc, char const* const* argv) -> int
{
    // requires args[0] is the program name
    WSS_ASSERT(argc > 0);

    auto const cl{command_line{argv, static_cast<unsigned>(argc)}};
    auto const result{spelling_bee::run(cl)};

    if (auto const exit_status{std::get_if<int>(&result)}) {
        return *exit_status;
    }

    auto const& suggestions{std::get<spelling_bee::words>(result)};
    print(suggestions);
}
