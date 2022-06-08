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

#include "input.h"
#include <spelling-bee/word.h>

#include <letter_set.h>
#include <node.h>
#include <scrabble_lexicon.h>
#include <wss_assert.h>

namespace {
    template<bool HasCenterLetter>
    auto solve(node const& n, spelling_bee::constraints& c, spelling_bee::word& suggestion, spelling_bee::words& suggestions) -> void
    {
        if (HasCenterLetter && n.letters['\0']) {
            suggestions.emplace_back(suggestion);
        }

        for (auto edge_it{begin(n)}; edge_it != end(n); ++edge_it) {
            auto const edge_letter{*edge_it};

            if (!c.allowed[edge_letter]) {
                continue;
            }

            suggestion.push_back(edge_letter);

            auto const& child{edge_it.child()};
            if (HasCenterLetter || edge_letter == c.center) {
                solve<true>(child, c, suggestion, suggestions);
            } else {
                solve<false>(child, c, suggestion, suggestions);
            }

            suggestion.pop_back();
        }
    }
}  // namespace

auto spelling_bee::solve(constraints c) -> spelling_bee::words
{
    WSS_ASSERT(valid(c));

    spelling_bee::words suggestions;
    spelling_bee::word suggestion;
    ::solve<false>(scrabble_lexicon, c, suggestion, suggestions);
    return suggestions;
}
