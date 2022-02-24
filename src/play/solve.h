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

#ifndef WSS_SOLVE_H
#define WSS_SOLVE_H

#include "board_premiums.h"

#include <letter_values.h>

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

struct node;
struct result;
template<typename T>
class board;

auto solve(
        node const& lexicon, letter_values const& letter_scores,
        std::string_view letters, board<char> tiles,
        board<premium> premiums)
        -> std::tuple<std::vector<result>, std::vector<std::string>>;

#endif  // WSS_SOLVE_H
