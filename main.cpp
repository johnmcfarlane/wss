#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <experimental/string_view>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <tuple>

#include "trie.h"
#include "words.h"

using std::array;
using std::begin;
using std::cout;
using std::cin;
using std::end;
using std::experimental::string_view;
using std::fill;
using std::max;
using std::numeric_limits;
using std::setw;
using std::string;

//constexpr auto num_columns = 79;
constexpr auto num_columns = 1;

using histogram = array<int, numeric_limits<char>::max()+1>;

auto solve(trie const& lexicon, string_view letters)
{
    histogram rack;
    fill(begin(rack), end(rack), 0);
    for (auto letter : letters) {
        ++rack[letter];
    }

    trie finds;

    lexicon.search(
            [&rack](auto letter) {
                auto& count = rack[letter];
                if (!count) {
                    return false;
                }
                --count;
                return true;
            },
            [&rack](auto letter) {
                ++rack[letter];
            },
            [&finds](auto word) {
                finds.insert(word);
            }
    );

    return finds;
}

auto get_lexicon(string_view letters)
{
    trie lexicon;

    auto it = words;
    while (*it) {
        string_view word = *it++;

        if (word.size()<3) {
            // too short
            continue;
        }

        if (word.size()>letters.size()) {
            // too few letters to make this word
            continue;
        }

        lexicon.insert(word);
    }

    auto const num_words = it-words;
    cout << "\nwords: " << num_words << '\n';
    return lexicon;
}

int main(int argc, char const* const* argv)
{
    string letters;

    cout << "letters: ";
    switch (argc) {
    case 1:
        cin >> letters;
        break;

    case 2:
        letters = argv[1];
        cout << letters << '\n';
        break;

    default:
        cout << "?\n";
        return EXIT_FAILURE;
    }

    cout << "compling lexicon...";
    auto lexicon = get_lexicon(letters);
    cout << "filtered words: " << lexicon.size() << '\n';

    auto finds = solve(lexicon, letters);

    int const max_column = std::max(1, num_columns/(int(letters.length())+1));
    int column = 0;
    finds.for_each([&](auto find) {
        cout << ((column++%max_column) ? ' ' : '\n')
             << find
             << setw(letters.length()+1-find.length());
    });

    cout << "\nfound: " << finds.size() << '\n';

    return EXIT_SUCCESS;
}
