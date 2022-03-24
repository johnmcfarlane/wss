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

#include "open_file.h"

#include <gsl/pointers>

#include <cstdio>

#if defined(_MSC_VER)
#include <stdio.h>
#endif

auto wss::open_file(char const* filename, char const* mode) -> file_handle
{
#if defined(_MSC_VER)
    FILE* f{};
    fopen_s(&f, filename, mode);
#else
    auto* const f{gsl::owner<FILE*>{std::fopen(filename, mode)}};
#endif
    return {f, &fclose};
}
