#ifndef WSS_SOLVE_H
#define WSS_SOLVE_H

#include "board_premiums.h"

#include "board.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <string_view>
#include <vector>

struct ray {
    coord start{};
    coord direction{};
};

struct result {
    std::string word;
    int score;
    ray pos;
};

auto solve(
        node const& lexicon, letter_values const& letter_scores,
        std::string_view letters, board<char> tiles,
        board<premium> premiums) -> std::vector<result>;

#endif //WSS_SOLVE_H
