// Copyright 2022 John McFarlane
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

#ifndef WSS_SPELLING_BEE_WORD_H
#define WSS_SPELLING_BEE_WORD_H

#include <command_line.h>
#include <letter_set.h>
#include <letter_values.h>

#include <fmt/format.h>

#include <array>
#include <vector>

namespace spelling_bee {
    using word = std::string;

    using words = std::vector<spelling_bee::word>;
}  // namespace spelling_bee

template<>
struct fmt::formatter<spelling_bee::word> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(spelling_bee::word const& w, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", std::string_view{w.data(), w.size()});
    }
};

#endif  // WSS_SPELLING_BEE_WORD_H
