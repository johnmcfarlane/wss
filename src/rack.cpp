#include "trie.h"
#include "words.h"

#include <clara.hpp>
#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <cstdlib>
#include <experimental/string_view>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

using std::array;
using std::begin;
using std::end;
using std::experimental::string_view;
using std::fill;
using std::for_each;
using std::numeric_limits;
using std::pair;
using std::string;
using std::tie;
using std::unique;
using std::vector;

using letter_values = array<int, numeric_limits<char>::max()+1>;

template<typename Container>
inline auto ssize(Container&& c)
{
    return std::ptrdiff_t(c.size());
}

constexpr letter_values scrabble_scores()
{
    constexpr auto letter_score_a = 1;
    constexpr auto letter_score_b = 3;
    constexpr auto letter_score_c = 3;
    constexpr auto letter_score_d = 2;
    constexpr auto letter_score_e = 1;
    constexpr auto letter_score_f = 4;
    constexpr auto letter_score_g = 2;
    constexpr auto letter_score_h = 4;
    constexpr auto letter_score_i = 1;
    constexpr auto letter_score_j = 8;
    constexpr auto letter_score_k = 5;
    constexpr auto letter_score_l = 1;
    constexpr auto letter_score_m = 3;
    constexpr auto letter_score_n = 1;
    constexpr auto letter_score_o = 1;
    constexpr auto letter_score_p = 3;
    constexpr auto letter_score_q = 10;
    constexpr auto letter_score_r = 1;
    constexpr auto letter_score_s = 1;
    constexpr auto letter_score_t = 1;
    constexpr auto letter_score_u = 1;
    constexpr auto letter_score_v = 4;
    constexpr auto letter_score_w = 4;
    constexpr auto letter_score_x = 8;
    constexpr auto letter_score_y = 4;
    constexpr auto letter_score_z = 10;

    letter_values scores{};
    scores['a'] = scores['A'] = letter_score_a;
    scores['b'] = scores['B'] = letter_score_b;
    scores['c'] = scores['C'] = letter_score_c;
    scores['d'] = scores['D'] = letter_score_d;
    scores['e'] = scores['E'] = letter_score_e;
    scores['f'] = scores['F'] = letter_score_f;
    scores['g'] = scores['G'] = letter_score_g;
    scores['h'] = scores['H'] = letter_score_h;
    scores['i'] = scores['I'] = letter_score_i;
    scores['j'] = scores['J'] = letter_score_j;
    scores['k'] = scores['K'] = letter_score_k;
    scores['l'] = scores['L'] = letter_score_l;
    scores['m'] = scores['M'] = letter_score_m;
    scores['n'] = scores['N'] = letter_score_n;
    scores['o'] = scores['O'] = letter_score_o;
    scores['p'] = scores['P'] = letter_score_p;
    scores['q'] = scores['Q'] = letter_score_q;
    scores['r'] = scores['R'] = letter_score_r;
    scores['s'] = scores['S'] = letter_score_s;
    scores['t'] = scores['T'] = letter_score_t;
    scores['u'] = scores['U'] = letter_score_u;
    scores['v'] = scores['V'] = letter_score_v;
    scores['w'] = scores['W'] = letter_score_w;
    scores['x'] = scores['X'] = letter_score_x;
    scores['y'] = scores['Y'] = letter_score_y;
    scores['z'] = scores['Z'] = letter_score_z;
    return scores;
}

constexpr letter_values wwf_scores()
{
    constexpr auto letter_score_a = 1;
    constexpr auto letter_score_b = 4;
    constexpr auto letter_score_c = 4;
    constexpr auto letter_score_d = 2;
    constexpr auto letter_score_e = 1;
    constexpr auto letter_score_f = 4;
    constexpr auto letter_score_g = 3;
    constexpr auto letter_score_h = 3;
    constexpr auto letter_score_i = 1;
    constexpr auto letter_score_j = 10;
    constexpr auto letter_score_k = 5;
    constexpr auto letter_score_l = 2;
    constexpr auto letter_score_m = 4;
    constexpr auto letter_score_n = 2;
    constexpr auto letter_score_o = 1;
    constexpr auto letter_score_p = 4;
    constexpr auto letter_score_q = 10;
    constexpr auto letter_score_r = 1;
    constexpr auto letter_score_s = 1;
    constexpr auto letter_score_t = 1;
    constexpr auto letter_score_u = 1;
    constexpr auto letter_score_v = 5;
    constexpr auto letter_score_w = 4;
    constexpr auto letter_score_x = 8;
    constexpr auto letter_score_y = 3;
    constexpr auto letter_score_z = 10;

    letter_values scores{};
    scores['a'] = scores['A'] = letter_score_a;
    scores['b'] = scores['B'] = letter_score_b;
    scores['c'] = scores['C'] = letter_score_c;
    scores['d'] = scores['D'] = letter_score_d;
    scores['e'] = scores['E'] = letter_score_e;
    scores['f'] = scores['F'] = letter_score_f;
    scores['g'] = scores['G'] = letter_score_g;
    scores['h'] = scores['H'] = letter_score_h;
    scores['i'] = scores['I'] = letter_score_i;
    scores['j'] = scores['J'] = letter_score_j;
    scores['k'] = scores['K'] = letter_score_k;
    scores['l'] = scores['L'] = letter_score_l;
    scores['m'] = scores['M'] = letter_score_m;
    scores['n'] = scores['N'] = letter_score_n;
    scores['o'] = scores['O'] = letter_score_o;
    scores['p'] = scores['P'] = letter_score_p;
    scores['q'] = scores['Q'] = letter_score_q;
    scores['r'] = scores['R'] = letter_score_r;
    scores['s'] = scores['S'] = letter_score_s;
    scores['t'] = scores['T'] = letter_score_t;
    scores['u'] = scores['U'] = letter_score_u;
    scores['v'] = scores['V'] = letter_score_v;
    scores['w'] = scores['W'] = letter_score_w;
    scores['x'] = scores['X'] = letter_score_x;
    scores['y'] = scores['Y'] = letter_score_y;
    scores['z'] = scores['Z'] = letter_score_z;

    return scores;
}

struct search_state {
    letter_values const letter_scores;
    letter_values rack{};
    vector<char> word{};
    vector<pair<string, int>> finds{};
};

void search(trie::edge_const_iterator first, trie::edge_const_iterator last,
        search_state& state, int score) noexcept
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
                ::search(begin(n), end(n), state, score);
                ++counter;
            }};

    for_each(first, last, [&](auto const& edge) {
        state.word.push_back(edge);
        recurse(edge.get_next(), state.rack[edge], state,
                score+state.letter_scores[edge]);
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

auto solve(trie const& lexicon, letter_values const& letter_scores, string_view letters)
{
    search_state state{letter_scores};

    fill(begin(state.rack), end(state.rack), 0);
    for (auto letter : letters) {
        ++state.rack[letter];
    }

    state.word.reserve(letters.size());

    search(begin(lexicon), end(lexicon), state, 0);

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
        fmt::fprintf(stderr, "Error in command line: %s\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss scrabble word suggester\n"
                    "(C)2019 John McFarlane\n\n"
                    "-n, --min-length   minimum word length for suggestions\n");
        return EXIT_FAILURE;
    }

    if (letters.empty()) {
        fmt::fprintf(stderr, "Please provide a 'rack' of letters\n");
        return EXIT_FAILURE;
    }

    if (min_length>ssize(letters)) {
        fmt::fprintf(stderr,
                "Too few letters, %zd, to achieve minimum word length, %d\n",
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
