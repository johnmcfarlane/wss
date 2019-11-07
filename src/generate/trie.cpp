#include "trie.h"

#include <ssize.h>

#include <fmt/printf.h>
#include <gsl/gsl_assert>

#include <algorithm>
#include <map>

namespace {
    using std::begin;
    using std::end;
    using std::string_view;

    auto find(node& n, char l)
    {
        return std::find_if(begin(n.edges), end(n.edges),
                [l](auto const& edge) { return edge==l; });
    }

    auto insert(node& n, char l) -> node&
    {
        auto found = find(n, l);
        if (found!=end(n.edges)) {
            return found->get_next();
        }

        n.edges.emplace_back(l, std::make_unique<node>());
        return n.edges.back().get_next();
    }

    auto insert(
            node& n,
            string_view::const_iterator pos,
            string_view::const_iterator end)
    {
        Expects(pos<=end);

        if (pos==end) {
            if (n.is_terminator) {
                return false;
            }

            n.is_terminator = true;
            return true;
        }
        auto letter = *pos;
        auto& next_node = insert(n, letter);
        return insert(next_node, pos+1, end);
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
    Expects(n);
    next = move(n);
}

// trie

auto trie::root_node() const -> node const&
{
    return root;
}

void trie::insert(string_view word)
{
    ::insert(root, begin(word), end(word));
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
            Expects(edge.ptr());
            nodes.emplace_hint(found.first,
                    std::make_pair(edge.get_next(), edge.ptr()));
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
    ::compress(root, nodes);
}
