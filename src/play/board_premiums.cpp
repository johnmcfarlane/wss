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

#include "board_premiums.h"

#include "board.h"

#include <iterator>
#include <optional>
#include <unordered_map>

auto load_board_premiums(gsl::cstring_span<> filename)
        -> std::optional<board<premium>>
{
    std::unordered_map<char, premium> const token_to_premium{
            {' ', premium::normal},
            {'d', premium::dl},
            {'t', premium::tl},
            {'D', premium::dw},
            {'T', premium::tw}};

    if (filename.empty()) {
        return std::nullopt;
    }

    return load_board<premium>(
            filename,
            [&token_to_premium](auto field) -> std::optional<premium> {
                auto found{token_to_premium.find(field)};
                if (found == std::end(token_to_premium)) {
                    return std::nullopt;
                }
                return found->second;
            });
}
