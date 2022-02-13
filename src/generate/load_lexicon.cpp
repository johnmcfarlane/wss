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
#include <string_view>

auto load_lexicon(std::vector<std::string> const& filenames)
        -> std::optional<multi_trie>
{
    auto num_root_nodes = int(filenames.size());
    auto lexicon = multi_trie{num_root_nodes};

    fmt::print("loading...\n");

    for (auto i = 0; i != num_root_nodes; ++i) {
        auto const& filename = filenames[i];

        auto f{std::unique_ptr<std::FILE, decltype(&fclose)>{
                std::fopen(filename.data(), "r"), &fclose}};

        if (!f) {
            fmt::print(stderr, "error: Could not open '{}'.\n", filename);
            return std::nullopt;
        }

        constexpr auto max_word{1024};
        std::array<char, max_word + 1> line{};
        while (std::fgets(line.data(), max_word, f.get()) != nullptr) {
            auto* const line_start{std::begin(line)};
            auto* const line_end{std::end(line)};

            auto* const word_start{line_start};
            auto* const word_end{std::find(line_start, line_end, '\n')};

            if (word_end == line_end) {
                fmt::print(stderr, "error: missing newline at end of '{}' in {}\n", line_start, filename);
                return std::nullopt;
            }

            auto const word{std::string_view{
                    word_start,
                    std::size_t(std::distance(word_start, word_end))}};

            auto const* const invalid_character{
                    std::find_if(word_start, word_end, [](auto c) {
                        return !std::islower(c);
                    })};
            if (invalid_character != word_end) {
                fmt::print(stderr, "error: invalid character, {}, in wordform '{}' in {}\n", int{*invalid_character}, word, filename);
                return std::nullopt;
            }

            std::transform(word_start, word_end, word_start, [](auto c) { return std::toupper(c); });

            lexicon.insert(word, i);
        }
    }

    fmt::print("compressing...\n");
    lexicon.compress();

    return lexicon;
}
