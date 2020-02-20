#include "board.h"
#include "board_premiums.h"
#include "board_tiles.h"
#include "coord.h"

#include <scores.h>
#include <wwf_lexicon.h>

#include <fmt/printf.h>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>
#include <lyra/lyra.hpp>

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

    struct ray {
        coord start{};
        coord direction{};
    };

    struct initial_state {
        ray pos;
        coord cross_direction{};
        letter_values letter_scores;
        board<char> tiles;
        board<premium> premiums;
        board<bool> qualifying_cells;
        std::vector<char>::iterator word;
        node lexicon;
        int rack_size;
    };

    struct result {
        std::string word;
        int score;
        ray pos;
    };

    struct step_state {
        std::vector<char>::iterator word_end;
        std::vector<result>& finds;
        letter_values& rack;

        coord pos{};

        // letters from the rack on the board used in this word
        int rack_remaining{0};

        // letters already on the board touching this word
        int num_qualifying_cells{0};

        // accumulates cross scores until the end
        int cross_scores{};
    };

    struct search_state {
        initial_state const& init;
        step_state& step;
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
        return b.cell(pos);
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
            coord const& part_start,
            int const part_length,
            coord const& cross_direction)
    {
        Expects((std::abs(cross_direction[0])==1)  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
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
            initial_state const& init,
            coord const& part_start,
            coord const& direction,
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
        auto const* node{&init.lexicon};
        for (auto i{extents.first}; i!=extents.second; ++i) {
            auto const pos{part_start+direction*(i)};
            auto const tile{init.tiles.cell(pos)};
            auto const cell_premium{init.premiums.cell(pos)};

            auto[letter, letter_multiplier] = [&]() {
                if (tile!=vacant) {
                    return std::make_tuple(tile, 1);
                }

                // mutation!!
                word_multiplier *= gsl::at(word_multipliers,
                        int(cell_premium));

                Expects(i>=0);  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
                Expects(i<ssize(word_part));  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
                auto const letter{gsl::at(word_part, i)};

                auto const letter_multiplier{
                        gsl::at(letter_multipliers, int(cell_premium))};

                return std::make_tuple(letter, letter_multiplier);
            }();

            auto const found{
                    std::find(begin(*node), end(*node), std::toupper(letter))};
            if (found==end(*node)) {
                return std::nullopt;
            }
            node = &found.child();

            auto const letter_score{init.letter_scores[letter]};
            score += letter_multiplier*letter_score;
        }
        if (!node->is_terminator) {
            return std::nullopt;
        }
        return word_multiplier*score;
    }

    void search(node const& n, search_state state);

    void recurse(node const& n, char letter, int qualifying_cells_count,
            search_state state)
    {
        *state.step.word_end++ = letter;
        state.step.num_qualifying_cells += qualifying_cells_count;
        state.step.pos += state.init.pos.direction;
        if (n.is_terminator
                && state.step.num_qualifying_cells>0
                && state.step.rack_remaining<state.init.rack_size
                && get(state.init.tiles, state.step.pos, vacant)
                        ==vacant) {
            auto const extents{word_extent(
                    state.init.tiles,
                    state.init.pos.start,
                    std::distance(
                            state.init.word, state.step.word_end),
                    state.init.pos.direction)};
            auto const word_score{calc_score(
                    state.init,
                    state.init.pos.start,
                    state.init.pos.direction,
                    extents,
                    gsl::span<char>{&*state.init.word, &*state.step.word_end})};
            Expects(word_score);  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL

            auto play_score{state.step.cross_scores+*word_score};
            if (state.step.rack_remaining==0) {
                play_score += state.init.letter_scores[score_index_empty_rack];
            }

            state.step.finds.emplace_back(result{
                    std::string{state.init.word, state.step.word_end},
                    play_score,
                    state.init.pos});
        }

        if (state.step.pos[0]<state.init.tiles.size()
                && state.step.pos[1]<state.init.tiles.size()) {
            ::search(n, state);
        }
        state.step.pos -= state.init.pos.direction;
        state.step.num_qualifying_cells -= qualifying_cells_count;
        --state.step.word_end;
    }

    auto fill_square(search_state state, node const& edge,
            std::pair<int, int> extents, int qualifying_cells_count,
            int& counter,
            char letter)
    {
        if (counter==0) {
            return;
        }

        auto const cross_score{calc_score(
                state.init,
                state.step.pos,
                state.init.cross_direction,
                extents,
                gsl::span<char>(&letter, 1))};

        // Cross-word is not a valid word in the lexicon.
        if (!cross_score) {
            return;
        }

        --counter;
        state.step.cross_scores += *cross_score;
        recurse(edge, letter, qualifying_cells_count, state);
        // cppcheck-suppress unreadVariable
        state.step.cross_scores -= *cross_score;
        ++counter;
    }

    void search(node const& n, search_state state)
    {
        Expects(state.step.rack_remaining>=0);  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL

        auto const qualifying_cells_count{
                state.init.qualifying_cells.cell(state.step.pos) ? 1 : 0};

        auto const board_tile{state.init.tiles.cell(state.step.pos)};
        if (board_tile==vacant) {
            auto i{begin(n)};
            auto const n_end{end(n)};
            if (i==n_end) {
                return;
            }

            --state.step.rack_remaining;
            auto& blank_count{state.step.rack[blank]};
            auto& wildcard_count{state.step.rack[wildcard]};

            auto const extents{word_extent(
                    state.init.tiles,
                    state.step.pos,
                    1,
                    state.init.cross_direction)};

            do {
                auto const letter{i.letter()};
                auto& letter_count{state.step.rack[letter]};
                auto const& edge(i.child());
                ++i;

                if (blank_count==0 && wildcard_count==0 && letter_count==0) {
                    continue;
                }

                fill_square(state, edge, extents, qualifying_cells_count, letter_count,
                        letter);
                fill_square(state, edge, extents, qualifying_cells_count, blank_count,
                        char(std::tolower(letter)));
                fill_square(state, edge, extents, qualifying_cells_count,
                        wildcard_count, letter);
            }
            while (i!=n_end);

            ++state.step.rack_remaining;
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

        recurse(found.child(), board_tile, qualifying_cells_count, state);
    }

    void search(search_state state)
    {
        if (state.init.qualifying_cells.cell(state.init.pos.start)) {
            auto const preceding{get(
                    state.init.tiles,
                    state.init.pos.start-state.init.pos.direction,
                    vacant)};
            if (preceding!=vacant) {
                // The start of a word cannot go on the board here
                // because there's a tile in the preceding space.
                return;
            }
        }

        search(state.init.lexicon, state);
    }

    auto populate_qualifying_cells(board<bool>& qualifying_cells,
            board<char> const& board_tiles, coord const& offset,
            coord const& first, coord const& last)
    {
        coord pos;
        for (pos[1] = first[1]; pos[1]!=last[1]; ++pos[1]) {
            for (pos[0] = first[0]; pos[0]!=last[0]; ++pos[0]) {
                if (board_tiles.cell(pos+offset)!=vacant) {
                    qualifying_cells.cell(pos) = true;
                }
            }
        }
    }

    auto make_qualifying_cells(board<char> const& board_tiles) -> board<bool>
    {
        auto const edge{ssize(board_tiles)};
        board<bool> qualifying_cells(edge);

        if (edge>0) {  // LCOV_EXCL_LINE - TODO: unit tests or fix GSL
            populate_qualifying_cells(qualifying_cells, board_tiles, {-1, 0}, {1, 0}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {1, 0}, {0, 0}, {edge-1, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, -1}, {0, 1}, {edge, edge});
            populate_qualifying_cells(qualifying_cells, board_tiles, {0, 1}, {0, 0}, {edge, edge-1});
        }

        qualifying_cells.cell({edge/2, edge/2}) = true;

        return qualifying_cells;
    }

    void refine_results(std::vector<result>& finds)
    {
        sort(begin(finds), end(finds), [](auto const& a, auto const& b) {
            return tie(b.score, a.word, a.pos.start[1], a.pos.start[0],
                    a.pos.direction[1], a.pos.direction[0])
                    <tie(a.score, b.word, b.pos.start[1], b.pos.start[0],
                            b.pos.direction[1], b.pos.direction[0]);
        });

        auto const last_unique = unique(begin(finds), end(finds),
                [](auto const& a, auto const& b) {
                    return tie(a.word, a.score, a.pos.start, a.pos.direction)
                            ==tie(b.word, b.score, b.pos.start, b.pos.direction);
                });
        finds.erase(last_unique, end(finds));
    }

    auto make_start_state(node const& lexicon, letter_values const& letter_scores,
            board<char> tiles, board<premium> premiums,
            std::vector<char>& word, std::string_view letters)
    {
        auto qualifying_cells{make_qualifying_cells(tiles)};
        return initial_state {
                {},
                {},
                letter_scores,
                std::move(tiles),
                std::move(premiums),
                std::move(qualifying_cells),
                std::begin(word),
                lexicon,
                ssize(letters)
        };
    }

    auto solve(node const& lexicon, letter_values const& letter_scores,
            std::string_view letters, board<char> tiles,
            board<premium> premiums)
    {
        auto const edge{ssize(tiles)};
        std::vector<char> word(edge+1);
        std::vector<result> finds;
        letter_values rack{};
        
        auto init{make_start_state(
                lexicon, letter_scores, std::move(tiles),
                std::move(premiums), word, letters)};
        step_state step{
                std::begin(word),
                finds,
                rack
        };
        search_state const state{
                init,
                step
        };

        state.step.rack_remaining = state.init.rack_size;

        for (auto letter : letters) {
            ++step.rack[letter];
        }

        for (auto bearing{0}; bearing!=2; ++bearing) {
            init.pos.direction[0] = init.cross_direction[1] = 1-bearing;
            init.pos.direction[1] = init.cross_direction[0] = bearing;
            for (init.pos.start[init.pos.direction[0]] = edge-1;
                    init.pos.start[init.pos.direction[0]]>=0;
                    --init.pos.start[init.pos.direction[0]]) {
                auto count_back{0};
                for (init.pos.start[init.pos.direction[1]] = edge-1;
                        init.pos.start[init.pos.direction[1]]>=0;
                        --init.pos.start[init.pos.direction[1]]) {
                    step.pos = init.pos.start;
                    if (init.qualifying_cells.cell(init.pos.start)) {
                        count_back = ssize(letters);
                    }
                    else {
                        --count_back;
                    }
                    if (count_back>0) {
                        search(state);
                    }
                }
            }
        }

        refine_results(step.finds);
        return move(step.finds);
    }

}  // namespace

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
}  // namespace
