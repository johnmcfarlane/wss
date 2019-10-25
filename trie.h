#ifndef WSS_TRIE_H
#define WSS_TRIE_H

#include <experimental/string_view>
#include <memory>
#include <vector>

class edge;

struct node {
    using edge_vector = std::vector<edge>;

    node();

    node(node const&) = delete;

    node(node&&) = default;

    ~node();

    edge_vector::const_iterator begin() const;

    edge_vector::const_iterator end() const;

    edge_vector edges;
    bool is_terminator;
};

class edge {
public:
    edge(char l, std::unique_ptr<node> n);

    operator char() const;

    node& get_next();

    node const& get_next() const;

private:
    char letter;
    std::unique_ptr<node> next;
};

class trie {
    using string_view = std::experimental::string_view;

public:
    using size_type = std::ptrdiff_t;

    class edge_const_iterator {
        using const_iterator = node::edge_vector::const_iterator;
    public:
        edge_const_iterator(const_iterator i)
                :pos(i) { }

        friend auto operator!=(edge_const_iterator a, edge_const_iterator b)
        {
            return a.pos!=b.pos;
        }

        friend auto operator++(edge_const_iterator& that)
        {
            ++that.pos;
            return that;
        }

        friend auto& operator*(edge_const_iterator const& that)
        {
            return *that.pos;
        }

    private:
        const_iterator pos;
    };

    trie();

    trie(trie const&) = delete;

    trie(trie&&) = default;

    ~trie();

    size_type size() const;

    edge_const_iterator begin() const;

    edge_const_iterator end() const;

    void insert(string_view word);

private:

    node root;
    size_type count = 0;
};

#endif //WSS_TRIE_H
