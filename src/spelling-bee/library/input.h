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

#ifndef WSS_SPELLING_BEE_INPUT_H
#define WSS_SPELLING_BEE_INPUT_H

#include <spelling-bee/word.h>

#include <command_line.h>
#include <letter_set.h>
#include <letter_values.h>

#include <fmt/format.h>

#include <array>
#include <compare>
#include <ranges>
#include <string>
#include <variant>
#include <vector>

namespace spelling_bee {
    constexpr auto num_letters{7};

    struct constraints {
        // contains seven letters
        letter_set allowed;

        // is uppercase letter; is found in allowed
        char center{};
    };

    [[nodiscard]] inline auto num_allowed(constraints c)
    {
        return std::popcount(c.allowed.bits());
    }

    [[nodiscard]] inline auto valid(constraints c) -> bool
    {
        if (std::isupper(c.center) == 0) {
            // The center letter is not an upper-case letter.
            return false;
        }

        if (!c.allowed[c.center]) {
            // The center letter is not allowed.
            return false;
        }

        if (num_allowed(c) != num_letters) {
            // The wrong number of letter is allowed.
            return false;
        }

        return true;
    }

    // Either exit program with given status or search with given letters.
    using input = std::variant<int, constraints>;
}  // namespace spelling_bee

#endif  // WSS_SPELLING_BEE_INPUT_H
