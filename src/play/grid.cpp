//
// Created by john on 27/10/2019.
//

#include "grid.h"

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
            pos = std::next(line_end);
        }

        return fields;
    }
}  // namespace

auto parse_grid(gsl::cstring_span<> buffer)
-> std::vector<std::vector<char>>
{
	std::vector<std::vector<char>> result;
	auto lines{split(buffer, '\n')};
	std::transform(
			std::begin(lines), std::end(lines),
			std::back_inserter(result),
			[](auto line) {
				std::vector<char> squares;
				std::copy(
						std::begin(line), std::end(line),
						std::back_inserter(squares));
				return squares;
			});
    return result;
}
