// Copyright 2019 John McFarlane
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

#ifndef WSS_BOARD_TILES_H
#define WSS_BOARD_TILES_H

#include <gsl/string_span>

#include <optional>

template<typename T>
class board;

auto load_board_tiles(gsl::cstring_span<> filename) -> std::optional<board<char>>;

#endif  // WSS_BOARD_TILES_H
