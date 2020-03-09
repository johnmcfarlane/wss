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

#include <cstring>
#include <iterator>

struct node
{
    char const* const letters;
    node const* edges;
    bool is_terminator;
};

class const_node_iterator {
public:
    using difference_type = int;
    using value_type = char const;
    using pointer = char const*;
    using reference = char const&;
    using iterator_category = std::forward_iterator_tag;

    const_node_iterator(const_node_iterator const&) = default;

    const_node_iterator(char const* l, node const* n)
            : edge_letter(l), edge_node(n) {}

    char letter() const {
        return *edge_letter;
    }

    node const& child() const {
        return *edge_node;
    }

    friend auto& operator*(const_node_iterator& rhs) {
        return *rhs.edge_letter;
    }
    friend auto& operator++(const_node_iterator& rhs) {
        ++rhs.edge_letter;
        ++rhs.edge_node;
        return rhs;
    }

    friend auto operator==(
            const_node_iterator const& lhs,
            const_node_iterator const& rhs) {
        return lhs.edge_letter==rhs.edge_letter;
    }
    friend auto operator!=(
            const_node_iterator const& lhs,
            const_node_iterator const& rhs) {
        return lhs.edge_letter!=rhs.edge_letter;
    }
private:
    char const* edge_letter{};
    node const* edge_node{};
};

inline const_node_iterator begin(node const& n)
{
    return const_node_iterator(n.letters, n.edges);
}

inline const_node_iterator end(node const& n)
{
    auto const l{std::strlen(n.letters)};
    return const_node_iterator(n.letters+l, n.edges+l);
}

#endif // WSS_NODE_H
