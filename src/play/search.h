// Copyright 2020 John McFarlane
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

#ifndef WSS_SEARCH_H
#define WSS_SEARCH_H

#include "board.h"
#include "board_premiums.h"
#include "coord.h"
#include "result.h"

#include <scores.h>
#include <wwf_lexicon.h>

struct initial_state {
    ray pos;
    coord cross_direction{};
    letter_values letter_scores;
    board<char> tiles;
    board<premium> premiums;
    board<bool> qualifying_cells;
    std::vector<char>::iterator word;
    node lexicon;
    int rack_size;
};

struct step_state {
    std::vector<char>::iterator word_end;
    std::vector<result>& finds;
    letter_values& rack;

    coord pos{};

    // letters from the rack on the board used in this word
    int rack_remaining{0};

    // letters already on the board touching this word
    int num_qualifying_cells{0};

    // accumulates cross scores until the end
    int cross_scores{};
};

struct search_state {
    initial_state const& init;
    step_state& step;
};

void search(search_state state);

#endif //WSS_SEARCH_H
