#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <experimental/string_view>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "trie.h"
#include "words.h"

using std::array;
using std::begin;
using std::end;
using std::experimental::string_view;
using std::fill;
using std::for_each;
using std::fprintf;
using std::max;
using std::numeric_limits;
using std::pair;
using std::string;
using std::tie;
using std::unique;
using std::unordered_map;
using std::vector;

using letter_counts = array<int, numeric_limits<char>::max()+1>;

constexpr letter_counts scrabble_scores()
{
    letter_counts scores{};
    scores['a'] = scores['A'] = 1;
    scores['b'] = scores['B'] = 3;
    scores['c'] = scores['C'] = 3;
    scores['d'] = scores['D'] = 2;
    scores['e'] = scores['E'] = 1;
    scores['f'] = scores['F'] = 4;
    scores['g'] = scores['G'] = 2;
    scores['h'] = scores['H'] = 4;
    scores['i'] = scores['I'] = 1;
    scores['j'] = scores['J'] = 8;
    scores['k'] = scores['K'] = 5;
    scores['l'] = scores['L'] = 1;
    scores['m'] = scores['M'] = 3;
    scores['n'] = scores['N'] = 1;
    scores['o'] = scores['O'] = 1;
    scores['p'] = scores['P'] = 3;
    scores['q'] = scores['Q'] = 10;
    scores['r'] = scores['R'] = 1;
    scores['s'] = scores['S'] = 1;
    scores['t'] = scores['T'] = 1;
    scores['u'] = scores['U'] = 1;
    scores['v'] = scores['V'] = 4;
    scores['w'] = scores['W'] = 4;
    scores['x'] = scores['X'] = 8;
    scores['y'] = scores['Y'] = 4;
    scores['z'] = scores['Z'] = 10;
    return scores;
}

constexpr letter_counts wwf_scores()
{
    letter_counts scores{};
    scores['a'] = scores['A'] = 1;
    scores['b'] = scores['B'] = 4;
    scores['c'] = scores['C'] = 4;
    scores['d'] = scores['D'] = 2;
    scores['e'] = scores['E'] = 1;
    scores['f'] = scores['F'] = 4;
    scores['g'] = scores['G'] = 3;
    scores['h'] = scores['H'] = 3;
    scores['i'] = scores['I'] = 1;
    scores['j'] = scores['J'] = 10;
    scores['k'] = scores['K'] = 5;
    scores['l'] = scores['L'] = 2;
    scores['m'] = scores['M'] = 4;
    scores['n'] = scores['N'] = 2;
    scores['o'] = scores['O'] = 1;
    scores['p'] = scores['P'] = 4;
    scores['q'] = scores['Q'] = 10;
    scores['r'] = scores['R'] = 1;
    scores['s'] = scores['S'] = 1;
    scores['t'] = scores['T'] = 1;
    scores['u'] = scores['U'] = 1;
    scores['v'] = scores['V'] = 5;
    scores['w'] = scores['W'] = 4;
    scores['x'] = scores['X'] = 8;
    scores['y'] = scores['Y'] = 3;
    scores['z'] = scores['Z'] = 10;
    return scores;
}

struct search_state {
    letter_counts rack{};
    vector<char> word;
    vector<pair<string, int>> finds;
};

void search(trie::edge_const_iterator first, trie::edge_const_iterator last,
        search_state& state, int score) noexcept
{
    auto const recurse{
            [&](node const& n, int& counter, search_state& state,
                    int score) noexcept {
                if (!counter) {
                    return;
                }

                if (n.is_terminator) {
                    state.finds.emplace_back(
                            string{begin(state.word), end(state.word)},
                            score);
                }

                --counter;
                ::search(begin(n), end(n), state, score);
                ++counter;
            }};

    for_each(first, last, [&](auto const& edge) {
        state.word.push_back(edge);
        recurse(edge.get_next(), state.rack[edge], state,
                score+scrabble_scores()[edge]);
        recurse(edge.get_next(), state.rack['?'], state, score);
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

auto solve(trie const& lexicon, string_view letters)
{
    search_state state;

    fill(begin(state.rack), end(state.rack), 0);
    for (auto letter : letters) {
        ++state.rack[letter];
    }

    state.word.reserve(letters.size());

    search(begin(lexicon), end(lexicon), state, 0);

    refine_results(state.finds);
    return move(state.finds);
}

auto get_lexicon(string_view letters)
{
    trie lexicon;

    auto it{words};
    while (*it) {
        auto const word{string{*it++}};

        if (word.size()<2) {
            // too short
            continue;
        }

        if (word.size()>letters.size()) {
            // too few letters to make this word
            continue;
        }

        lexicon.insert(word);
    }

    return lexicon;
}

int main(int argc, char const* const* argv)
{
    if (argc!=2) {
        fprintf(stderr,
                "Please provide your letters (or wildcards with '?').\n");
        return EXIT_FAILURE;
    }

    auto letters{string{argv[1]}};
    transform(begin(letters), end(letters), begin(letters), tolower);

    auto const lexicon{get_lexicon(letters)};

    auto finds{solve(lexicon, letters)};

    for (auto const& find : finds) {
        puts(find.first.c_str());
    };
}
