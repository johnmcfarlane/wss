#ifndef WSS_RESULT_H
#define WSS_RESULT_H

#include "coord.h"

#include <tuple>

struct ray {
    coord start{};
    coord direction{};
};

struct result {
    std::string word;
    int score;
    ray pos;
};

inline auto operator==(result const& a, result const& b)
{
    return tie(a.score, a.pos.start[0], a.pos.start[1],
               a.pos.direction[0], a.pos.direction[1], a.word)
        == tie(b.score, b.pos.start[0], b.pos.start[1],
               b.pos.direction[0], b.pos.direction[1], b.word);
}

inline auto operator<(result const& a, result const& b)
{
    return tie(b.score, a.word, a.pos.start[1], a.pos.start[0],
               a.pos.direction[1], a.pos.direction[0])
         < tie(a.score, b.word, b.pos.start[1], b.pos.start[0],  // NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
               b.pos.direction[1], b.pos.direction[0]);
}

#endif  // WSS_RESULT_H
