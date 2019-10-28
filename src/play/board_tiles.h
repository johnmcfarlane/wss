//
// Created by john on 27/10/2019.
//

#ifndef WSS_BOARD_TILES_H
#define WSS_BOARD_TILES_H

#include "board.h"

#include <tile.h>

#include <gsl/string_span>

#include <optional>

std::optional<board<char>> load_board_tiles(gsl::cstring_span<> filename);

#endif //WSS_BOARD_TILES_H
