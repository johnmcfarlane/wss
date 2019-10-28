//
// Created by john on 27/10/2019.
//

#include "board_tiles.h"

#include <cctype>
#include <unordered_map>

std::optional<board<char>> load_board_tiles(gsl::cstring_span<> filename)
{
    return load_board<char>(filename, [](auto field) -> std::optional<char> {
        if (field.empty()) {
            return std::optional<char>{'\0'};
        }
        if (field.size()>1) {
            return std::nullopt;
        }

        auto letter{*std::begin(field)};
        if (std::isalpha(letter)) {
            return std::tolower(letter);
        }

        if (letter==vacant) {
            return letter;
        }

        return std::nullopt;
    });
}
