#include "trie.h"

#include "ssize.h"

#include <fmt/printf.h>
#include <gsl/gsl_assert>

#include <algorithm>
#include <array>
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

    auto& insert(node& n, char l)
    {
        auto found = find(n, l);
        if (found!=end(n.edges)) {
            return found->get_next();
        }

        edge e{l, std::make_unique<node>()};
        n.edges.emplace_back(std::move(e));
        return n.edges.back().get_next();
    }

    bool insert(node& n, string_view word, string_view ending)
    {
        Expects(word.size()>=ending.size());
        Expects(&word.back()==&ending.back());

        if (ending.empty()) {
            if (n.is_terminator) {
                return false;
            }

            Expects(!n.is_terminator);
            n.is_terminator = true;
            return true;
        }
        auto letter = ending.front();
        auto& next_node = insert(n, letter);
        return insert(next_node, word, ending.substr(1));
    }
}  // namespace

// node

node::edge_vector::const_iterator node::begin() const { return edges.begin(); }

node::edge_vector::const_iterator node::end() const { return edges.end(); }

// edge

edge::edge(char l, std::unique_ptr<node> n)
        :letter(l), next(std::move(n)) { }

edge::operator char() const { return letter; }

std::shared_ptr<node> const& edge::ptr() const { return next; }

node& edge::get_next() { return *next; }

node const& edge::get_next() const { return *next; }

void edge::set_next(std::shared_ptr<node> n) { 
    Expects(n);
    next = move(n); 
}

// trie

trie::trie() = default;

trie::~trie() = default;

trie::size_type trie::size() const
{
    return count;
}

node const& trie::root_node() const
{
    return root;
}

void trie::insert(string_view word)
{
    count += ::insert(root, word, word) ? 1 : 0;
}

bool operator<(edge const& lhs, edge const& rhs)
{
    return std::tie(static_cast<char const&>(lhs), lhs.ptr())
            <std::tie(static_cast<char const&>(rhs), rhs.ptr());
}

bool operator<(node const& lhs, node const& rhs)
{
    return std::tie(lhs.edges, lhs.is_terminator)
            <std::tie(rhs.edges, rhs.is_terminator);
}

using node_map = std::map<node, std::shared_ptr<node>>;

int compress(node& n, node_map& nodes)
{
    auto sum{0};
    for (auto& edge : n.edges) {
        auto& child{edge.get_next()};
        sum+=compress(child, nodes);

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
