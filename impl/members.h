//
// Created by john on 12/9/18.
//

#ifndef WSS_MEMBERS_H
#define WSS_MEMBERS_H

#include <memory>

struct trie::edge {
    char letter;
    std::unique_ptr<node> next;
};

namespace impl {
    template<typename Function>
    void for_each(trie::node const& n, Function const& f)
    {
        if (n.word.data()) {
            f(n.word);
        }

        for (auto& edge : n.edges) {
            for_each(*edge.next, f);
        }
    }

    template<typename EntryFunction, typename ExitFunction, typename ResultFunction>
    void search(trie::node const& n, EntryFunction on_entry, ExitFunction on_exit, ResultFunction f)
    {
        if (auto word = n.word; !word.empty()) {
            f(word);
        }
        for (auto const& edge : n.edges) {
            if (auto letter = edge.letter; on_entry(letter)) {
                trie::node const& next = *edge.next;
                search(next, on_entry, on_exit, f);
                on_exit(letter);
            }
        }
    }
}

template<typename Function>
void trie::for_each(Function const& f) const
{
    impl::for_each(root, f);
}

template<typename EntryFunction, typename ExitFunction, typename ResultFunction>
void trie::search(EntryFunction on_entry, ExitFunction on_exit, ResultFunction f) const
{
    impl::search(root, on_entry, on_exit, f);
}

#endif //WSS_MEMBERS_H
