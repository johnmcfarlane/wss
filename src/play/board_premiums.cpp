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
    if (filename.empty()) {
        return std::nullopt;
    }
    
    std::unordered_map<std::string, premium> const token_to_premium{
            {"  ",   premium::normal},
            {"dl", premium::dl},
            {"tl", premium::tl},
            {"dw", premium::dw},
            {"tw", premium::tw}
    };

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
