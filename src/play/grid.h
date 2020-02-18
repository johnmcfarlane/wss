//
// Created by john on 27/10/2019.
//

#ifndef WSS_GRID_H
#define WSS_GRID_H

#include <gsl/string_span>

#include <vector>

auto parse_grid(gsl::cstring_span<> buffer)
-> std::vector<std::vector<char>>;

#endif //WSS_GRID_H
