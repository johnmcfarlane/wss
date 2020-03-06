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

#include "solve.h"

#include "board_premiums.h"
#include "board_tiles.h"

#include <lexicon.h>
#include <scores.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <utility>

namespace {
    using std::string;
} // namespace


auto main(int argc, char const* const* argv) -> int
{
    using namespace std::literals;
    auto const infochimps_name{"infochimps"s};
    auto const scrabble_name{"scrabble"s};
    auto const wwf_name{"wwf"s};

    auto help{false};
    auto letters{string{}};
    auto board_filename{string{}};
    auto premiums_filename{string{}};
    auto game_name{string{wwf_name}};
    auto cli{
        lyra::help(help)
        | lyra::opt(game_name, "-g")["--game"]
                .choices(infochimps_name, scrabble_name, wwf_name)
        | lyra::arg(letters, "letters")("Letter \"rack\" including wildcards as ? and blanks as _")
        | lyra::arg(board_filename, "board")("text file containing played letters")
        | lyra::arg(premiums_filename, "premiums")("text file describing premium tiles (see 'boards' directory)")
    };
    auto result = cli.parse(lyra::args(argc, argv));

    if (!result) {
        fmt::fprintf(stderr, "error: in command line: %s\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss scrabble board suggester\n"
                    "(C)2019 John McFarlane\n\n"
                    "play [options] <rack> <board>\n");
        for (auto const& help_column : cli.get_help_text()) {
            fmt::printf("%.10s   %s\n", help_column.option, help_column.description);
        }
        return EXIT_FAILURE;
    }

    if (letters.empty()) {
        fmt::fprintf(stderr, "Please provide a 'rack' of letters\n");
        return EXIT_FAILURE;
    }

    if (board_filename.empty()) {
        fmt::fprintf(stderr, "Please provide a board layout file\n");
        return EXIT_FAILURE;
    }

    std::transform(begin(letters), end(letters), begin(letters),
            [](auto c) { return std::toupper(c); });

    auto board_tiles{load_board_tiles(board_filename)};
    if (!board_tiles) {
        fmt::print(stderr, "error: failed to load board tiles from {}\n",
                board_filename);
        return EXIT_FAILURE;
    }

    auto board_premiums{load_board_premiums(premiums_filename)};
    if (!board_premiums) {
        if (premiums_filename.empty()) {
            board_premiums.emplace(board_tiles->size());
        }
        else {
            fmt::print(stderr, "error: failed to load board premiums from {}\n",
                    premiums_filename);
            return EXIT_FAILURE;
        }
    }

    if (ssize(*board_premiums)!=ssize(*board_tiles)) {
        fmt::print(stderr,
                "error: premium and tile boards are different sizes ({} and {})\n",
                ssize(*board_premiums), ssize(*board_tiles));
        return EXIT_FAILURE;
    }

    auto const lexicon{std::map{
            std::pair{infochimps_name, infochimps_lexicon},
            std::pair{scrabble_name, scrabble_lexicon},
            std::pair{wwf_name, wwf_lexicon}
    }.at(game_name)};
    auto const scores{std::map{
            std::pair{infochimps_name, wwf_scores()},
            std::pair{scrabble_name, scrabble_scores()},
            std::pair{wwf_name, wwf_scores()}
    }.at(game_name)};
    auto [finds, invalid_words] {
            solve(lexicon, scores, letters, std::move(*board_tiles),
                    std::move(*board_premiums))};

    if (!invalid_words.empty()) {
        fmt::print(stderr, "invalid words:");
        for (auto const& invalid_word : invalid_words) {
            fmt::print(stderr, " {}", invalid_word);
        }
        fmt::print(stderr, "\n");
    }

    for (auto const& find : finds) {
        fmt::print("{1:3} {2:5} {3} {0}\n",
                find.word.c_str(),
                find.score,
                fmt::format("{:2},{:1}", find.pos.start[0]+1,
                        find.pos.start[1]+1),
                (find.pos.direction[0]!=0) ? '-' : '|');
    };
}
