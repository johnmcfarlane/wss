
#include <vector>
#include <experimental/string_view>

class trie {
    using string_view = std::experimental::string_view;
public:
    struct edge;

    struct node {
        node();

        node(node const&) = delete;

        node(node&&) = default;

        ~node();

        using edge_vector = std::vector<edge>;
        edge_vector edges;
        string_view word;
    };

    using size_type = long long;

    trie();

    trie(trie const&) = delete;

    trie(trie&&) = default;

    ~trie();

    size_type size() const;

    string_view find(string_view word) const;

    void insert(string_view word);

    void sort();

    template<typename ResultFunction>
    void for_each(ResultFunction const& f) const;

    template<typename EntryFunction, typename ExitFunction, typename ResultFunction>
    void search(EntryFunction on_entry, ExitFunction on_exit, ResultFunction f) const;

private:

    node root;
    size_type count = 0;
};
