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
#include "trie.h"

#include <letter_set.h>
#include <ssize.h>

#include <fmt/format.h>
#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std::literals;

using node_map = std::map<node, std::string>;
using edge_map = std::unordered_map<
        std::string,
        std::tuple<std::string, int>>;

void write_nodes(
        node const& n,
        std::string& word,
        node_map& nodes,
        edge_map& edges,
        std::ofstream& source_cpp)
{
    letter_set letters;
    if (n.is_terminator) {
        letters.set('\0');
    }

    std::vector<std::string> word_parts;

    for (auto const& e : n) {
        word.push_back(e);
        letters.set(e);

        auto const& next_node{e.get_next()};
        auto const found{nodes.equal_range(next_node)};
        if (found.first != found.second) {
            word_parts.push_back(found.first->second);
        } else {
            nodes.emplace_hint(found.first, std::pair{next_node, word});
            write_nodes(next_node, word, nodes, edges, source_cpp);
            word_parts.push_back(word);
        }

        word.pop_back();
    }

    auto const id{word.empty() ? fmt::format("root_node") : fmt::format("n{}", word)};
    auto edges_line{std::string{}};

    // edge array
    if (!word_parts.empty()) {
        auto node_edges{std::string{}};
        for (auto i{0}; i != ssize(word_parts); ++i) {
            if (i != 0) {
                node_edges += ',';
            }
            node_edges += fmt::format("n{}", word_parts[i]);
        }

        auto found_edge = edges.find(node_edges);
        if (found_edge != std::end(edges)) {
            auto [edges_id, element_index] = found_edge->second;
            if (element_index != 0) {
                edges_line = edges_id + '+' + std::to_string(element_index);
            } else {
                edges_line = edges_id;
            }
        } else {
            auto sub_array{std::string{}};
            auto const edges_id = id + 'e';
            for (auto i{ssize(word_parts) - 1}; i >= 0; --i) {
                sub_array = fmt::format("n{}{}", word_parts[i], sub_array);
                edges[sub_array] = make_tuple(edges_id, i);
                if (i != 0) {
                    sub_array = ","s.append(sub_array);
                }
            }
            source_cpp << "node const " << edges_id << "[] {";
            source_cpp << sub_array
                       << "};\n";
            edges_line = edges_id;
        }
    } else {
        edges_line = "nullptr";
    }

    // node
    source_cpp << "node const " << id << " = {\n";

    // node::letters
#if defined(WSS_SHOW_LETTERS)
    source_cpp << "  letter_set{";
    auto first_letter{true};
    for (auto letter : n) {
        if (!first_letter) {
            source_cpp << ", ";
        } else {
            first_letter = false;
        }
        source_cpp << '\'' << letter << '\'';
    }
    source_cpp << "},\n";
#else
    source_cpp << "  from_bits(0x" << std::hex << letters.bits() << "U),\n";
#endif

    // node::edges
    source_cpp << "  " << edges_line << "\n};\n";
}

void write_lexicon(
        trie const& lexicon,
        std::string const& name,
        std::string_view source_filename)
{
    fmt::print("saving...\n");

    auto const source_cpp_filename{fmt::format("{}.cpp", source_filename)};
    auto const source_h_filename{fmt::format("{}.h", source_filename)};

    std::ofstream source_cpp(source_cpp_filename);

    source_cpp << "#include \"" << source_h_filename << "\"\n";
    source_cpp << "namespace {\n";

    std::string word;
    node_map nodes;
    edge_map edges;
    auto const& root_node = lexicon.root_node();
    write_nodes(root_node, word, nodes, edges, source_cpp);

    source_cpp << "} //namespace\n";

    std::ofstream source_h(source_h_filename);
    source_h << "#include <node.h>\n";
    source_cpp
            << fmt::format(
                       "node const& {}{{root_node}};\n",
                       name);

    source_h << "extern node const& " << name << ";\n";
}

auto main(int argc, char const* const* argv) -> int
{
    auto lexicon_filename = std::string{};
    auto name = std::string{};
    auto source_filename = std::string{};
    bool help{false};
    auto cli{
            lyra::help(help)
            | lyra::arg(lexicon_filename, "lexicon")(
                    "text file containing list of words")
            | lyra::arg(name, "name")("Name of lexicon")
            | lyra::arg(source_filename, "source")("source filename")};
    auto result = cli.parse(lyra::args(argc, argv));

    if (!result) {
        fmt::print(stderr, "Command line error: {}\n", result.message());
        return EXIT_FAILURE;
    }

    if (help) {
        fmt::printf("wss lexicon source file generator\n"
                    "(C)2019 John McFarlane\n\n");
        std::cout << cli;
        return EXIT_FAILURE;
    }

    auto const lexicon = load_lexicon(lexicon_filename);
    if (!lexicon) {
        return EXIT_FAILURE;
    }

    write_lexicon(*lexicon, name, source_filename);
}
