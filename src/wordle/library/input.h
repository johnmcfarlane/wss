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

#ifndef WSS_WORDLE_INPUT_H
#define WSS_WORDLE_INPUT_H

#include <wordle/word.h>

#include <command_line.h>
#include <letter_set.h>
#include <letter_values.h>

#include <fmt/format.h>

#include <array>
#include <compare>
#include <variant>
#include <vector>

namespace wordle {
    constexpr auto letter_score_bound{3};
    enum class letter_score {
        miss,  // grey; letter places nowhere
        near,  // yellow; letter places elsewhere
        hit  // green; letter places here
    };

    using scores = std::array<letter_score, word_size>;

    // called a try according to documentation
    struct attempt {
        wordle::word guess;
        wordle::scores feedback;
    };

    using attempts = std::vector<attempt>;

    struct query {
        attempts history;
        bool debug;
    };

    // either perform a query, or exit program with given status
    using input = std::variant<int, query>;
}  // namespace wordle

#endif  // WSS_WORDLE_INPUT_H
