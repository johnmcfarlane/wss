#ifndef WSS_SOLVE_H
#define WSS_SOLVE_H

#include "board.h"
#include "board_premiums.h"
#include "result.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <string_view>
#include <vector>

auto solve(
        node const& lexicon, letter_values const& letter_scores,
        std::string_view letters, board<char> tiles,
        board<premium> premiums) -> std::vector<result>;

#endif //WSS_SOLVE_H
