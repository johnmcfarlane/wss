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

#include "solve.h"

#include "constraints.h"
#include <wordle/word.h>

#include <letter_set.h>
#include <letter_values.h>
#include <node.h>
#include <wordle_lexicon.h>

#include <array>

namespace {
    auto solve(node const& n, wordle::constraints& c, wordle::word& suggestion, int position, wordle::words& suggestions)
    {
        if (position == wordle::word_size) {
            for (auto l{'A'}; l != 'Z' + 1; ++l) {
                if (c.minimum[l] > 0) {
                    return;
                }
            }

            suggestions.emplace_back(suggestion);
            return;
        }

        for (auto edge_it{begin(n)}; edge_it != end(n); ++edge_it) {
            auto const edge_letter{*edge_it};

            if (!c.allowed[position][edge_letter]) {
                continue;
            }

            if (c.maximum[edge_letter] <= 0) {
                continue;
            }

            c.minimum[edge_letter]--;
            c.maximum[edge_letter]--;
            suggestion[position] = edge_letter;
            solve(edge_it.child(), c, suggestion, position + 1, suggestions);
            c.minimum[edge_letter]++;
            c.maximum[edge_letter]++;
        }
    }
}  // namespace

auto wordle::solve(constraints c) -> wordle::words
{
    validate(c);

    wordle::words suggestions;
    wordle::word suggestion;
    ::solve(wordle_lexicon, c, suggestion, 0, suggestions);
    return suggestions;
}
