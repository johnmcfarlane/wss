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

#ifndef WSS_MULTI_TRIE_H
#define WSS_MULTI_TRIE_H

#include <memory>
#include <string_view>
#include <vector>

class edge;

struct node {
    using edge_vector = std::vector<edge>;

    node(edge_vector e, int i, bool t)
            :edges(std::move(e)), root_index(i), is_terminator(t) { }

    auto begin() const -> edge_vector::const_iterator;

    edge_vector::const_iterator end() const;

    edge_vector edges;
    int root_index;
    bool is_terminator{false};
};

class edge {
public:
    edge(char l, std::unique_ptr<node> n);

    operator char() const;

    std::shared_ptr<node> const& ptr() const;

    node& get_next();
    node const& get_next() const;

    void set_next(std::shared_ptr<node> n);

private:
    char letter;
    std::shared_ptr<node> next;
};

class multi_trie {
    using string_view = std::string_view;

public:
    using size_type = std::ptrdiff_t;

    explicit multi_trie(int num_root_nodes);

    std::vector<node> const& root_nodes() const;

    void insert(string_view word, int index);

    void compress();

private:

    std::vector<node> roots;
};

inline bool operator<(edge const& lhs, edge const& rhs)
{
    return std::tie(static_cast<char const&>(lhs), lhs.ptr())
            <std::tie(static_cast<char const&>(rhs), rhs.ptr());
}

inline bool operator<(node const& lhs, node const& rhs)
{
    return std::tie(lhs.edges, lhs.is_terminator)
            <std::tie(rhs.edges, rhs.is_terminator);
}

#endif //WSS_MULTI_TRIE_H
