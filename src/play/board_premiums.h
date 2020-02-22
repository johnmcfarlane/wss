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

#ifndef WSS_BOARD_PREMIUMS_H
#define WSS_BOARD_PREMIUMS_H

#include <gsl/string_span>

#include "board.h"

enum class premium {
    normal,
    dl,
    tl,
    dw,
    tw
};

constexpr std::array<int, 5> letter_multipliers {
        1,
        2,
        3,
        1,
        1
};

constexpr std::array<int, 5> word_multipliers {
        1,
        1,
        1,
        2,
        3
};

auto load_board_premiums(gsl::cstring_span<> filename)
-> std::optional<board<premium>>;

#endif //WSS_BOARD_PREMIUMS_H
