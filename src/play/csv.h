//
// Created by john on 27/10/2019.
//

#ifndef WSS_CSV_H
#define WSS_CSV_H

#include <gsl/string_span>

#include <vector>

auto parse_csv(gsl::cstring_span<> buffer)
-> std::vector<std::vector<gsl::cstring_span<>>>;

#endif //WSS_CSV_H
