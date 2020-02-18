//
// Created by john on 27/10/2019.
//

#include "board_premiums.h"

#include "board.h"

#include <optional>
#include <unordered_map>

auto load_board_premiums(gsl::cstring_span<> filename)
-> std::optional<board<premium>>
{
    std::unordered_map<std::string, premium> const token_to_premium{
            // LCOV_EXCL_START - statically defined
            {" ", premium::normal},
            {"d", premium::dl},
            {"t", premium::tl},
            {"D", premium::dw},
            {"T", premium::tw}
            // LCOV_EXCL_STOP
    };

    if (filename.empty()) {
        return std::nullopt;
    }

    return load_board<premium>(
            filename,
            [&token_to_premium](auto field) -> std::optional<premium> {
        auto found{token_to_premium.find(std::string(
                std::begin(field),
                std::end(field)))};
        if (found==std::end(token_to_premium)) {
            return std::nullopt;
        }
        return found->second;
    });
}
