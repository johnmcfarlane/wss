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

#include "load_lexicon.h"

#include <ssize.h>

#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cctype>

auto load_lexicon(std::string_view filename)
-> std::optional<trie>
{
    trie lexicon;

    auto f{std::unique_ptr<std::FILE, decltype(&fclose)>{
            std::fopen(filename.data(), "r"), &fclose}};

    if (!f) {
        fmt::print(stderr, "error: Could not open '{}'.\n", filename);
        return std::nullopt;
    }

    constexpr auto max_word{1024};
    std::array<char, max_word+1> line{};
    while (std::fgets(line.data(), max_word, f.get())!=nullptr) {
        auto* const newline{std::find(begin(line), end(line), '\n')};
        if (newline==end(line)) {
            fmt::print(stderr, "error: missing newline at end of '{}'\n", line.data());
            return std::nullopt;
        }

        std::transform(std::begin(line), newline, std::begin(line),
                [](auto c) { return std::toupper(c); });

        auto word{std::string_view{line.data(),
                std::size_t(std::distance(begin(line), newline))}};

        lexicon.insert(word);
    }

    lexicon.compress();

    return lexicon;
}
