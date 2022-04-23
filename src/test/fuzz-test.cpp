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

#include "fuzz-test.h"

#include <command_line.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iterator>
#include <utility>

command_line_data::command_line_data(std::vector<std::string> p)
    : _parameters(std::move(p))
{
    std::transform(
            std::begin(_parameters), std::end(_parameters),
            std::back_inserter(_parameters_starts),
            [](auto const& s) { return s.c_str(); });
}

auto command_line_data::command_line() const -> ::command_line
{
    return _parameters_starts;
}

fuzzer_input::fuzzer_input(std::uint8_t const* data, std::size_t size)
    : _bytes(data, size)
{
}

auto fuzzer_input::read_string(int length) -> std::optional<std::string>
{
    if (length > size()) {
        return std::nullopt;
    }

    auto const s{std::string{_bytes.data(), _bytes.data() + length}};
    _bytes = _bytes.subspan(length);

    return s;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto fuzzer_input::read_command_line(int max_parameters, int max_parameter_size)
        -> std::optional<command_line_data>
{
    auto const num_parameters{read_integer(1, max_parameters)};
    if (!num_parameters) {
        return std::nullopt;
    }

    auto parameters{std::vector<std::string>{}};
    for (auto i{0}; i != *num_parameters; ++i) {
        auto const parameter_size{read_integer<int>(1, max_parameter_size)};
        if (!parameter_size) {
            return std::nullopt;
        }

        auto parameter{read_string(*parameter_size)};
        if (!parameter) {
            return std::nullopt;
        }

        parameters.push_back(std::move(*parameter));
    }

    return std::optional{command_line_data(std::move(parameters))};
}

auto fuzzer_input::size() const -> std::ptrdiff_t
{
    return static_cast<std::ptrdiff_t>(_bytes.size());
}

auto fuzzer_input::read_buffer(void* destination, int num_bytes) -> bool
{
    if (num_bytes > size()) {
        return false;
    }

    std::memcpy(destination, _bytes.data(), num_bytes);
    _bytes = _bytes.subspan(num_bytes);

    return true;
}

#if !defined(WSS_USE_LIBFUZZER)
auto main() -> int
{
    std::puts("libfuzzer not enabled");
}
#endif
