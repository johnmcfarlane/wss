#include "load_lexicon.h"

#include <ssize.h>

#include <fmt/printf.h>

#include <algorithm>
#include <cctype>

auto load_lexicon(std::string_view filename, int min_letters, int max_letters)
-> std::optional<trie>
{
    trie lexicon;

    auto f{std::unique_ptr<std::FILE, decltype(&fclose)>{
            std::fopen(filename.data(), "r"), &fclose}};

    if (!f) {
        fmt::print(stderr, "error: Could not open '{}'.\n", filename);
        return std::nullopt;
    }

    constexpr auto max_word{1024};
    std::array<char, max_word+1> line{};
    while (std::fgets(line.data(), max_word, f.get())!=nullptr) {
        auto const newline{std::find(begin(line), end(line), '\n')};
        if (newline==end(line)) {
            fmt::print("error: missing newline at end of '{}'", line.data());
            return std::nullopt;
        }

        std::transform(std::begin(line), newline, std::begin(line),
                [](auto c) { return std::toupper(c); });

        auto word{std::string_view{line.data(),
                std::size_t(std::distance(begin(line), newline))}};

        if (ssize(word)<min_letters) {
            continue;
        }

        if (ssize(word)>max_letters) {
            continue;
        }

        lexicon.insert(word);
    }

    lexicon.compress();

    return lexicon;
}
