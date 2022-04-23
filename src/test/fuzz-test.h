// Copyright 2022 John McFarlane
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

#if !defined(WSS_TEST_FUZZ_TEST)
#define WSS_TEST_FUZZ_TEST

#include <command_line.h>
#include <wss_assert.h>

#include <fmt/format.h>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

template<>
struct fmt::formatter<command_line> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(command_line const& c, FormatContext& ctx)
    {
        format_to(ctx.out(), "{{");

        bool need_comma = false;
        for (auto const& parameter : c) {
            if (need_comma) {
                format_to(ctx.out(), ",");
            }
            need_comma = true;

            format_to(ctx.out(), "\"{}\"", parameter);
        }

        format_to(ctx.out(), "}}");
        return ctx.out();
    }
};

class command_line_data {
public:
    explicit command_line_data(std::vector<std::string> p);
    command_line_data(command_line_data&&) = default;
    explicit command_line_data(command_line_data const&) = delete;
    ~command_line_data() = default;
    auto operator=(command_line_data&&) -> command_line_data& = default;
    auto operator=(command_line_data const&) -> command_line_data& = delete;

    [[nodiscard]] auto command_line() const -> command_line;

private:
    std::vector<std::string> _parameters;
    std::vector<char const*> _parameters_starts;
};

class fuzzer_input {
public:
    fuzzer_input(std::uint8_t const* data, std::size_t size);

    template<typename T>
        requires std::is_trivially_copyable_v<T>
    [[nodiscard]] auto read_object() -> std::optional<T>
    {
        T object;
        static_assert(std::is_trivially_copyable_v<T>);

        auto const object_size{int{sizeof(object)}};
        if (!read_buffer(&object, object_size)) {
            return std::nullopt;
        }

        return object;
    }

    template<typename Integer = int>
    [[nodiscard]] auto read_integer(Integer minimum, Integer maximum)
            -> std::optional<Integer>
    {
        WSS_ASSERT(minimum < maximum);

        auto const integer{read_object<Integer>()};
        if (!integer) {
            return std::nullopt;
        }

        if (*integer < minimum || *integer > maximum) {
            return std::nullopt;
        }

        return integer;
    }

    [[nodiscard]] auto read_string(int length)
            -> std::optional<std::string>;
    [[nodiscard]] auto read_command_line(int max_parameters, int max_parameter_size)
            -> std::optional<command_line_data>;

private:
    [[nodiscard]] auto size() const -> std::ptrdiff_t;
    auto read_buffer(void* destination, int num_bytes) -> bool;

    std::span<std::uint8_t const> _bytes;
};

#endif  // WSS_TEST_FUZZ_TEST
