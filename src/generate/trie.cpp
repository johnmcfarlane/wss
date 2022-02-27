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

#include "trie.h"

#include <wss_assert.h>

#include <algorithm>
#include <iterator>
#include <map>
#include <utility>

namespace {
    using std::begin;
    using std::end;
    using std::string_view;

    auto insert(gen_node::edge_vector& e, char l) -> gen_node&
    {
        auto const [found_first, found_last] = std::equal_range(begin(e), end(e), l);

        auto const num_found = std::distance(found_first, found_last);
        if (num_found != 0) {
            WSS_ASSERT(num_found == 1);
            return found_first->get_next();
        }

        return e.emplace(found_first, l, std::make_unique<gen_node>())->get_next();
    }

    auto insert(
            gen_node& n,
            string_view::const_iterator
                    pos,
            string_view::const_iterator end)
    {
        WSS_ASSERT(pos <= end);

        if (pos == end) {
            if (n.is_terminator) {
                return false;
            }

            n.is_terminator = true;
            return true;
        }
        auto letter = *pos;
        auto& next_node = insert(n.edges, letter);
        return insert(next_node, pos + 1, end);
    }
}  // namespace

// gen_node

auto begin(gen_node const& n)
        -> gen_node::edge_vector::const_iterator
{
    return std::begin(n.edges);
}

auto end(gen_node const& n)
        -> gen_node::edge_vector::const_iterator
{
    return std::end(n.edges);
}

// edge

edge::edge(char l, std::unique_ptr<gen_node> n)
    : _letter(l)
    , _next(std::move(n))
{
}

edge::operator char() const
{
    return _letter;
}

auto edge::ptr() const -> std::shared_ptr<gen_node> const&
{
    return _next;
}

auto edge::get_next() -> gen_node&
{
    return *_next;
}

auto edge::get_next() const -> gen_node const&
{
    return *_next;
}

void edge::set_next(std::shared_ptr<gen_node> n)
{
    WSS_ASSERT(n);
    _next = std::move(n);
}

// trie

trie::trie()
    : _root{{}, true}
{
}

auto trie::root_node() const -> gen_node const&
{
    return _root;
}

void trie::insert(string_view word)
{
    ::insert(_root, begin(word), end(word));
}

using node_map = std::map<gen_node, std::shared_ptr<gen_node>>;

auto compress(gen_node& n, node_map& nodes) -> int
{
    auto sum{0};
    for (auto& edge : n.edges) {
        auto& child{edge.get_next()};
        sum += compress(child, nodes);

        auto found{nodes.equal_range(child)};
        if (found.first == found.second) {
            WSS_ASSERT(edge.ptr());
            nodes.emplace_hint(found.first, std::pair{edge.get_next(), edge.ptr()});
            continue;
        }

        edge.set_next(found.first->second);
        ++sum;
    }
    return sum;
}

void trie::compress()
{
    node_map nodes;
    ::compress(_root, nodes);
}
