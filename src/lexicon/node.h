// Copyright 2019 John McFarlane
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WSS_NODE_H
#define WSS_NODE_H

#include <letter_set.h>
#include <wss_assert.h>

#include <cstring>
#include <iterator>

struct node
{
    letter_set letters;
    node const* edges;
};

class const_node_iterator {
public:
    using difference_type = int;
    using value_type = char const;
    using pointer = char const*;
    using reference = char const&;
    using iterator_category = std::forward_iterator_tag;

    const_node_iterator(const_node_iterator const&) = default;

    const_node_iterator(letter_set::const_iterator l, node const* n) noexcept
            : _letter(l), _node(n) {}

    char letter() const {
        return *_letter;
    }

    node const& child() const {
        return *_node;
    }

    friend auto operator*(const_node_iterator const& rhs) {
        WSS_ASSERT(rhs._node);
        return *rhs._letter;
    }
    friend auto& operator++(const_node_iterator& rhs) {
        WSS_ASSERT(rhs._node);
        ++ rhs._letter;
        ++ rhs._node;
        return rhs;
    }

    friend auto operator==(
            const_node_iterator const& lhs,
            const_node_iterator const& rhs) {
        return lhs._letter==rhs._letter;
    }
    friend auto operator!=(
            const_node_iterator const& lhs,
            const_node_iterator const& rhs) {
        return lhs._letter!=rhs._letter;
    }
private:
    letter_set::const_iterator _letter;
    node const* _node;
};

inline const_node_iterator begin(node const& n)
{
    return const_node_iterator{
            begin(n.letters),
            n.edges};
}

inline const_node_iterator end(node const& n)
{
    return const_node_iterator{
            end(n.letters),
            nullptr
    };
}

#endif // WSS_NODE_H
