//
// Created by john on 27/10/2019.
//

#include "csv.h"

#include <gsl/string_span>

#include <algorithm>
#include <iterator>
#include <vector>

namespace {
    auto split(gsl::cstring_span<> paragraph, char delimiter)
    -> std::vector<gsl::cstring_span<>>
    {
        std::vector<gsl::cstring_span<>> fields;

        for (auto end{std::end(paragraph)}, pos{std::begin(paragraph)};
                pos!=end;) {
            auto const line_end{std::find(pos, end, delimiter)};
            fields.emplace_back(&*pos, std::distance(pos, line_end));
            if (line_end==end) {
                break;
            }
            pos = std::next(line_end);
        }

        return fields;
    }
}  // namespace

auto parse_csv(gsl::cstring_span<> buffer)
-> std::vector<std::vector<gsl::cstring_span<>>>
{
    std::vector<std::vector<gsl::cstring_span<>>> result;
    auto lines{split(buffer, '\n')};
    std::transform(
            std::begin(lines), std::end(lines),
            std::back_inserter(result),
            [](auto line) {
                return split(line, ',');
            });
    return result;
}
