#include <scores.h>
#include <ssize.h>
#include <tile.h>
#include <wwf_lexicon.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using std::begin;
using std::end;
using std::string_view;
using std::fill;
using std::pair;
using std::string;
using std::tie;
using std::unique;
using std::vector;

struct search_state {
    letter_values const letter_scores;
    letter_values rack{};
    vector<char> word{};
    vector<pair<string, int>> finds{};
};

void search(node const& n, search_state& state, int score, 
        int min_length) noexcept
{
    -- min_length;
    auto const recurse{
            [&](node const& n, int& counter, search_state& state,
                    int score) noexcept {
                if (counter==0) {
                    return;
                }

                if (n.is_terminator && min_length<=0) {
                    state.finds.emplace_back(
                            string{begin(state.word), end(state.word)},
                            score);
                }

                --counter;
                ::search(n, state, score, min_length);
                ++counter;
            }};

    for (auto i{begin(n)}; i!=end(n); ++i) {
        auto letter{i.letter()};
        auto const& child{i.child()};
        state.word.push_back(letter);
        auto const letter_score{state.letter_scores[letter]};
        recurse(child, state.rack[letter], state,
                score+letter_score);
        recurse(child, state.rack[wildcard], state,
                score+letter_score);
        recurse(child, state.rack[blank], state, score);
        state.word.pop_back();
    }
}

void refine_results(vector<pair<string, int>>& finds)
{
    // group by words, highest-scoring first
    sort(begin(finds), end(finds), [](auto a, auto b) {
        return tie(a.first, b.second)<tie(b.first, a.second);
    });

    // eliminate lower-scoring entries with the same word
    auto const first_erased{unique(begin(finds), end(finds),
            [](auto a, auto b) {
                return a.first==b.first;
            })};
    finds.erase(first_erased, end(finds));

    sort(begin(finds), end(finds), [](auto a, auto b) {
        return tie(b.second, a.first)
                <tie(a.second, b.first);
    });
}

auto solve(node const& node, letter_values const& letter_scores,
        string_view letters, int min_length)
{
    search_state state{letter_scores};

    fill(begin(state.rack), end(state.rack), 0);
    for (auto letter : letters) {
        ++state.rack[letter];
    }

    state.word.reserve(letters.size());

    search(node, state, 0, min_length);

    refine_results(state.finds);
    return move(state.finds);
}

auto main(int argc, char const* const* argv) -> int
{
    auto help{false};
    auto min_length{2};
    auto letters{string{}};
    auto cli{
        lyra::help(help)
        | lyra::opt(min_length, "minimum length")["-n"]["--min-length"]("minimum number of letters in words suggested")
        | lyra::arg(letters, "letters")("Letter \"rack\" including wildcards as '?'")
    };
    auto result = cli.parse(lyra::args(argc, argv));

    if (!result) {
        fmt::print(stderr, "Error in command line: {}\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss scrabble word suggester\n"
                    "(C)2019 John McFarlane\n\n");
        for (auto const& help_column : cli.get_help_text()) {
            fmt::printf("%.10s   %s\n", help_column.option, help_column.description);
        }
        return EXIT_FAILURE;
    }

    if (letters.empty()) {
        fmt::print(stderr, "Please provide a 'rack' of letters\n");
        return EXIT_FAILURE;
    }

    if (min_length>ssize(letters)) {
        fmt::print(stderr,
                "Too few letters, {}, to achieve minimum word length, {}\n",
                letters.size(), min_length);
        return EXIT_FAILURE;
    }

    std::transform(begin(letters), end(letters), begin(letters),
            [](auto c) { return std::toupper(c); });

    auto finds{solve(wwf_lexicon, wwf_scores(), letters, min_length)};

    for (auto const& find : finds) {
        puts(find.first.c_str());
    };
}
