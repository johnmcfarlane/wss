#ifndef WSS_RESULT_H
#define WSS_RESULT_H

#include "coord.h"

struct ray {
    coord start{};
    coord direction{};
};

struct result {
    std::string word;
    int score;
    ray pos;
};

#endif //WSS_RESULT_H
