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

    edge_vector edges;
    bool is_terminator{false};
};

auto begin(node const& n) -> node::edge_vector::const_iterator;
auto end(node const& n) -> node::edge_vector::const_iterator;

class edge {
public:
    edge(char l, std::unique_ptr<node> n);

    operator char() const;  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    [[nodiscard]] auto ptr() const -> std::shared_ptr<node> const&;

    auto get_next() -> node&;
    [[nodiscard]] auto get_next() const -> node const&;

    void set_next(std::shared_ptr<node> n);

private:
    char _letter;
    std::shared_ptr<node> _next;
};

class trie {
    using string_view = std::string_view;

public:
    using size_type = std::ptrdiff_t;

    explicit trie();

    [[nodiscard]] auto root_node() const -> node const&;

    void insert(string_view word);

    void compress();

private:
    node _root;
};

inline auto operator<(edge const& lhs, edge const& rhs) -> bool
{
    return std::tie(static_cast<char const&>(lhs), lhs.ptr())
         < std::tie(static_cast<char const&>(rhs), rhs.ptr());
}

inline auto operator<(node const& lhs, node const& rhs) -> bool
{
    return std::tie(lhs.edges, lhs.is_terminator)
         < std::tie(rhs.edges, rhs.is_terminator);
}

#endif  // WSS_MULTI_TRIE_H
