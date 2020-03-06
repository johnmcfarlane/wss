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

auto load_lexicon(std::vector<std::string> const& filenames)
-> std::optional<multi_trie>
{
    auto num_root_nodes = int(filenames.size());
    auto lexicon = multi_trie{num_root_nodes};

    for (auto i = 0; i!=num_root_nodes; ++i) {
        auto const& filename = filenames[i];

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
                fmt::print(stderr,
                        "error: missing newline at end of '{}' in {}\n",
                        line.data(), filename);
                return std::nullopt;
            }

            if (std::any_of(std::begin(line), newline,
                    [](auto c) {
                        return !std::isalpha(c)
                                || std::isupper(c);
                    })) {
                fmt::print(stderr,
                        "warning: invalid wordform '{}' in {}\n",
                        line.data(), filename);
            }

            std::transform(std::begin(line), newline, std::begin(line),
                    [](auto c) { return std::toupper(c); });

            auto word{std::string_view{line.data(),
                    std::size_t(std::distance(begin(line), newline))}};

            lexicon.insert(word, i);
        }
    }

    lexicon.compress();

    return lexicon;
}
