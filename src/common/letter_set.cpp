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

#include "letter_set.h"

#include <cctype>

letter_set const letter_set::all = {letter_set{
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z'}};
letter_set const letter_set::none = {letter_set{}};

[[nodiscard]] auto sentinel(letter_set const& letters)
        -> letter_set::const_iterator
{
    auto const bits{letters.bits() | letter_set{'Z' + 1}.bits()};
    return letter_set::const_iterator{'A' - 1, bits};
}

auto begin(letter_set const& letters)
        -> letter_set::const_iterator
{
    return std::next(sentinel(letters));
}

auto end(letter_set const& /*letters*/)
        -> letter_set::const_iterator
{
    return letter_set::const_iterator{'Z' + 1, 1};
}

[[nodiscard]] auto letter_set::find(char l) const
        -> letter_set::const_iterator
{
    WSS_ASSERT(std::isupper(l));

    auto const i = index(l);
    auto const b = rep{1} << i;
    if ((_bits & b) == 0) {
        return end(*this);
    }

    auto s{sentinel(*this)};
    return letter_set::const_iterator{char(s._letter + i), s._bits >> i};
}
