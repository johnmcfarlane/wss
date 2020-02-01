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
        if (field.empty()) {
            return std::optional<char>{'\0'};
        }
        if (field.size()>1) {
            return std::nullopt;
        }

        // Tool won't tell me where the other identifier is.
        // cppcheck-suppress shadowVar
        auto letter{field[0]};
        if (std::isalpha(letter)) {
            return letter;
        }

        if (letter==vacant) {
            return letter;
        }

        return std::nullopt;
    });
}
