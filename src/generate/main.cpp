// Copyright 2019 John McFarlane
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "load_lexicon.h"

#include <ssize.h>
#include <wss_assert.h>

#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using node_map = std::map<node, std::string>;

void dump_word(
        int const root_node_index,
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
                    std::pair{next_node, word});
            dump_word(root_node_index, next_node, word, nodes, source_cpp);
            word_parts.push_back(word);
        }

        word.pop_back();
    }

    auto const id{word.empty()
            ? fmt::format("root_node{}", root_node_index)
            : fmt::format("n{}{}", word, n.root_index)};
    auto const edges_id{id+"e"};

    // edge array
    if (!word_parts.empty()) {
        source_cpp << "node " << edges_id << "[] {";
        for (auto i{0}; i!=ssize(word_parts); ++i) {
            if (i!=0) {
                source_cpp << ",";
            }
            source_cpp << fmt::format("n{}{}", word_parts[i],
                    n.edges[i].ptr()->root_index);
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
        multi_trie const& lexicon,
        std::vector<std::string> const& names,
        std::string_view source_filename)
{
    auto const source_cpp_filename{fmt::format("{}.cpp", source_filename)};
    auto const source_h_filename{fmt::format("{}.h", source_filename)};

    std::ofstream source_cpp(source_cpp_filename);

    source_cpp << "#include \"" << source_h_filename << "\"\n";
    source_cpp << "namespace {\n";

    std::string word;
    node_map nodes;
    auto const& root_nodes = lexicon.root_nodes();
    for (auto index = 0;
            index!=int(root_nodes.size());
            ++index) {
        dump_word(index, root_nodes[index], word, nodes,
                source_cpp);
    }

    source_cpp << "} //namespace\n";

    std::ofstream source_h(source_h_filename);
    source_h << "#include <node.h>\n";
    for (auto index = 0;
            index!=int(root_nodes.size());
            ++index) {
        auto const name = names[index];
        source_cpp
                << fmt::format(
                        "node const& {}{{root_node{}}};\n",
                        name,
                        index);

        source_h << "extern node const& " << name << ";\n";
    }
}

auto main(int argc, char const* const* argv) -> int
{
    auto lexicon_filenames = std::vector<std::string>{2};
    auto names = std::vector<std::string>{2};
    auto source_filename = std::string{};
    bool help{false};
    auto cli{
            lyra::help(help)
                    | lyra::arg(lexicon_filenames[0], "lexicon1")(
                            "text file containing 1st list of words")
                    | lyra::arg(names[0], "name1")("Name of 1st lexicon")
                    | lyra::arg(lexicon_filenames[1], "lexicon2")(
                            "text file containing 2nd list of words")
                    | lyra::arg(names[1], "name2")("Name of 2st lexicon")
                    | lyra::arg(source_filename, "source")("source filename")
    };
    auto result = cli.parse(lyra::args(argc, argv));
    WSS_ASSERT(result);

    if (help) {
        fmt::printf("wss lexicon source file generator\n"
                    "(C)2019 John McFarlane\n\n");
        for (auto const& help_column : cli.get_help_text()) {
            fmt::printf("%.10s   %s\n", help_column.option,
                    help_column.description);
        }
        return EXIT_FAILURE;
    }

    auto const lexicon = load_lexicon(lexicon_filenames);
    if (!lexicon) {
        return EXIT_FAILURE;
    }

    dump_lexicon(*lexicon, names, source_filename);
}
