// Copyright 2020 John McFarlane
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

#ifndef WSS_LETTER_SET_H
#define WSS_LETTER_SET_H

#include "wss_assert.h"

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <numeric>
#include <ostream>

class letter_set;
constexpr letter_set from_bits(std::uint32_t) noexcept;

class letter_set {
public:
    using rep = std::uint32_t;

    static const letter_set all;
    static const letter_set none;

    explicit constexpr letter_set() noexcept
            :_bits{0} { }

    constexpr letter_set(letter_set const&) noexcept = default;

    explicit letter_set(std::initializer_list<char> const& letters) noexcept
            :_bits{std::accumulate(
                    std::begin(letters), std::end(letters),
                    rep{0},
                    [](auto bits, auto letter) {
                        return bits | bit(letter);
                    })} { }

    void reset(char letter)
    {
        _bits &= ~bit(letter);
    }

    auto set(char letter)
    {
        _bits |= bit(letter);
    }

    auto set(char letter, bool value)
    {
        return value ? set(letter) : reset(letter);
    }

    [[nodiscard]] auto operator[](char letter) const
    {
        return (_bits & bit(letter))!=0;
    }

    [[nodiscard]] constexpr auto bits() const
    {
        return _bits;
    }

    [[nodiscard]] friend constexpr letter_set from_bits(rep bits) noexcept
    {
        letter_set letters;
        letters._bits = bits;
        return letters;
    }

    class const_iterator;

    [[nodiscard]] const_iterator find(char l) const;

private:
    [[nodiscard]] static int bit(char letter)
    {
        return rep{1} << index(letter);
    }

    [[nodiscard]] static int index(char letter)
    {
        WSS_ASSERT(letter=='\0'
                || std::isalpha(letter)
                || std::isalpha(letter-1));
        return letter & 0x1f;
    }

    rep _bits;
};

[[nodiscard]] inline constexpr auto operator|(
        letter_set const& l, letter_set const& r)
{
    return from_bits(l.bits() | r.bits());
}

class letter_set::const_iterator {
public:
    using difference_type = int;
    using value_type = char const;
    using pointer = char const*;
    using reference = char const&;
    using iterator_category = std::forward_iterator_tag;

    constexpr const_iterator(const_iterator const&) = default;

private:
    constexpr const_iterator(char l, letter_set::rep b) noexcept
            :_letter(l), _bits(b) { }

public:
    friend auto operator*(const_iterator const& rhs)
    {
        WSS_ASSERT(isupper(rhs._letter));
        WSS_ASSERT(rhs._bits & 1);
        WSS_ASSERT(rhs._bits>1);
        return rhs._letter;
    }

    friend auto& operator++(const_iterator& rhs)
    {
        WSS_ASSERT(rhs._bits>1);
        auto const inc{__builtin_ctz(rhs._bits & ~letter_set::rep{1})};
        rhs._letter += inc;
        rhs._bits = rhs._bits >> inc;
        return rhs;
    }

    [[nodiscard]] friend auto operator-(
            const_iterator const& lhs, const_iterator const& rhs)
    {
        WSS_ASSERT((lhs._bits & 1)==(rhs._bits & 1));
        auto const lhs_num_bits{__builtin_popcount(lhs._bits)};
        auto const rhs_num_bits{__builtin_popcount(rhs._bits)};
        return rhs_num_bits-lhs_num_bits;
    }

    friend constexpr auto operator==(
            const_iterator const& lhs,
            const_iterator const& rhs)
    {
        return lhs._letter==rhs._letter;
    }

    friend constexpr auto operator!=(
            const_iterator const& lhs,
            const_iterator const& rhs)
    {
        return lhs._letter!=rhs._letter;
    }

    friend auto begin(letter_set const&) -> letter_set::const_iterator;

    friend auto end(letter_set const&) -> letter_set::const_iterator;

    friend const_iterator letter_set::find(char l) const;

private:
    friend auto sentinel(letter_set const&) -> letter_set::const_iterator;
    
    char _letter;
    letter_set::rep _bits;
};

[[nodiscard]] auto begin(letter_set const&) -> letter_set::const_iterator;

[[nodiscard]] auto end(letter_set const&) -> letter_set::const_iterator;

#endif //WSS_LETTER_SET_H
