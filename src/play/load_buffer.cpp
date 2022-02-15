// Copyright 2019 John McFarlane
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

#include "load_buffer.h"

#include <fmt/printf.h>
#include <gsl/string_span>

#include <cstdio>
#include <memory>
#include <vector>

auto load_buffer(gsl::cstring_span<> filename)
        -> std::optional<std::vector<char>>
{
    auto f{std::unique_ptr<std::FILE, decltype(&fclose)>{
            std::fopen(filename.data(), "r"), &fclose}};

    if (!f) {
        fmt::print(stderr, "error: Could not open '{}'.\n", gsl::to_string(filename));
        return std::nullopt;
    }

    std::fseek(f.get(), 0, SEEK_END);
    auto const file_size{std::ftell(f.get())};
    std::fseek(f.get(), 0, SEEK_SET);

    std::vector<char> buffer(file_size);
    auto const read{std::ptrdiff_t(
            std::fread(buffer.data(), 1, file_size, f.get()))};

    // LCOV_EXCL_START - can only happen if file changes size mid-test
    if (read != file_size) {
        fmt::print(stderr, "error: could not read, '{}'.\n", gsl::to_string(filename));
        return std::nullopt;
    }
    // LCOV_EXCL_STOP

    return buffer;
}
