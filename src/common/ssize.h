//
// Created by john on 27/10/2019.
//

#ifndef WSS_SSIZE_H
#define WSS_SSIZE_H

#include <cstddef>

template<typename Container>
inline auto ssize(Container&& c)
{
    return int(c.size());
}

#endif //WSS_SSIZE_H
