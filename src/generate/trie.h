#ifndef WSS_TRIE_H
#define WSS_TRIE_H

#include <gsl/gsl_assert>

#include <memory>
#include <string_view>
#include <vector>

class edge;

struct node {
    using edge_vector = std::vector<edge>;

    auto begin() const -> edge_vector::const_iterator;

    edge_vector::const_iterator end() const;

    edge_vector edges;
    bool is_terminator{false};
};

class edge {
public:
    edge(char l, std::unique_ptr<node> n);

    operator char() const;

    std::shared_ptr<node> const& ptr() const;

    node& get_next();
    node const& get_next() const;

    void set_next(std::shared_ptr<node> n);

private:
    char letter;
    std::shared_ptr<node> next;
};

class trie {
    using string_view = std::string_view;

public:
    using size_type = std::ptrdiff_t;

    node const& root_node() const;

    void insert(string_view word);

    void compress();

private:

    node root;
};

inline bool operator<(edge const& lhs, edge const& rhs)
{
    return std::tie(static_cast<char const&>(lhs), lhs.ptr())
            <std::tie(static_cast<char const&>(rhs), rhs.ptr());
}

inline bool operator<(node const& lhs, node const& rhs)
{
    return std::tie(lhs.edges, lhs.is_terminator)
            <std::tie(rhs.edges, rhs.is_terminator);
}

#endif //WSS_TRIE_H
