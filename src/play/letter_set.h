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

#include <wss_assert.h>

#include <bitset>
#include <cstdint>

class letter_set
{
    explicit letter_set(std::uint32_t bits) noexcept : _bitfield{bits} {}
public:
    static const letter_set all;
    static const letter_set none;

    explicit letter_set() = default;

    void reset(char letter)
    {
        _bitfield.reset(index(letter));
    }

    auto set(char letter, bool value = true)
    {
        return _bitfield.set(index(letter), value);
    }

    auto operator[](char letter) const
    {
        return _bitfield[index(letter)];
    }
private:
    [[nodiscard]] static int index(char letter)
    {
        WSS_ASSERT(std::isalpha(letter));
        return letter & 0x1f;
    }

    std::bitset<32> _bitfield;
    static_assert(sizeof(_bitfield)==2*sizeof(std::uint32_t));
};

#endif //WSS_LETTER_SET_H
