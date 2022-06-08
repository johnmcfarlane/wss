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

#ifndef WSS_SPELLING_BEE_SOLVE_H
#define WSS_SPELLING_BEE_SOLVE_H

#include <spelling-bee/word.h>

namespace spelling_bee {
    struct constraints;

    auto solve(constraints c) -> words;
}  // namespace spelling_bee

#endif  // WSS_SPELLING_BEE_SOLVE_H
