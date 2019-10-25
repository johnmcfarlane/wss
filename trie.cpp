#include "trie.h"

#include <algorithm>
#include <cassert>

namespace {
    using std::begin;
    using std::end;
    using std::experimental::string_view;
    using std::find_if;
    using std::make_unique;
    using std::sort;

    auto find(node& n, char l)
    {
        return find_if(begin(n.edges), end(n.edges),
                [l](auto const& edge) { return edge==l; });
    }

    auto& insert(node& n, char l)
    {
        auto found = find(n, l);
        if (found!=end(n.edges)) {
            return found->get_next();
        }

        edge e{l, make_unique<node>()};
        n.edges.emplace_back(std::move(e));
        return n.edges.back().get_next();
    }

    bool insert(node& n, string_view word, string_view ending)
    {
        assert(word.size()>=ending.size());
        assert(&word.back()==&ending.back());

        if (ending.empty()) {
            if (n.is_terminator) {
                return false;
            }

            assert(!n.is_terminator);
            n.is_terminator = true;
            return true;
        }
        auto letter = ending.front();
        auto& next_node = insert(n, letter);
        return insert(next_node, word, ending.substr(1));
    }
}

// node

node::node() = default;

node::~node() = default;

node::edge_vector::const_iterator node::begin() const { return edges.begin(); }

node::edge_vector::const_iterator node::end() const { return edges.end(); }

// edge

edge::edge(char l, std::unique_ptr<node> n)
        :letter(l), next(std::move(n)) { }

edge::operator char() const { return letter; }

node& edge::get_next() { return *next; }

node const& edge::get_next() const { return *next; }

// trie

trie::trie() = default;

trie::~trie() = default;

trie::size_type trie::size() const
{
    return count;
}

trie::edge_const_iterator trie::begin() const
{
    return std::begin(root.edges);
}

trie::edge_const_iterator trie::end() const
{
    return std::end(root.edges);
}

void trie::insert(string_view word)
{
    count += ::insert(root, word, word);
}
