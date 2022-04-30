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

#include <wordle/word.h>

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <string>

SCENARIO("Wordle", "Represent 5-letter words")
{
    GIVEN("a word object")
    {
        constexpr auto w{wordle::word{'A', 'B', '*', 'Y', 'Z'}};

        WHEN("formatted with the {fmt} library")
        {
            auto const s{fmt::format("Word is \"{}\"", w)};

            THEN("output contains the word")
            {
                REQUIRE("Word is \"AB*YZ\"" == s);
            }
        }
    }
}
