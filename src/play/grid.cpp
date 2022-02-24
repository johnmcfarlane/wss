// Copyright 2019 John McFarlane
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

#include "grid.h"

#include <gsl/span>
#include <gsl/string_span>

#include <algorithm>
#include <iterator>
#include <vector>

namespace {
    auto split(gsl::cstring_span<> paragraph, char delimiter)
            -> std::vector<gsl::cstring_span<>>
    {
        std::vector<gsl::cstring_span<>> fields;

        for (auto end{std::end(paragraph)}, pos{std::begin(paragraph)};
             pos != end;) {
            auto const line_end{std::find(pos, end, delimiter)};
            fields.emplace_back(&*pos, std::distance(pos, line_end));
            pos = std::next(line_end);
        }

        return fields;
    }
}  // namespace

auto parse_grid(gsl::cstring_span<> buffer)
        -> std::vector<std::vector<char>>
{
    std::vector<std::vector<char>> result;
    auto lines{split(buffer, '\n')};
    std::transform(
            std::begin(lines), std::end(lines),
            std::back_inserter(result),
            [](auto line) {
                std::vector<char> squares;
                std::copy(
                        std::begin(line), std::end(line),
                        std::back_inserter(squares));
                return squares;
            });
    return result;
}
