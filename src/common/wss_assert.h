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

#ifndef WSS_ASSERT_H
#define WSS_ASSERT_H

#include <exception>

// For testing coverage, assertions are not necessarily a concern.
#if defined(WSS_DISABLE_ASSERTS)
#define WSS_ASSERT(cond)

// In debug builds, fail fast and loud when an assertion is challenged.
#elif !defined(NDEBUG)
#define WSS_ASSERT(cond) ((cond) ? static_cast<void>(0) : std::terminate())

// In optimised GCC builds, optimise/sanitize accordingly.
#elif defined(__GNUC__)
#define WSS_ASSERT(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())

// In optimised MSVC builds, optimise/sanitize accordingly.
#elif defined(_MSC_VER)
#define WSS_ASSERT(cond) __assume(cond)

// In other optimised builds assume code is correct.
#else
#define WSS_ASSERT(cond)

#endif

#endif //WSS_ASSERT_H
