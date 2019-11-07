#ifndef WSS_LOAD_LEXICON_H
#define WSS_LOAD_LEXICON_H

#include <optional>
#include <string_view>

#include "trie.h"

auto load_lexicon(std::string_view filename)
-> std::optional<trie>;

#endif //WSS_LOAD_LEXICON_H
