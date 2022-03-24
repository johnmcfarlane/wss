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

#ifndef WSS_COMMON_OPEN_FILE_H
#define WSS_COMMON_OPEN_FILE_H

#include <cstdio>
#include <memory>

namespace wss {
    using file_handle = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

    auto open_file(char const* filename, char const* mode) -> file_handle;
}  // namespace wss

#endif  // WSS_COMMON_OPEN_FILE_H
