#include <scores.h>
#include <ssize.h>
#include <tile.h>
#include <trie.h>
#include <words.h>

#include <clara.hpp>
#include <fmt/printf.h>

#include <algorithm>
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
using std::for_each;
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

void search(node const& n, search_state& state, int score) noexcept
{
    auto const recurse{
            [&](node const& n, int& counter, search_state& state,
                    int score) noexcept {
                if (counter==0) {
                    return;
                }

                if (n.is_terminator) {
                    state.finds.emplace_back(
                            string{begin(state.word), end(state.word)},
                            score);
                }

                --counter;
                ::search(n, state, score);
                ++counter;
            }};

    for_each(begin(n), end(n), [&](auto const& edge) {
        state.word.push_back(edge);
        auto const letter_score{state.letter_scores[edge]};
        recurse(edge.get_next(), state.rack[edge], state,
                score+letter_score);
        recurse(edge.get_next(), state.rack[wildcard], state,
                score+letter_score);
        recurse(edge.get_next(), state.rack[blank], state, score);
        state.word.pop_back();
    });
}

void refine_results(vector<pair<string, int>>& finds)
{
    // group by words, highest-scoring first
    sort(begin(finds), end(finds), [](auto a, auto b) {
        return tie(a.first, b.second)<tie(b.first, a.second);
    });

    // eliminate lower-scoring entries with the same word
    finds.erase(unique(begin(finds), end(finds), [](auto a, auto b) {
        return a.first==b.first;
    }), end(finds));

    sort(begin(finds), end(finds), [](auto a, auto b) {
        return tie(b.second, a.first)
                <tie(a.second, b.first);
    });
}

auto solve(trie const& lexicon, letter_values const& letter_scores,
        string_view letters)
{
    search_state state{letter_scores};

    fill(begin(state.rack), end(state.rack), 0);
    for (auto letter : letters) {
        ++state.rack[letter];
    }

    state.word.reserve(letters.size());

    search(lexicon.root_node(), state, 0);

    refine_results(state.finds);
    return move(state.finds);
}

auto get_lexicon(int min_letters, int max_letters)
{
    trie lexicon;

    for (auto word_ptr : words) {
        auto word{std::string{word_ptr}};

        if (ssize(word)<min_letters) {
            continue;
        }

        if (ssize(word)>max_letters) {
            continue;
        }

        lexicon.insert(word);
    }

    return lexicon;
}

int main(int argc, char const* const* argv)
{
    using clara::Arg;
    using clara::Args;
    using clara::Help;
    using clara::Opt;

    auto help{false};
    auto min_length{2};
    auto letters{string{}};
    auto cli{
            Opt(min_length, "minimum length")["-n"]["--min-length"](
                    "minimum number of letters in words suggested")
                    | Arg(letters, "letters")(
                            "Letter \"rack\" including wildcards as '?'")
                    | Help(help)};
    auto result = cli.parse(Args(argc, argv));

    if (!result) {
        fmt::print(stderr, "Error in command line: {}\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss scrabble word suggester\n"
                    "(C)2019 John McFarlane\n\n");
        for (auto const& help_column : cli.getHelpColumns()) {
            fmt::printf("{10}   {}\n", help_column.left, help_column.right);
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

    transform(begin(letters), end(letters), begin(letters), tolower);

    auto const lexicon{get_lexicon(min_length, letters.size())};

    auto finds{solve(lexicon, wwf_scores(), letters)};

    for (auto const& find : finds) {
        puts(find.first.c_str());
    };
}
