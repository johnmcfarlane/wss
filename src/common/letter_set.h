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

#include <bit>
#include <cctype>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <numeric>

class letter_set;
constexpr auto from_bits(std::uint32_t /*bits*/) noexcept -> letter_set;

class letter_set {
public:
    using rep = std::uint32_t;

    static const letter_set all;
    static const letter_set none;

    constexpr letter_set() noexcept = default;

    constexpr letter_set(std::initializer_list<char> const& letters) noexcept
        : _bits{std::accumulate(
                std::begin(letters), std::end(letters),
                rep{0},
                [](auto bits, auto letter) {
                    return bits | bit(letter);
                })}
    {
    }

    constexpr void reset(char letter)
    {
        _bits &= ~bit(letter);
    }

    constexpr void set(char letter)
    {
        _bits |= bit(letter);
    }

    constexpr auto set(char letter, bool value)
    {
        return value ? set(letter) : reset(letter);
    }

    [[nodiscard]] constexpr auto operator[](char letter) const
    {
        return (_bits & bit(letter)) != 0;
    }

    constexpr auto operator==(letter_set const& that) const
    {
        return _bits == that._bits;
    }

    constexpr auto operator!=(letter_set const& that) const
    {
        return _bits != that._bits;
    }

    [[nodiscard]] constexpr auto bits() const
    {
        return _bits;
    }

    [[nodiscard]] friend constexpr auto from_bits(rep bits) noexcept -> letter_set
    {
        letter_set letters;
        letters._bits = bits;
        return letters;
    }

    class const_iterator;

    [[nodiscard]] auto find(char l) const -> const_iterator;

private:
    static constexpr auto isalpha(char letter)
    {
        return (letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z');
    }

    [[nodiscard]] static constexpr auto bit(char letter) -> int
    {
        return rep{1} << index(letter);
    }

    [[nodiscard]] static constexpr auto index(char letter) -> int
    {
        constexpr auto letter_mask{0x1f};
        WSS_ASSERT(letter == '\0' || isalpha(letter) || letter == 'Z' + 1 || letter == 'z' + 1);
        return letter & letter_mask;
    }

    rep _bits{};
};

[[nodiscard]] inline constexpr auto operator~(letter_set const& letters)
{
    return from_bits(~letters.bits() & letter_set::all.bits());
}

[[nodiscard]] inline constexpr auto operator&(
        letter_set const& l, letter_set const& r)
{
    return from_bits(l.bits() & r.bits());
}

[[nodiscard]] inline constexpr auto operator|(
        letter_set const& l, letter_set const& r)
{
    return from_bits(l.bits() | r.bits());
}

[[nodiscard]] inline constexpr auto operator^(
        letter_set const& l, letter_set const& r)
{
    return from_bits(l.bits() ^ r.bits());
}

class letter_set::const_iterator {
public:
    using difference_type = int;
    using value_type = char const;
    using pointer = char const*;
    using reference = char const&;
    using iterator_category = std::forward_iterator_tag;

private:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    constexpr const_iterator(char l, letter_set::rep b) noexcept
        : _letter(l)
        , _bits(b)
    {
    }

public:
    friend auto operator*(const_iterator const& rhs)
    {
        WSS_ASSERT(isupper(rhs._letter));
        WSS_ASSERT(rhs._bits & 1);
        WSS_ASSERT(rhs._bits > 1);
        return rhs._letter;
    }

    friend auto operator++(const_iterator& rhs) -> auto&
    {
        WSS_ASSERT(rhs._bits > 1);
        auto const inc{std::countr_zero(rhs._bits & ~letter_set::rep{1})};
        rhs._letter += inc;
        rhs._bits = rhs._bits >> inc;
        return rhs;
    }

    [[nodiscard]] friend auto operator-(
            const_iterator const& lhs, const_iterator const& rhs)
    {
        WSS_ASSERT((lhs._bits & 1) == (rhs._bits & 1));
        auto const lhs_num_bits{std::popcount(lhs._bits)};
        auto const rhs_num_bits{std::popcount(rhs._bits)};
        return rhs_num_bits - lhs_num_bits;
    }

    friend constexpr auto operator==(
            const_iterator const& lhs,
            const_iterator const& rhs)
    {
        return lhs._letter == rhs._letter;
    }

    friend constexpr auto operator!=(
            const_iterator const& lhs,
            const_iterator const& rhs)
    {
        return lhs._letter != rhs._letter;
    }

    friend auto begin(letter_set const& /*letters*/) -> letter_set::const_iterator;

    friend auto end(letter_set const& /*unused*/) -> letter_set::const_iterator;

    friend auto letter_set::find(char l) const -> const_iterator;

private:
    friend auto sentinel(letter_set const& /*letters*/) -> letter_set::const_iterator;

    char _letter;
    letter_set::rep _bits;
};

[[nodiscard]] auto begin(letter_set const& /*letters*/) -> letter_set::const_iterator;

[[nodiscard]] auto end(letter_set const& /*unused*/) -> letter_set::const_iterator;

#endif  // WSS_LETTER_SET_H
