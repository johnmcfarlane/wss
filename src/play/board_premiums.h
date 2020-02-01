//
// Created by john on 27/10/2019.
//

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
