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

#include "trie.h"

#include "open_file.h"

#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <string_view>

auto load_lexicon(std::string const& filename)
        -> std::optional<trie>
{
    auto lexicon = trie{};

    fmt::print("loading...\n");

    auto const f{wss::open_file(filename.data(), "r")};

    if (!f) {
        fmt::print(stderr, "error: Could not open '{}'.\n", filename);
        return std::nullopt;
    }

    constexpr auto max_word{1024};
    std::array<char, max_word + 1> line{};
    while (std::fgets(line.data(), max_word, f.get()) != nullptr) {
        auto const line_start{std::begin(line)};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)
        auto const line_end{std::end(line)};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)

        auto const word_start{line_start};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)
        auto const word_end{std::find(line_start, line_end, '\n')};  // NOLINT(llvm-qualified-auto,readability-qualified-auto)

        if (word_end == line_end) {
            fmt::print(stderr, "error: missing newline at end of '{}' in {}\n", line.data(), filename);
            return std::nullopt;
        }

        auto const word{std::string_view{
                line.data(),
                static_cast<std::size_t>(std::distance(word_start, word_end))}};

        // NOLINTNEXTLINE(llvm-qualified-auto,readability-qualified-auto)
        auto const invalid_character{
                std::find_if(word_start, word_end, [](auto c) {
                    return !std::islower(c);
                })};
        if (invalid_character != word_end) {
            fmt::print(stderr, "error: invalid character, {}, in wordform '{}' in {}\n", int{*invalid_character}, word, filename);
            return std::nullopt;
        }

        std::transform(word_start, word_end, word_start, [](auto c) { return std::toupper(c); });

        lexicon.insert(word);
    }

    fmt::print("compressing...\n");
    lexicon.compress();

    return lexicon;
}
