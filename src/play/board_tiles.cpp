//
// Created by john on 27/10/2019.
//

#include "board_tiles.h"

#include <cctype>
#include <unordered_map>

auto load_board_tiles(gsl::cstring_span<> filename)
-> std::optional<board<char>>
{
    return load_board<char>(filename, [](auto field) -> std::optional<char> {
        if (std::isalpha(field)) {
            return field;
        }

        if (field==vacant) {
            return field;
        }

        return std::nullopt;
    });
}
