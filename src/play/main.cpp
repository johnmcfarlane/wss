#include "board.h"
#include "board_premiums.h"
#include "board_tiles.h"
#include "coord.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <clara.hpp>
#include <fmt/printf.h>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>

#include <algorithm>
#include <cassert>
#include <cctype>
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

    struct board_state {
        letter_values letter_scores;
        board<char> tiles;
        board<premium> premiums;
        board<bool> neighbours;
        node lexicon;
        int edge;
    };

    struct move_start {
        coord start{};
        coord direction{};
    };

    struct result {
        std::string word;
        int score;
        move_start move;
    };

    struct search_state {
        board_state const board;
        move_start move;

        // letters from the rack on the board used in this word
        int rack_used{0};

        // letters already on the board touching this word
        int num_neighbours{0};

        letter_values rack{};
        coord pos{};

        // accumulates cross scores until the end
        int cross_scores{};

        std::vector<char> word{};
        std::vector<result> finds{};
    };

    template<typename T>
    auto get(board<T> const& b, coord pos, T oob) -> T
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

    auto word_extent(
            board<char> const& board_tiles,
            coord const part_start,
            int const part_length,
            coord const cross_direction)
    {
        Expects((std::abs(cross_direction[0])==1)
                !=(std::abs(cross_direction[1])==1));

        auto pre_word_part{-1};
        while (get(
                board_tiles,
                part_start+cross_direction*pre_word_part,
                vacant)!=vacant) {
            --pre_word_part;
        }
        ++pre_word_part;

        auto post_word_part{part_length};
        while (get(
                board_tiles,
                part_start+cross_direction*post_word_part,
                vacant)!=vacant) {
            ++post_word_part;
        }

        return std::make_pair(pre_word_part, post_word_part);
    }

    auto calc_score(
            search_state const& state,
            coord const part_start,
            coord const cross_direction,
            std::pair<int, int> extents,
            gsl::span<char> word_part)
    -> std::optional<int>
    {
        auto const word_size{extents.second-extents.first};
        if (word_size==1) {
            return 0;
        }

        int word_multiplier{1};
        int score{0};
        auto* node{&state.board.lexicon};
        for (auto i{extents.first}; i!=extents.second; ++i) {
            auto const pos{part_start+cross_direction*(i)};
            auto const tile{state.board.tiles[pos[1]][pos[0]]};
            auto const cell_premium{state.board.premiums[pos[1]][pos[0]]};

            auto[letter, letter_multiplier] = [&]() {
                if (tile!=vacant) {
                    return std::make_tuple(tile, 1);
                }

                // mutation!!
                word_multiplier *= gsl::at(::word_multiplier,
                        int(cell_premium));

                Expects(i>=0);
                Expects(i<ssize(word_part));
                auto const letter{gsl::at(word_part, i)};

                auto const letter_multiplier{
                        gsl::at(::letter_multiplier, int(cell_premium))};

                return std::make_tuple(letter, letter_multiplier);
            }();

            auto const found{
                    std::find(begin(*node), end(*node), std::toupper(letter))};
            if (found==end(*node)) {
                return std::nullopt;
            }
            node = &found.child();

            auto const letter_score{state.board.letter_scores[letter]};
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
                state.board.neighbours[state.pos[1]][state.pos[0]] ? 1 : 0};

        auto const recurse{
                [&](node const& n, char letter,
                        search_state& state) noexcept {
                    state.word.push_back(letter);
                    state.num_neighbours += neighbour_count;
                    state.pos += state.move.direction;
                    if (n.is_terminator
                            && state.num_neighbours>0
                            && state.rack_used>0
                            && get(state.board.tiles, state.pos, vacant)
                                    ==vacant) {
                        auto const extents{word_extent(
                                state.board.tiles,
                                state.move.start,
                                ssize(state.word),
                                state.move.direction)};
                        auto const word_score{calc_score(
                                state,
                                state.move.start,
                                state.move.direction,
                                extents,
                                state.word)};
                        Expects(word_score);

                        state.finds.emplace_back(result{
                                string{begin(state.word), end(state.word)},
                                state.cross_scores+*word_score,
                                state.move});
                    }

                    if (state.pos[0]<state.board.edge
                            && state.pos[1]<state.board.edge) {
                        ::search(n, state);
                    }
                    state.pos -= state.move.direction;
                    state.num_neighbours -= neighbour_count;
                    state.word.pop_back();
                }};

        auto& board_tile{state.board.tiles[state.pos[1]][state.pos[0]]};
        if (board_tile==vacant) {
            ++state.rack_used;

            auto const n_end(end(n));
            for (auto i{begin(n)}; i!=n_end; ++i) {
                auto const& edge(i.child());
                auto const letter{i.letter()};
                coord const cross_direction{state.move.direction[1],
                        state.move.direction[0]};
                auto const extents{word_extent(
                        state.board.tiles,
                        state.pos,
                        1,
                        cross_direction)};

                auto const fill_square{[&](int& counter, char letter) {
                    if (counter==0) {
                        return;
                    }

                    auto const cross_score{calc_score(
                            state,
                            state.pos,
                            cross_direction,  // NOLINTNEXTLINE(clang-analyzer-core.NonNullParamChecker)
                            extents,
                            gsl::span<char>(&letter, 1))};

                    // Cross-word is not a valid word in the lexicon.
                    if (!cross_score) {
                        return;
                    }

                    --counter;
                    state.cross_scores += *cross_score;
                    recurse(edge, letter, state);
                    state.cross_scores -= *cross_score;
                    ++counter;
                }};

                fill_square(state.rack[letter], letter);
                fill_square(state.rack[blank], char(std::tolower(letter)));
                fill_square(state.rack[wildcard], letter);
            }

            --state.rack_used;
            return;
        }

        auto const found{std::find_if(
                begin(n),
                end(n),
                [board_tile](auto const& c) {
                    return c==std::toupper(board_tile);
                })
        };
        if (found==end(n)) {
            return;
        }

        recurse(found.child(), board_tile, state);
    }

    void search(node const& lexicon, search_state& state, coord direction)
    {
        if (state.board.neighbours[state.move.start[1]][state.move.start[0]]) {
            auto const preceding{get(
                    state.board.tiles,
                    state.move.start-direction,
                    vacant)};
            if (preceding!=vacant) {
                // The start of a word cannot go on the board here
                // because there's a tile in the preceding space.
                return;
            }
        }

        state.move.direction = direction;
        search(lexicon, state);
    }

    auto make_board_neighbours(board<char> const& board_tiles) -> board<bool>
    {
        auto const edge{board_tiles.size()};
        board<bool> board_neighbours(board_tiles.size());

        auto const populate_board_neighbours{
                [&](coord offset, coord first, coord last) {
                    for (auto y{first[1]}; y!=last[1]; ++y) {
                        Expects(y+offset[1]>=0);
                        Expects(y+offset[1]<edge);

                        for (auto x{first[0]}; x!=last[0]; ++x) {
                            Expects(x+offset[0]>=0);
                            Expects(x+offset[0]<edge);

                            // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage)
                            if (board_tiles[y+offset[1]][x+offset[0]]!=vacant) {
                                board_neighbours[y][x] = true;
                            }
                        }
                    }
                }
        };

        if (edge>0) {
            populate_board_neighbours({-1, 0}, {1, 0}, {edge, edge});
            populate_board_neighbours({1, 0}, {0, 0}, {edge-1, edge});
            populate_board_neighbours({0, -1}, {0, 1}, {edge, edge});
            populate_board_neighbours({0, 1}, {0, 0}, {edge, edge-1});
        }

        return board_neighbours;
    }

    void refine_results(std::vector<result>& finds)
    {
        sort(begin(finds), end(finds), [](auto a, auto b) {
            return tie(b.score, a.word, a.move.start[1], a.move.start[0],
                    a.move.direction[1], a.move.direction[0])
                    <tie(a.score, b.word, b.move.start[1], b.move.start[0],
                            b.move.direction[1], b.move.direction[0]);
        });
    }

    auto solve(node const& lexicon, letter_values const& letter_scores,
            std::string_view letters, board<char> tiles,
            board<premium> premiums)
    {
        auto neighbours{make_board_neighbours(tiles)};
        auto const edge{ssize(tiles)};
        search_state state{
                board_state{
                        letter_scores,
                        std::move(tiles),
                        std::move(premiums),
                        std::move(neighbours),
                        lexicon,
                        edge
                },
                {}
        };

        std::fill(begin(state.rack), end(state.rack), 0);
        for (auto letter : letters) {
            ++state.rack[letter];
        }

        for (auto bearing{0}; bearing!=2; ++bearing) {
            coord const direction{1-bearing, bearing};
            for (state.move.start[direction[0]] = edge-1;
                    state.move.start[direction[0]]>=0;
                    --state.move.start[direction[0]]) {
                auto count_back{0};
                for (state.move.start[direction[1]] = edge-1;
                        state.move.start[direction[1]]>=0;
                        --state.move.start[direction[1]]) {
                    state.pos = state.move.start;
                    if (state.board.neighbours
                                [state.move.start[1]][state.move.start[0]]) {
                        count_back = ssize(letters);
                    }
                    else {
                        --count_back;
                    }
                    if (count_back>0) {
                        search(lexicon, state, direction);
                    }
                }
            }
        }

        refine_results(state.finds);
        return move(state.finds);
    }

}  // namespace

auto main(int argc, char const* const* argv) -> int
{
    using clara::Arg;
    using clara::Args;
    using clara::Help;
    using clara::Opt;

    auto help{false};
    auto min_length{2};
    auto lexicon_filename{string{}};
    auto board_filename{string{}};
    auto premiums_filename{string{}};
    auto letters{string{}};
    auto cli{
            Opt(min_length, "minimum length")["-n"]["--min-length"](
                    "minimum number of letters in words suggested")
                    | Arg(letters, "letters")(
                            "Letter \"rack\" including wildcards as ? and blanks as _")
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
                fmt::format("{:2},{:1}", find.move.start[0]+1,
                        find.move.start[1]+1),
                (find.move.direction[0]!=0) ? '-' : '|');
    };
}  // namespace
