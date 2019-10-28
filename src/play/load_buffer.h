//
// Created by john on 27/10/2019.
//

#ifndef WSS_LOAD_BUFFER_H
#define WSS_LOAD_BUFFER_H

#include <gsl/string_span>

#include <optional>
#include <vector>

auto load_buffer(gsl::cstring_span<> filename)
-> std::optional<std::vector<char>>;

#endif //WSS_LOAD_BUFFER_H
