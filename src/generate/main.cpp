#include "load_lexicon.h"

#include <ssize.h>

#include <clara.hpp>
#include <fmt/printf.h>

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using node_map = std::map<node, std::string>;

void dump_word(
        node const& n,
        std::string& word,
        node_map& nodes,
        std::ofstream& source_cpp)
{
    std::string letters;
    std::vector<std::string> word_parts;

    for (auto const& e : n) {
        word.push_back(e);
        letters.push_back(e);

        auto const& next_node{e.get_next()};
        auto const found{nodes.equal_range(next_node)};
        if (found.first!=found.second) {
            word_parts.push_back(found.first->second);
        }
        else {
            nodes.emplace_hint(found.first,
                    std::make_pair(next_node, word));
            dump_word(next_node, word, nodes, source_cpp);
            word_parts.push_back(word);
        }

        word.pop_back();
    }

    auto const id{(word.empty() ? std::string{"root_node"} : "n" + word)};
    auto const edges_id{id+"e"};
    
    // edge array
    if (!word_parts.empty()) {
        source_cpp << "node " << edges_id << "[] {";
        for (auto i{0}; i!=ssize(word_parts); ++i) {
            if (i!=0) {
                source_cpp << ",";
            }
            source_cpp << "n" + word_parts[i];
        }
        source_cpp << "}; //NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)\n";
    }

    // node
    source_cpp << "node " << id << " = {\n";
    
    // node::letters
    source_cpp << "  \"" << letters << "\",\n";
    
    // node::edges
    if (!word_parts.empty()) {
        source_cpp << "  " << edges_id << ", //NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)\n";
    }
    else {
        source_cpp << "  nullptr,\n";
    }
    
    // node::is_terminator
    source_cpp << "  " << (n.is_terminator ? "true" : "false") << '\n';
    source_cpp << "};\n";
}

void dump_lexicon(
        trie const& lexicon,
        std::string_view const& name)
{
    auto const source_cpp_filename{std::string{name}+".cpp"};
    auto const source_h_filename{std::string{name}+".h"};

    std::ofstream source_cpp(source_cpp_filename);

    source_cpp << "#include \"" << source_h_filename << "\"\n";
    source_cpp << "namespace {\n";

    std::string word;
    node_map nodes;
    dump_word(lexicon.root_node(), word, nodes, source_cpp);

    source_cpp << "} //namespace\n";

    source_cpp << "node const& " << name << "{root_node};\n";

    std::ofstream source_h(source_h_filename);
    source_h << "#include <node.h>\n";
    source_h << "extern node const& " << name << ";\n";
}

int main(int argc, char const* const* argv)
{
    using clara::Arg;
    using clara::Args;
    using clara::Help;
    using clara::Opt;

    auto lexicon_filename{std::string{}};
    auto name{std::string{}};
    auto help{false};
    auto cli{
            Arg(lexicon_filename, "lexicon")(
                    "text file containing list of words")
                    | Arg(name, "name")(
                            "Name of lexicon / source filename")
                    | Help(help)};
    auto result = cli.parse(Args(argc, argv));

    if (!result) {
        fmt::print(stderr, "Error in command line: {}\n",
                result.errorMessage().c_str());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss lexicon source file generator\n"
                    "(C)2019 John McFarlane\n\n");
        for (auto const& help_column : cli.getHelpColumns()) {
            fmt::printf("{10}   {}\n", help_column.left, help_column.right);
        }
        return EXIT_FAILURE;
    }

    auto const lexicon{
            load_lexicon(lexicon_filename, 0, std::numeric_limits<int>::max())};
    if (!lexicon) {
        fmt::print(stderr, "error: failed to lexicon from {}\n",
                lexicon_filename);
        return EXIT_FAILURE;
    }

    dump_lexicon(*lexicon, name);
}
