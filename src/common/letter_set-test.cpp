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

#include "letter_set.h"

#include <catch2/catch.hpp>

#include <algorithm>
#include <initializer_list>
#include <string>

namespace Catch {
    template<>
    struct StringMaker<letter_set> {
        static auto convert(letter_set const& value) -> std::string
        {
            auto s{std::string{}};
            s += '{';
            for (auto const letter : value) {
                s += letter;
            }
            s += '}';
            return s;
        }
    };
}  // namespace Catch

TEST_CASE("letter_set constants")
{
    REQUIRE(from_bits(0) == letter_set::none);
    REQUIRE(from_bits(0b111111111111111111111111110) == letter_set::all);
}

TEST_CASE("letter_set initialisation")
{
    REQUIRE(from_bits(0) == letter_set{});
    REQUIRE(from_bits(0) == letter_set{std::initializer_list<char>{}});
    REQUIRE(from_bits(0) != letter_set{{}});
}

TEST_CASE("letter_set implicit bit setting")
{
    constexpr auto initial_value{from_bits(0b11110000)};
    auto ls{letter_set{initial_value}};
    ls.reset('f');
    ls.set('a');
    REQUIRE(from_bits(0b10110010) == ls);
}

TEST_CASE("letter_set explicit bit setting")
{
    constexpr auto initial_value{from_bits(0b11110000)};
    auto ls{letter_set{initial_value}};
    ls.set('f', false);
    ls.set('a', true);
    REQUIRE(from_bits(0b10110010) == ls);
}

TEST_CASE("letter_set bitwise ops")
{
    REQUIRE((~from_bits(0b001000000000000000000000000)) == from_bits(0b110111111111111111111111110));
    REQUIRE((letter_set({'a', 'b'}) & letter_set({'b', 'c'})) == letter_set({'b'}));
    REQUIRE((letter_set({'a', 'b'}) | letter_set({'b', 'c'})) == letter_set({'a', 'b', 'c'}));
    REQUIRE((letter_set({'a', 'b'}) ^ letter_set({'b', 'c'})) == letter_set({'a', 'c'}));
}

TEST_CASE("letter_set iteration")
{
    constexpr auto ls{from_bits(0b001000000000010000100000000)};
    auto const count{std::count_if(begin(ls), end(ls), [](auto) { return true; })};
    REQUIRE(3 == count);
}

TEST_CASE("letter_set iterator difference")
{
    constexpr auto ls{from_bits(0b10101010)};
    REQUIRE(4 == end(ls) - begin(ls));
}

TEST_CASE("letter_set iterator indifference")
{
    constexpr auto ls{from_bits(0b0)};
    REQUIRE(begin(ls) == end(ls));
}

TEST_CASE("letter_set find")
{
    constexpr auto ls{from_bits(0b1010)};
    REQUIRE(begin(ls) == ls.find('A'));
    REQUIRE(end(ls) == ls.find('B'));
}
