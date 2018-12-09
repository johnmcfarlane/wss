#include <algorithm>
#include <cassert>

#include "trie.h"

namespace {
    using std::begin;
    using std::end;
    using std::experimental::string_view;
    using std::find_if;
    using std::make_unique;
    using std::sort;
}

namespace {
    auto find(trie::node& n, char l)
    {
        return find_if(begin(n.edges), end(n.edges), [l](auto const& edge) { return edge.letter==l; });
    }

    auto find(trie::node const& n, char l)
    {
        return find_if(begin(n.edges), end(n.edges), [l](auto const& edge) { return edge.letter==l; });
    }

    bool terminal(trie::node& n)
    {
        return n.word!=nullptr;
    }

    void terminate(trie::node& n, string_view word)
    {
        assert(!terminal(n));
        n.word = word.data();
    }

    auto& insert(trie::node& n, char l)
    {
        auto found = find(n, l);
        if (found!=end(n.edges)) {
            return *found->next;
        }

        trie::edge e{l, make_unique<trie::node>()};
        n.edges.emplace_back(std::move(e));
        return *n.edges.back().next;
    }

    string_view find(trie::node const& n, string_view ending)
    {
        if (ending.empty()) {
            return n.word;
        }

        auto found = find(n, ending.front());
        if (found==end(n.edges)) {
            return nullptr;
        }

        return find(*found->next, ending.substr(1));
    }

    bool insert(trie::node& n, string_view word, string_view ending)
    {
        assert(word.size()>=ending.size());
        assert(&word.back()==&ending.back());

        if (ending.empty()) {
            if (terminal(n)) {
                return false;
            }

            terminate(n, word);
            return true;
        }
        auto letter = ending.front();
        auto& next_node = insert(n, letter);
        return insert(next_node, word, ending.substr(1));
    }

    void sort(trie::node& n)
    {
        sort(begin(n.edges), end(n.edges), [](auto const& a, auto const& b) {
            return a.letter<b.letter;
        });
        for (auto& edge : n.edges) {
            sort(*edge.next);
        }
    }

}

// trie::node

trie::node::node() = default;

trie::node::~node() = default;

// trie

trie::trie() = default;

trie::~trie() = default;

trie::size_type trie::size() const
{
    return count;
}

trie::string_view trie::find(string_view word) const
{
    return ::find(root, word);
}

void trie::insert(string_view word)
{
    count += ::insert(root, word, word);
}

void trie::sort()
{
    ::sort(root);
}
