// Copyright 2018 John McFarlane
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

#include "multi_trie.h"

#include <ssize.h>
#include <wss_assert.h>

#include <algorithm>
#include <fmt/printf.h>
#include <map>

namespace {
    using std::begin;
    using std::end;
    using std::string_view;

    auto insert(int index, node::edge_vector& e, char l) -> node&
    {
        auto const [found_first, found_last] = std::equal_range(begin(e), end(e), l);

        auto const num_found = std::distance(found_first, found_last);
        if (num_found != 0) {
            WSS_ASSERT(num_found == 1);
            return found_first->get_next();
        }

        return e.emplace(found_first, 
                l,
                std::make_unique<node>(node::edge_vector{}, index, false))->get_next();
    }

    auto insert(
            int index,
            node& n,
            string_view::const_iterator
            pos,
            string_view::const_iterator end)
    {
        WSS_ASSERT(pos<=end);

        if (pos==end) {
            if (n.is_terminator) {
                return false;
            }

            n.is_terminator = true;
            return true;
        }
        auto letter = *pos;
        auto& next_node = insert(index, n.edges, letter);
        return insert(index, next_node, pos+1, end);
    }
}  // namespace

// node

auto node::begin() const
-> node::edge_vector::const_iterator { return edges.begin(); }

auto node::end() const
-> node::edge_vector::const_iterator { return edges.end(); }

// edge

edge::edge(char l, std::unique_ptr<node> n)
        :letter(l), next(std::move(n)) { }

edge::operator char() const { return letter; }

auto edge::ptr() const -> std::shared_ptr<node> const& { return next; }

auto edge::get_next() -> node& { return *next; }

auto edge::get_next() const -> node const& { return *next; }

void edge::set_next(std::shared_ptr<node> n)
{
    WSS_ASSERT(n);
    next = move(n);
}

// multi_trie

multi_trie::multi_trie(int num_root_nodes)
{
    for (auto i = 0; i!=num_root_nodes; ++i) {
        roots.push_back({{}, i, true});
    }
    WSS_ASSERT(num_root_nodes>=0);
}

auto multi_trie::root_nodes() const -> std::vector<node> const&
{
    return roots;
}

void multi_trie::insert(string_view word, int index)
{
    ::insert(index, roots[index], begin(word), end(word));
}

using node_map = std::map<node, std::shared_ptr<node>>;

auto compress(node& n, node_map& nodes) -> int
{
    auto sum{0};
    for (auto& edge : n.edges) {
        auto& child{edge.get_next()};
        sum += compress(child, nodes);

        auto found{nodes.equal_range(child)};
        if (found.first==found.second) {
            WSS_ASSERT(edge.ptr());
            nodes.emplace_hint(found.first,
                    std::pair{edge.get_next(), edge.ptr()});
            continue;
        }

        edge.set_next(found.first->second);
        ++sum;
    }
    return sum;
}

void multi_trie::compress()
{
    node_map nodes;
    for (auto& root_node : roots) {
        ::compress(root_node, nodes);
    }
}
