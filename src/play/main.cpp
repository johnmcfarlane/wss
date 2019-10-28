#include "board.h"
#include "board_premiums.h"
#include "board_tiles.h"
#include "coord.h"

#include <scores.h>
#include <trie.h>
#include <words.h>

#include <clara.hpp>
#include <fmt/printf.h>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstdlib>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {
    using std::begin;
    using std::end;
    using std::string;
    using std::tie;

    struct result {
        std::string word;
        int score;
        coord start;
        coord direction;
    };

    struct search_state {
        letter_values const letter_scores;
        board<char> const& board_tiles;
        board<premium> const& board_premiums;
        board<bool> const& board_neighbours;
        trie const& lexicon;
        int const edge;

        // letters from the rack on the board used in this word
        int rack_used{0};

        // letters already on the board touching this word
        int num_neighbours{0};

        letter_values rack{};
        coord start{};
        coord pos{};
        coord direction{};

        // accumulates cross scores until the end
        int cross_scores{};

        std::vector<char> word{};
        std::vector<result> finds{};
    };

    template<typename T>
    T get(board<T> const& b, coord pos, T oob)
    {
        if (pos[0]<0) {
            return oob;
        }
        if (pos[1]<0) {
            return oob;
        }
        auto const edge{b.size()};
        if (pos[0]>=edge) {
            return oob;
        }
        if (pos[1]>=edge) {
            return oob;
        }
        return b[pos[1]][pos[0]];
    }

    template<typename T>
    void print(board<T> const& b)
    {
        auto const edge{b.size()};
        for (auto y{0}; y!=edge; ++y) {
            fmt::print("{}  ", y);
            for (auto x{0}; x!=edge; ++x) {
                fmt::print("{} ", b[y][x]);
            }
            fmt::print("\n");
        }
    }

    auto calc_score(
            search_state const& state,
            coord const part_start,
            coord const part_end,
            coord const cross_direction,
            std::vector<char> const& word_part)
    -> std::optional<int>
    {
        Expects((std::abs(cross_direction[0])==1)
                !=(std::abs(cross_direction[1])==1));

        // word_part corresponds to the given span of cells.
        Expects((part_end[0]-part_start[0])+(part_end[1]-part_start[1])
                ==ssize(word_part));

        auto pre_word_part{0};
        auto word_start{part_start};
        while (get(state.board_tiles, word_start-cross_direction, vacant)
                !=vacant) {
            word_start -= cross_direction;
            ++pre_word_part;
        }

        auto post_word_part{0};
        auto word_end{part_end};
        while (get(state.board_tiles, word_end, vacant)!=vacant) {
            word_end += cross_direction;
            ++post_word_part;
        }
        
        auto const word_size{pre_word_part+ssize(word_part)+post_word_part};
        if (word_size==1) {
            return 0;
        }

        int word_multiplier{1};
        int score{0};
        auto* node{&state.lexicon.root_node()};
        for (auto i{0}; i!=word_size; ++i) {
            auto const pos{word_start+cross_direction*i};
            auto const tile{state.board_tiles[pos[1]][pos[0]]};
            auto const cell_premium{state.board_premiums[pos[1]][pos[0]]};

            auto[letter, letter_multiplier] = [&]() {
                if (tile!=vacant) {
                    return std::make_tuple(tile, 1);
                }

                // mutation!!
                word_multiplier *= gsl::at(::word_multiplier, int(cell_premium));

                Expects(i-pre_word_part>=0);
                Expects(i-pre_word_part<ssize(word_part));
                auto const letter{gsl::at(word_part, i-pre_word_part)};

                auto const letter_multiplier{
                        gsl::at(::letter_multiplier, int(cell_premium))};

                return std::make_tuple(letter, letter_multiplier);
            }();

            auto const found{std::find(begin(*node), end(*node), letter)};
            if (found==end(*node)) {
                return std::nullopt;
            }
            node = &found->get_next();

            auto const letter_score{state.letter_scores[letter]};
            score += letter_multiplier*letter_score;
        }
        if (!node->is_terminator) {
            return std::nullopt;
        }
        return word_multiplier*score;
    }

    void search(node const& n, search_state& state) noexcept
    {
        auto const neighbour_count{
                state.board_neighbours[state.pos[1]][state.pos[0]] ? 1 : 0};

        auto const recurse{
                [&](node const& n, int& counter, search_state& state) noexcept {
                    if (counter==0) {
                        return;
                    }

                    state.num_neighbours += neighbour_count;
                    state.pos += state.direction;
                    if (n.is_terminator
                            && state.num_neighbours>0
                            && state.rack_used>0
                            && get(state.board_tiles, state.pos, vacant)
                                    ==vacant) {
                        auto const word_score{calc_score(
                                state,
                                state.start,
                                state.pos,
                                state.direction,
                                state.word)};
                        Expects(word_score);

                        state.finds.emplace_back(result{
                                string{begin(state.word), end(state.word)},
                                state.cross_scores+*word_score,
                                state.start,
                                state.direction});
                    }

                    if (state.pos[0]<state.board_tiles.size()
                            && state.pos[1]<state.board_tiles.size()) {
                        --counter;
                        ::search(n, state);
                        ++counter;
                    }
                    state.pos -= state.direction;
                    state.num_neighbours -= neighbour_count;
                }};

        auto& board_tile{state.board_tiles[state.pos[1]][state.pos[0]]};
        if (board_tile==vacant) {
            std::for_each(begin(n), end(n), [&](auto const& letter) {
                coord const cross_direction{state.direction[1],
                        state.direction[0]};
                auto const cross_score{calc_score(
                        state,
                        state.pos,
                        state.pos+cross_direction,
                        cross_direction,
                        {letter})};

                // Cross-word is not a valid word in the lexicon.
                if (!cross_score) {
                    return;
                }

                state.word.push_back(letter);
                state.cross_scores += *cross_score;
                ++state.rack_used;

                recurse(letter.get_next(), state.rack[letter], state);
                recurse(letter.get_next(), state.rack[blank], state);

                --state.rack_used;
                state.cross_scores -= *cross_score;
                state.word.pop_back();
            });
            return;
        }

        auto const found{std::find_if(
                begin(n),
                end(n),
                [board_tile](auto const& c) {
                    return c==board_tile;
                })
        };
        if (found==end(n)) {
            return;
        }

        state.word.push_back(board_tile);
        auto dummy_counter{1};
        recurse(found->get_next(), dummy_counter, state);
        state.word.pop_back();
    }

    void search(trie const& lexicon, search_state& state, coord direction)
    {
        if (state.board_neighbours[state.start[1]][state.start[0]]) {
            auto const preceding{get(
                    state.board_tiles,
                    state.start-direction,
                    vacant)};
            if (preceding!=vacant) {
                // The start of a word cannot go on the board here
                // because there's a tile in the preceding space.
                return;
            }
        }

        state.direction = direction;
        search(lexicon.root_node(), state);
    }

    board<bool> make_board_neighbours(board<char> const& board_tiles)
    {
        auto const edge{board_tiles.size()};
        board<bool> board_neighbours(board_tiles.size());

        auto const populate_board_neighbours{
                [&](int offset_x, int offset_y, int begin_x, int begin_y,
                        int end_x, int end_y) {
                    for (auto x{begin_x}; x!=end_x; ++x) {
                        for (auto y{begin_y}; y!=end_y; ++y) {
                            Expects(y+offset_y>=0);
                            Expects(y+offset_y<edge);
                            Expects(x+offset_x>=0);
                            Expects(x+offset_x<edge);
                            if (board_tiles[y+offset_y][x+offset_x]!=vacant) {
                                board_neighbours[y][x] = true;
                            }
                        }
                    }
                }
        };

        populate_board_neighbours(-1, 0, 1, 0, edge, edge);
        populate_board_neighbours(1, 0, 0, 0, edge-1, edge);
        populate_board_neighbours(0, -1, 0, 1, edge, edge);
        populate_board_neighbours(0, 1, 0, 0, edge, edge-1);

        return board_neighbours;
    }

    void refine_results(std::vector<result>& finds)
    {
        // group by words, highest-scoring first
        sort(begin(finds), end(finds), [](auto a, auto b) {
            return tie(a.word, b.score)<tie(b.word, a.score);
        });

        // eliminate lower-scoring entries with the same word
        finds.erase(std::unique(begin(finds), end(finds), [](auto a, auto b) {
            return a.word==b.word;
        }), end(finds));

        sort(begin(finds), end(finds), [](auto a, auto b) {
            return tie(b.score, a.word)
                    <tie(a.score, b.word);
        });
    }

    auto solve(trie const& lexicon, letter_values const& letter_scores,
            std::string_view letters, board<char> const& board_tiles,
            board<premium> const& board_premiums)
    {
        search_state state{
                letter_scores,
                board_tiles,
                board_premiums,
                make_board_neighbours(board_tiles),
                lexicon,
                ssize(board_premiums)};

        std::fill(begin(state.rack), end(state.rack), 0);
        for (auto letter : letters) {
            ++state.rack[letter];
        }

        for (state.start[1] = 0;
                state.start[1]!=state.board_tiles.size();
                ++state.start[1]) {
            for (state.start[0] = 0;
                    state.start[0]!=state.board_tiles.size();
                    ++state.start[0]) {
                state.pos = state.start;
                //search(lexicon, state, {1, 0});
                search(lexicon, state, {0, 1});
            }
        }

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
}  // namespace

int main(int argc, char const* const* argv)
{
    using clara::Arg;
    using clara::Args;
    using clara::Help;
    using clara::Opt;

    auto help{false};
    auto min_length{2};
    auto premiums_filename{string{}};
    auto board_filename{string{}};
    auto letters{string{}};
    auto cli{
            Opt(min_length, "minimum length")["-n"]["--min-length"](
                    "minimum number of letters in words suggested")
                    | Arg(letters, "letters")(
                            "Letter \"rack\" including wildcards as '?'")
                    | Arg(board_filename, "board")(
                            "CSV file containing played letters")
                    | Arg(premiums_filename, "premiums")(
                            "CSV file describing premium tiles (see 'boards' directory)")
                    | Help(help)};
    auto result = cli.parse(Args(argc, argv));

    if (!result) {
        fmt::fprintf(stderr, "error: in command line: %s\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss scrabble board suggester\n"
                    "(C)2019 John McFarlane\n\n"
                    "play [options] <rack> <board>\n");
        for (auto const& help_column : cli.getHelpColumns()) {
            fmt::printf("{10}   {}\n", help_column.left, help_column.right);
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

    if (min_length>ssize(letters)) {
        fmt::fprintf(stderr,
                "error: too few letters, %zd, to achieve minimum word length, %d\n",
                letters.size(), min_length);
        return EXIT_FAILURE;
    }

    transform(begin(letters), end(letters), begin(letters), tolower);

    auto const board_tiles{load_board_tiles(board_filename)};
    if (!board_tiles) {
        fmt::print(stderr, "error: failed to load board files from {}\n",
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

    auto const lexicon{get_lexicon(min_length, ssize(*board_tiles))};

    auto finds{
            solve(lexicon, wwf_scores(), letters, *board_tiles,
                    *board_premiums)};

    for (auto const& find : finds) {
        fmt::print("{},{}:{}={}\n", find.start[0], find.start[1],
                find.word.c_str(),
                find.score);
    };
}  // namespace
