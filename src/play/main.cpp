#include "solve.h"

#include "board_premiums.h"
#include "board_tiles.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>

namespace {
    using std::string;
} // namespace


auto main(int argc, char const* const* argv) -> int
{
    auto help{false};
    auto lexicon_filename{string{}};
    auto board_filename{string{}};
    auto premiums_filename{string{}};
    auto letters{string{}};
    auto cli{
        lyra::help(help)
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

    auto finds{
            solve(wwf_lexicon, wwf_scores(), letters, std::move(*board_tiles),
                    std::move(*board_premiums))};

    for (auto const& find : finds) {
        fmt::print("{1:3} {2:5} {3} {0}\n",
                find.word.c_str(),
                find.score,
                fmt::format("{:2},{:1}", find.pos.start[0]+1,
                        find.pos.start[1]+1),
                (find.pos.direction[0]!=0) ? '-' : '|');
    };
}
