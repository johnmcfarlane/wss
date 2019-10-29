#ifndef WSS_TRIE_H
#define WSS_TRIE_H

#include <gsl/gsl_assert>

#include <optional>
#include <string_view>
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
    bool is_terminator{false};
};

class edge {
public:
    edge(edge const&) { Expects(false); }
    edge(edge&& that) = default;
    edge(char l, std::unique_ptr<node> n);

    operator char() const;

    node& get_next();

    node const& get_next() const;

private:
    char letter;
    std::unique_ptr<node> next;
};

class trie {
    using string_view = std::string_view;

public:
    using size_type = std::ptrdiff_t;

    class edge_const_iterator {
        using const_iterator = node::edge_vector::const_iterator;
    public:
        using difference_type = const_iterator::difference_type;
        using value_type = const_iterator::value_type;
        using pointer = const_iterator::pointer;
        using reference = const_iterator::reference;
        using iterator_category = const_iterator::iterator_category;

        edge_const_iterator(const_iterator i)
                :pos(i) { }

        friend auto operator==(edge_const_iterator a, edge_const_iterator b)
        {
            return a.pos==b.pos;
        }
        friend auto operator!=(edge_const_iterator a, edge_const_iterator b)
        {
            return a.pos!=b.pos;
        }

        friend auto operator++(edge_const_iterator& that)
        {
            ++that.pos;
            return that;
        }
        friend auto operator-(
                edge_const_iterator& lhs, edge_const_iterator& rhs)
        {
            return lhs.pos-rhs.pos;
        }

        friend auto& operator*(edge_const_iterator const& that)
        {
            return that.pos.operator*();
        }
        auto* operator->() const
        {
            return pos.operator->();
        }

    private:
        const_iterator pos;
    };

    trie();

    trie(trie const&) = delete;

    trie(trie&&) = default;

    ~trie();

    size_type size() const;
    
    node const& root_node() const;

    void insert(string_view word);

private:

    node root;
    size_type count = 0;
};

auto load_lexicon(std::string_view filename, int min_letters, int max_letters)
-> std::optional<trie>;

#endif //WSS_TRIE_H
